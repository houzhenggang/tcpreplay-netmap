/*
 * Copyright (c) 2006 Aaron Turner
 * All rights reserved.
 *
 * This file is generated by scripts/dlt2name.pl which converts your pcap-bpf.h
 * header file which comes with libpcap into a header file
 * which translates DLT values to their string names as well as a list of all
 * of the available DLT types.
 *
 * Hence DO NOT EDIT THIS FILE!
 * If your DLT type is not listed here, edit the %known hash in
 * scripts/dlt2name.pl
 * 
 * This file contains data which was taken from libpcap's pcap-bpf.h.  
 * The copyright/license is included below:
 */
 
 /*-
  * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997
  *      The Regents of the University of California.  All rights reserved.
  *
  * This code is derived from the Stanford/CMU enet packet filter,
  * (net/enet.c) distributed as part of 4.3BSD, and code contributed
  * to Berkeley by Steven McCanne and Van Jacobson both of Lawrence 
  * Berkeley Laboratory.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions
  * are met:
  * 1. Redistributions of source code must retain the above copyright
  *    notice, this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright
  *    notice, this list of conditions and the following disclaimer in the
  *    documentation and/or other materials provided with the distribution.
  * 3. All advertising materials mentioning features or use of this software
  *    must display the following acknowledgement:
  *      This product includes software developed by the University of
  *      California, Berkeley and its contributors.
  * 4. Neither the name of the University nor the names of its contributors
  *    may be used to endorse or promote products derived from this software
  *    without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
  * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  * SUCH DAMAGE.
  *
  *      @(#)bpf.h       7.1 (Berkeley) 5/7/91
  *
  * @(#) $Header: /tcpdump/master/libpcap/pcap-bpf.h,v 1.34.2.6 2005/08/13 22:29:47 hannes Exp $ (LBL)
  */


/* include all the DLT types form pcap-bpf.h */

extern const char *dlt2desc[];
extern const char *dlt2name[];
#define DLT2DESC_LEN 181
#define DLT2NAME_LEN 181

#ifndef DLT_NULL
#define DLT_NULL 0
#endif

#ifndef DLT_EN10MB
#define DLT_EN10MB 1
#endif

#ifndef DLT_EN3MB
#define DLT_EN3MB 2
#endif

#ifndef DLT_AX25
#define DLT_AX25 3
#endif

#ifndef DLT_PRONET
#define DLT_PRONET 4
#endif

#ifndef DLT_CHAOS
#define DLT_CHAOS 5
#endif

#ifndef DLT_IEEE802
#define DLT_IEEE802 6
#endif

#ifndef DLT_ARCNET
#define DLT_ARCNET 7
#endif

#ifndef DLT_SLIP
#define DLT_SLIP 8
#endif

#ifndef DLT_PPP
#define DLT_PPP 9
#endif

#ifndef DLT_FDDI
#define DLT_FDDI 10
#endif

#ifndef DLT_ATM_RFC1483
#define DLT_ATM_RFC1483 11
#endif

#ifndef DLT_RAW
#define DLT_RAW 12
#endif

#ifndef DLT_ENC
#define DLT_ENC 13
#endif

#ifndef DLT_PPP_BSDOS
#define DLT_PPP_BSDOS 14
#endif

#ifndef DLT_SLIP_BSDOS
#define DLT_SLIP_BSDOS 15
#endif

#ifndef DLT_PPP_BSDOS
#define DLT_PPP_BSDOS 16
#endif

#ifndef DLT_OLD_PFLOG
#define DLT_OLD_PFLOG 17
#endif

#ifndef DLT_PFSYNC
#define DLT_PFSYNC 18
#endif

#ifndef DLT_ATM_CLIP
#define DLT_ATM_CLIP 19
#endif

#ifndef DLT_REDBACK_SMARTEDGE
#define DLT_REDBACK_SMARTEDGE 32
#endif

#ifndef DLT_PPP_SERIAL
#define DLT_PPP_SERIAL 50
#endif

#ifndef DLT_PPP_ETHER
#define DLT_PPP_ETHER 51
#endif

#ifndef DLT_SYMANTEC_FIREWALL
#define DLT_SYMANTEC_FIREWALL 99
#endif

#ifndef DLT_C_HDLC
#define DLT_C_HDLC 104
#endif

#ifndef DLT_IEEE802_11
#define DLT_IEEE802_11 105
#endif

#ifndef DLT_FRELAY
#define DLT_FRELAY 107
#endif

#ifndef DLT_LOOP
#define DLT_LOOP 108
#endif

#ifndef DLT_ENC
#define DLT_ENC 109
#endif

#ifndef DLT_LINUX_SLL
#define DLT_LINUX_SLL 113
#endif

#ifndef DLT_LTALK
#define DLT_LTALK 114
#endif

#ifndef DLT_ECONET
#define DLT_ECONET 115
#endif

#ifndef DLT_IPFILTER
#define DLT_IPFILTER 116
#endif

#ifndef DLT_PFLOG
#define DLT_PFLOG 117
#endif

#ifndef DLT_CISCO_IOS
#define DLT_CISCO_IOS 118
#endif

#ifndef DLT_PRISM_HEADER
#define DLT_PRISM_HEADER 119
#endif

#ifndef DLT_AIRONET_HEADER
#define DLT_AIRONET_HEADER 120
#endif

#ifndef DLT_HHDLC
#define DLT_HHDLC 121
#endif

#ifndef DLT_IP_OVER_FC
#define DLT_IP_OVER_FC 122
#endif

#ifndef DLT_SUNATM
#define DLT_SUNATM 123
#endif

