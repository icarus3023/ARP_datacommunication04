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


typedef struct _IP_ADDR
{
	union {
		struct { UCHAR s_b1, s_b2, s_b3, s_b4; } S_un_b;
		struct { unsigned short s_w1, s_w2; } S_un_w;
		unsigned long S_addr;
		UCHAR s_ip_addr[4];
	}S_un;

} IP_ADDR, *LPIP_ADDR;

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
		unsigned char ip_verlen;	// ip version		(1byte)
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
		IP_ADDR				cache_ipaddr;
		IP_ADDR				cache_netmaskaddr;
		IP_ADDR				cache_gatewayaddr;
		unsigned short		cache_flag;
	} STATIC_CACHE;


	CArray<STATIC_CACHE> static_table;
protected:
	IPLayer_HEADER	m_sHeader ;
};