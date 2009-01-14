/* $Id$ */

/*
 * Copyright (c) 2001-2004 Aaron Turner.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright owners nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This code allows us to use tcpdump to print packet decodes.
 * Basically, we create a local AF_UNIX socketpair, fork a copy
 * of ourselves, link 1/2 of the pair to STDIN of the child and
 * replace the child with tcpdump.  We then send a "pcap" file
 * over the socket so that tcpdump can print it's decode to STDOUT.
 *
 * Idea and a lot of code stolen from Christain Kreibich's
 *  <christian@whoop.org> libnetdude 0.4 code.  Any bugs are mine. :)
 *
 * This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors
 */

#include "config.h"
#include "defines.h"
#include "common.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "tcpdump.h"
#include "lib/strlcpy.h"

#ifdef DEBUG
extern int debug;
#endif

char *options_vec[OPTIONS_VEC_SIZE];
static int tcpdump_fill_in_options(char *opt);
static int can_exec(const char *filename);

/**
 * given a packet, print a decode of via tcpdump
 */
int
tcpdump_print(tcpdump_t *tcpdump, struct pcap_pkthdr *pkthdr, const u_char *data)
{
    struct pollfd poller[1];
    int result;
    char decode[TCPDUMP_DECODE_LEN];

    assert(tcpdump);
    assert(pkthdr);
    assert(data);

    poller[0].fd = tcpdump->infd;
    poller[0].events = POLLOUT;
    poller[0].revents = 0;
    
    /* wait until we can write to the tcpdump socket */
    result = poll(poller, 1, TCPDUMP_POLL_TIMEOUT);
    if (result < 0)
        errx(-1, "Error during poll() to write to tcpdump\n%s", strerror(errno));

    if (result == 0)
        err(-1, "poll() timeout... tcpdump seems to be having a problem keeping up\n"
            "Try increasing TCPDUMP_POLL_TIMEOUT");


    /* result > 0 if we get here */

    if (write(tcpdump->infd, (char *)pkthdr, sizeof(struct pcap_pkthdr))
        != sizeof(struct pcap_pkthdr))
        errx(-1, "Error writing pcap file header to tcpdump\n%s", strerror(errno));

#ifdef DEBUG
    if (debug >= 5) {
        if (write(tcpdump->debugfd, (char *)pkthdr, sizeof(struct pcap_pkthdr))
            != sizeof(struct pcap_pkthdr))
            errx(-1, "Error writing pcap file header to tcpdump debug\n%s", strerror(errno));
    }
#endif

    if (write(tcpdump->infd, data, pkthdr->caplen) != (ssize_t)pkthdr->caplen)
        errx(-1, "Error writing packet data to tcpdump\n%s", strerror(errno));

#ifdef DEBUG
    if (debug >= 5) {
        if (write(tcpdump->debugfd, data, pkthdr->caplen) != (ssize_t)pkthdr->caplen)
            errx(-1, "Error writing packet data to tcpdump debug\n%s", strerror(errno));
    }
#endif

    /* Wait for output from tcpdump */
    poller[0].fd = tcpdump->outfd;
    poller[0].events = POLLIN;
    poller[0].revents = 0;

    result = poll(poller, 1, TCPDUMP_POLL_TIMEOUT);
    if (result < 0)
        errx(-1, "Error during poll() to write to tcpdump\n%s", strerror(errno));

    if (result == 0)
        err(-1, "poll() timeout... tcpdump seems to be having a problem keeping up\n"
            "Try increasing TCPDUMP_POLL_TIMEOUT");

    /* result > 0 if we get here */
    if (read(tcpdump->outfd, &decode, TCPDUMP_DECODE_LEN) < 0)
        errx(-1, "Error reading tcpdump decode: %s", strerror(errno));
            
    printf("%s", decode);

    return TRUE;
}

/**
 * init our tcpdump handle using the given pcap handle
 * Basically, this starts up tcpdump as a child and communicates
 * to it via a pair of sockets (stdout/stdin)
 */