#ifndef DLT_RIO
#define DLT_RIO 124
#endif

#ifndef DLT_PCI_EXP
#define DLT_PCI_EXP 125
#endif

#ifndef DLT_AURORA
#define DLT_AURORA 126
#endif

#ifndef DLT_IEEE802_11_RADIO
#define DLT_IEEE802_11_RADIO 127
#endif

#ifndef DLT_TZSP
#define DLT_TZSP 128
#endif

#ifndef DLT_ARCNET_LINUX
#define DLT_ARCNET_LINUX 129
#endif

#ifndef DLT_JUNIPER_MLPPP
#define DLT_JUNIPER_MLPPP 130
#endif

#ifndef DLT_JUNIPER_MLFR
#define DLT_JUNIPER_MLFR 131
#endif

#ifndef DLT_JUNIPER_ES
#define DLT_JUNIPER_ES 132
#endif

#ifndef DLT_JUNIPER_GGSN
#define DLT_JUNIPER_GGSN 133
#endif

#ifndef DLT_JUNIPER_MFR
#define DLT_JUNIPER_MFR 134
#endif

#ifndef DLT_JUNIPER_ATM2
#define DLT_JUNIPER_ATM2 135
#endif

#ifndef DLT_JUNIPER_SERVICES
#define DLT_JUNIPER_SERVICES 136
#endif

#ifndef DLT_JUNIPER_ATM1
#define DLT_JUNIPER_ATM1 137
#endif

#ifndef DLT_APPLE_IP_OVER_IEEE1394
#define DLT_APPLE_IP_OVER_IEEE1394 138
#endif

#ifndef DLT_MTP2_WITH_PHDR
#define DLT_MTP2_WITH_PHDR 139
#endif

#ifndef DLT_MTP2
#define DLT_MTP2 140
#endif

#ifndef DLT_MTP3
#define DLT_MTP3 141
#endif

#ifndef DLT_SCCP
#define DLT_SCCP 142
#endif

#ifndef DLT_DOCSIS
#define DLT_DOCSIS 143
#endif

#ifndef DLT_LINUX_IRDA
#define DLT_LINUX_IRDA 144
#endif

#ifndef DLT_IBM_SP
#define DLT_IBM_SP 145
#endif

#ifndef DLT_IBM_SN
#define DLT_IBM_SN 146
#endif

#ifndef DLT_USER0
#define DLT_USER0 147
#endif

#ifndef DLT_USER1
#define DLT_USER1 148
#endif

#ifndef DLT_USER2
#define DLT_USER2 149
#endif

#ifndef DLT_USER3
#define DLT_USER3 150
#endif

#ifndef DLT_USER4
#define DLT_USER4 151
#endif

#ifndef DLT_USER5
#define DLT_USER5 152
#endif

#ifndef DLT_USER6
#define DLT_USER6 153
#endif

#ifndef DLT_USER7
#define DLT_USER7 154
#endif

#ifndef DLT_USER8
#define DLT_USER8 155
#endif

#ifndef DLT_USER9
#define DLT_USER9 156
#endif

#ifndef DLT_USER10
#define DLT_USER10 157
#endif

#ifndef DLT_USER11
#define DLT_USER11 158
#endif

#ifndef DLT_USER12
#define DLT_USER12 159
#endif

#ifndef DLT_USER13
#define DLT_USER13 160
#endif

#ifndef DLT_USER14
#define DLT_USER14 161
#endif

#ifndef DLT_USER15
#define DLT_USER15 162
#endif

#ifndef DLT_IEEE802_11_RADIO_AVS
#define DLT_IEEE802_11_RADIO_AVS 163
#endif

#ifndef DLT_JUNIPER_MONITOR
#define DLT_JUNIPER_MONITOR 164
#endif

#ifndef DLT_BACNET_MS_TP
#define DLT_BACNET_MS_TP 165
#endif

#ifndef DLT_PPP_PPPD
#define DLT_PPP_PPPD 166
#endif

#ifndef DLT_JUNIPER_PPPOE
#define DLT_JUNIPER_PPPOE 167
#endif

#ifndef DLT_JUNIPER_PPPOE_ATM
#define DLT_JUNIPER_PPPOE_ATM 168
#endif

#ifndef DLT_GPRS_LLC
#define DLT_GPRS_LLC 169
#endif

#ifndef DLT_GPF_T
#define DLT_GPF_T 170
#endif

#ifndef DLT_GPF_F
#define DLT_GPF_F 171
#endif

#ifndef DLT_GCOM_T1E1
#define DLT_GCOM_T1E1 172
#endif

#ifndef DLT_GCOM_SERIAL
#define DLT_GCOM_SERIAL 173
#endif

#ifndef DLT_JUNIPER_PIC_PEER
#define DLT_JUNIPER_PIC_PEER 174
#endif

#ifndef DLT_ERF_ETH
#define DLT_ERF_ETH 175
#endif

#ifndef DLT_ERF_POS
#define DLT_ERF_POS 176
#endif

#ifndef DLT_LINUX_LAPD
#define DLT_LINUX_LAPD 177
#endif

#ifndef DLT_JUNIPER_ETHER
#define DLT_JUNIPER_ETHER 178
#endif

#ifndef DLT_JUNIPER_PPP
#define DLT_JUNIPER_PPP 179
#endif

#ifndef DLT_JUNIPER_FRELAY
#define DLT_JUNIPER_FRELAY 180
#endif

#ifndef DLT_JUNIPER_CHDLC
#define DLT_JUNIPER_CHDLC 181
#endif


