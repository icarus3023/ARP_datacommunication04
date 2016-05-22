// IPLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

// 경로 유효상태
#define UP_FLAG 0x01
// 경로방향 : 라우터
#define GATE_FLAG 0x02
// 경로방향 : 호스트
#define HOST_FLAG 0x04

// 라우터 테이블
typedef struct ROUTE_TABLE_ITEM {
	// 목적주소
	IP_ADDR des;
	// 마스크 범위
	IP_ADDR netmask;
	// 게이트웨이 주소
	IP_ADDR gateway;
	// flag : 경로의 상태 및 위치
	char flag;
	// 이더넷 카드 정보
	int ethcard;
	// 목적위치까지의 Hop 갯수
	int metric;
}ROUTE_TABLE_ITEM;



class CIPLayer 
: public CBaseLayer  
{
private:
	inline void		ResetHeader( );
	unsigned char srcip[4];
	unsigned char destip[4];
	ROUTE_TABLE_ITEM entry[MAX_ENTRY];
	int entrycount;
	void RouterTablesort();

public:
	CIPLayer( char* pName );
	virtual ~CIPLayer();
	void SetSrcIPAddress(unsigned char* src_ip);
	void SetDstIPAddress(unsigned char* dst_ip);
	void SetFragOff(unsigned short fragoff);
	ROUTE_TABLE_ITEM* getEntry(int &index);
	void delEntry(int &index);
	void addEntry(ROUTE_TABLE_ITEM &item);
	int getEntryCount();


	BOOL Send(unsigned char* ppayload, int nlength);
	BOOL Receive(unsigned char* ppayload);

	typedef struct _IPLayer_HEADER {
		unsigned short ip_verlen;	// ip version		(1byte)
		unsigned char ip_tos;		// type of service	(1byte)
		unsigned short ip_len;		// total packet length	(2byte)
		unsigned short ip_id;		// datagram id			(2byte)
		unsigned short ip_fragoff;	// fragment offset		(2byte)
		unsigned char ip_ttl;		// time to live in gateway hops (1byte)
		unsigned char ip_proto;		// IP protocol			(1byte)
		unsigned short ip_cksum;	// header checksum		(2byte)
		unsigned char ip_src[4];	// IP address of source	(4byte)
		unsigned char ip_dst[4];	// IP address of destination	(4byte)
		unsigned char ip_data[IP_DATA_SIZE]; // variable length data
		
	} IPLayer_HEADER, *PIPLayer_HEADER ;
	typedef struct _CACHE_ENTRY {
		unsigned char				cache_ipaddr[4];
		unsigned char				cache_netmaskaddr[4];
		unsigned char				cache_gatewayaddr[4];
		unsigned short				cache_flag;
		int							static_interface;
	} STATIC_CACHE;


	CArray<STATIC_CACHE> static_table;
	typedef struct _ICMPLAYER_HEADER {
		unsigned char icmp_type;	// icmp version		(1byte)
		unsigned char icmp_code;		// type of service	(1byte)
		unsigned short icmp_chksum;		// total packet length	(2byte)
		unsigned short icmp_id;		// datagram id			(2byte)
		unsigned short icmp_seqnum;	// fragment offset		(2byte)
		unsigned char ip_data[32]; // variable length data

	} ICMPLAYER_HEADER, *pICMPLAYER_HEADER;
	bool InsertStaticTable(unsigned char *cache_ipaddr, unsigned char *cache_netmaskaddr, unsigned char *cache_gatewayaddr, unsigned short cache_flag, int static_interface);
protected:
	IPLayer_HEADER	m_sHeader ;
};