int
tcpdump_open(tcpdump_t *tcpdump, pcap_t *pcap)
{
    int infd[2], outfd[2];
    FILE *writer;
    
    assert(tcpdump);
    assert(pcap);

    if (tcpdump->pid != 0) {
        warn("tcpdump process already running");
        return FALSE;
    }

    /* is tcpdump executable? */
    if (! can_exec(TCPDUMP_BINARY)) {
        errx(-1, "Unable to execute tcpdump binary: %s", TCPDUMP_BINARY);
    }
    
#ifdef DEBUG
     strlcpy(tcpdump->debugfile, TCPDUMP_DEBUG, sizeof(tcpdump->debugfile));
     if (debug >= 5) {
         dbgx(5, "Opening tcpdump debug file: %s", tcpdump->debugfile);

         if ((tcpdump->debugfd = open(tcpdump->debugfile, O_WRONLY|O_CREAT|O_TRUNC, 
                S_IREAD|S_IWRITE|S_IRGRP|S_IROTH)) == -1) {
            errx(-1, "Error opening tcpdump debug file: %s\n%s", tcpdump->debugfile, strerror(errno));
        }
    }
#endif

    /* copy over the args */
    dbg(2, "Prepping tcpdump options...");
    tcpdump_fill_in_options(tcpdump->args);

    dbg(2, "Starting tcpdump...");

    /* create our socket pair to send packet data to tcpdump via */
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, infd) < 0)
        errx(-1, "Unable to create stdin socket pair: %s", strerror(errno));

    /* create our socket pair to read packet decode from tcpdump */
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, outfd) < 0)
        errx(-1, "Unable to create stdout socket pair: %s", strerror(errno));
 
         
    if ((tcpdump->pid = fork() ) < 0)
        errx(-1, "Fork failed: %s", strerror(errno));

    dbgx(2, "tcpdump pid: %d", tcpdump->pid);
    
    if (tcpdump->pid > 0) {
        /* we're still in tcpreplay */
        dbgx(2, "[parent] closing input fd %d", infd[1]);
        close(infd[1]);  /* close the tcpdump side */
        dbgx(2, "[parent] closing output fd %d", outfd[1]);
        close(outfd[1]);
        tcpdump->infd = infd[0];
        tcpdump->outfd = outfd[0];

        /* send the pcap file header to tcpdump */
        writer = fdopen(tcpdump->infd, "w");
        if ((tcpdump->dumper = pcap_dump_fopen(pcap, writer)) == NULL) {
            warnx("[parent] pcap_dump_fopen(): %s", pcap_geterr(pcap));
            return FALSE;
        }
        pcap_dump_flush(tcpdump->dumper);

        if (fcntl(tcpdump->infd, F_SETFL, O_NONBLOCK) < 0)
            warnx("[parent] Unable to fcntl tcpreplay socket:\n%s", strerror(errno));

        if (fcntl(tcpdump->outfd, F_SETFL, O_NONBLOCK) < 0)
            warnx("[parent] Unable to fnctl stdout socket:\n%s", strerror(errno));
            
    }
    else {
        dbg(2, "[child] started the kid");

        /* we're in the child process */
        dbgx(2, "[child] closing in fd %d", infd[0]);
        dbgx(2, "[child] closing out fd %d", outfd[0]);
        close(infd[0]); /* close the tcpreplay side */
        close(outfd[0]);
    
        /* copy our side of the socketpair to our stdin */
        if (infd[1] != STDIN_FILENO) {
            if (dup2(infd[1], STDIN_FILENO) != STDIN_FILENO)
                errx(-1, "[child] Unable to copy socket to stdin: %s", 
                    strerror(errno));
        }
    
        /* copy our side of the socketpair to our stdout */
        if (outfd[1] != STDOUT_FILENO) {
            if (dup2(outfd[1], STDOUT_FILENO) != STDOUT_FILENO)
                errx(-1, "[child] Unable to copy socket to stdout: %s", 
                    strerror(errno));
        }

        /* exec tcpdump */
        dbg(2, "[child] Exec'ing tcpdump...");
        if (execv(TCPDUMP_BINARY, options_vec) < 0)
            errx(-1, "Unable to exec tcpdump: %s", strerror(errno));

    }
    
    return TRUE;
}

