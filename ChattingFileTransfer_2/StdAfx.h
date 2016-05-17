// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_)
#define AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_

//추가
#define ARP_TYPE_REQUIRE		0x0001 //request보낼때는 0x0001
#define ARP_TYPE_RESPONE		0x0002 //reply보낼때는 0x0002
#define ENET_TYPE_ARP		0x0806
//
//추가
typedef struct _ETHERNET_ADDR
{
	union {
		struct {
			unsigned char e0, e1, e2, e3, e4, e5;
		} s_un_byte;
		unsigned char s_ether_addr[6];
	} S_un;

#define e_addr		S_un.s_ether_addr
#define e_addr0		S_un.s_un_byte.e0
#define e_addr1		S_un.s_un_byte.e1
#define e_addr2		S_un.s_un_byte.e2
#define e_addr3		S_un.s_un_byte.e3
#define e_addr4		S_un.s_un_byte.e4
#define e_addr5		S_un.s_un_byte.e5

} ETHERNET_ADDR;


typedef struct _IP_ADDR
{
	union {
		struct {
			unsigned char i0, i1, i2, i3;
		} s_un_byte;
		unsigned char s_ip_addr[4];
	} S_un;

#define i_addr		S_un.s_ip_addr
#define i_addr0		S_un.s_un_byte.i0
#define i_addr1		S_un.s_un_byte.i1
#define i_addr2		S_un.s_un_byte.i2
#define i_addr3		S_un.s_un_byte.i3
} IP_ADDR;
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <afxcontrolbars.h>
using namespace std;

//{{AFX_INSERT_LOCATION}}

#define MAX_LAYER_NUMBER		0xff

#define ETHER_MAX_SIZE			1514
#define ETHER_HEADER_SIZE		14
#define ETHER_MAX_DATA_SIZE		( ETHER_MAX_SIZE - ETHER_HEADER_SIZE )
#define ETHER_PROTO_TYPE_IP		0x0800
#define ETHER_PROTO_TYPE_ARP	0x0806
#define ETHER_PROTO_TYPE_RARP	0x0835

#define TCP_HEADER_SIZE			( sizeof(unsigned int) * 2 +				\
								  sizeof(unsigned short) * 5 +					\
								  sizeof(unsigned char) * 6 )
#define TCP_DATA_SIZE			( ETHER_MAX_SIZE - ( TCP_HEADER_SIZE +  IP_HEADER_SIZE + \
																		ETHER_HEADER_SIZE ) )

#define IP_HEADER_SIZE			( sizeof(unsigned short) * 4 +					\
								  sizeof(unsigned char) * 12  )
#define IP_DATA_SIZE			( ETHER_MAX_SIZE - ( IP_HEADER_SIZE  + \
														ETHER_HEADER_SIZE ))

#define APP_HEADER_SIZE			( sizeof(unsigned short) +					\
								  sizeof(unsigned char)  +				\
								  sizeof(unsigned long) )
#define APP_DATA_SIZE 			( ETHER_MAX_DATA_SIZE - ( APP_HEADER_SIZE +		\
												          TCP_HEADER_SIZE +		\
												          IP_HEADER_SIZE ) )

#define FILE_HEADER_SIZE		( sizeof(unsigned long) * 2 +				\
								  sizeof(unsigned short) +					\
								  sizeof(unsigned char) * 3 )
#define FILE_DATA_SIZE			( ETHER_MAX_DATA_SIZE - ( FILE_HEADER_SIZE +		\
												          TCP_HEADER_SIZE +			\
												          IP_HEADER_SIZE ) )
#define FILE_READ_SIZE	1400

#define TCP_PORT_CHAT	0x2080
#define TCP_PORT_FILE	0x2090

#define NI_COUNT_NIC	10

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_)