/**
 * shutdown tcpdump 
 */
void
tcpdump_close(tcpdump_t *tcpdump)
{
    if (! tcpdump)
        return;

    if (tcpdump->pid <= 0)
        return;

    dbgx(2, "[parent] killing tcpdump pid: %d", tcpdump->pid);

    kill(tcpdump->pid, SIGKILL);
    close(tcpdump->infd);
    close(tcpdump->outfd);

    if (waitpid(tcpdump->pid, NULL, 0) != tcpdump->pid)
        errx(-1, "[parent] Error in waitpid: %s", strerror(errno));

    tcpdump->pid = 0;
    tcpdump->infd = 0;
    tcpdump->outfd = 0;
}

/** 
 * forcefully kill tcpdump 
 */
void
tcpdump_kill(tcpdump_t *tcpdump)
{
    if (tcpdump->pid) {
        if (kill(tcpdump->pid, SIGTERM) != 0) {
            kill(tcpdump->pid, SIGKILL);
        }
    }
    tcpdump->infd = 0;
    tcpdump->outfd = 0;
    tcpdump->pid = 0;
}


/**
 * copy the string of args (*opt) to the vector (**opt_vec)
 * for a max of opt_len.  Returns the number of options
 * in the vector
 */
static int
tcpdump_fill_in_options(char *opt)
{
    char options[256];
    char *arg, *newarg;
    int i = 1, arglen;
    char *token = NULL;

    /* zero out our options_vec for execv() */
    memset(options_vec, '\0', OPTIONS_VEC_SIZE);
    
    /* first arg should be the binary (by convention) */
    options_vec[0] = TCPDUMP_BINARY;
       

    /* prep args */
    memset(options, '\0', 256);
    if (opt != NULL) {
        strlcat(options, opt, sizeof(options));
    }
    strlcat(options, TCPDUMP_ARGS, sizeof(options));
    dbgx(2, "[child] Will execute: tcpdump %s", options);


    /* process args */
    
    /* process the first argument */
    arg = strtok_r(options, OPT_DELIM, &token);
    arglen = strlen(arg) + 2; /* -{arg}\0 */
    newarg = (char *)safe_malloc(arglen);
    strlcat(newarg, "-", arglen); 
    strlcat(newarg, arg, arglen);
    options_vec[i++] = newarg;

    /* process the remaining args 
       note that i < OPTIONS_VEC_SIZE - 1
       because: a) we need to add '-' as an option to the end
       b) because the array has to be null terminated
    */
    while (((arg = strtok_r(NULL, OPT_DELIM, &token)) != NULL) &&
           (i < OPTIONS_VEC_SIZE - 1)) {

        arglen = strlen(arg) + 2;
        newarg = (char *)safe_malloc(arglen);
        strlcat(newarg, "-", arglen);
        strlcat(newarg, arg, arglen);
        options_vec[i++] = newarg;

    }

    /* tell -r to read from stdin */
    options_vec[i] = "-";

    return(i);
}


/**
 * can we exec the given file? 
 */
static int
can_exec(const char *filename)
{
    struct stat st;

    if (!filename || filename[0] == '\0')
        return FALSE;

    /* Stat the file to see if it's executable and
       if the user may run it.
    */
    if (lstat(filename, &st) < 0)
        return FALSE;

    if ((st.st_mode & S_IXUSR) ||
        (st.st_mode & S_IXGRP) ||
        (st.st_mode & S_IXOTH))
        return TRUE;

    return FALSE;
}
