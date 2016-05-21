// IPLayer.cpp: implementation of the CIPLayer class.
//
//////////////////////////////////////////////////////////////////////

#include <WinSock2.h>
#include "stdafx.h"
#include "arp.h"
#include "IPLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIPLayer::CIPLayer( char* pName )
: CBaseLayer( pName )
{
	entrycount = 0;
	ResetHeader( );
}

CIPLayer::~CIPLayer()
{
}

void CIPLayer::ResetHeader()
{
	m_sHeader.ip_verlen = 0x00;
	m_sHeader.ip_tos = 0x00;
	m_sHeader.ip_len = 0x0000;
	m_sHeader.ip_id = 0x0000;
	m_sHeader.ip_fragoff = 0x0000;
	m_sHeader.ip_ttl = 0x00;
	m_sHeader.ip_proto = 0x00;
	m_sHeader.ip_cksum = 0x00;
	memset( m_sHeader.ip_src, 0, 4);
	memset( m_sHeader.ip_dst, 0, 4);
	memset( m_sHeader.ip_data, 0, IP_DATA_SIZE);
}

void CIPLayer::SetSrcIPAddress(unsigned char* src_ip)
{
	memcpy( m_sHeader.ip_src, src_ip, 4);
}

void CIPLayer::SetDstIPAddress(unsigned char* dst_ip)
{
	memcpy( m_sHeader.ip_dst, dst_ip, 4);
}

void CIPLayer::SetFragOff(unsigned short fragoff)
{
	m_sHeader.ip_fragoff = fragoff;
}

BOOL CIPLayer::Send(unsigned char* ppayload, int nlength)
{
	memcpy( m_sHeader.ip_data, ppayload, nlength ) ;
	
	BOOL bSuccess = FALSE ;
	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader,nlength+IP_HEADER_SIZE);

	return bSuccess;
}

BOOL CIPLayer::Receive(unsigned char* ppayload)
{
	PIPLayer_HEADER pFrame = (PIPLayer_HEADER) ppayload ;
	
	BOOL bSuccess = FALSE ;

	if(memcmp((char *)pFrame->ip_dst,(char *)m_sHeader.ip_src,4) ==0 &&
		memcmp((char *)pFrame->ip_src,(char *)m_sHeader.ip_src,4) !=0 && 
		memcmp((char *)pFrame->ip_src,(char *)m_sHeader.ip_dst,4) ==0 )
	{
		bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)pFrame->ip_data);
	}
	return bSuccess ;
}

// 라우터 테이블 아이템 추가
void CIPLayer::addEntry(ROUTE_TABLE_ITEM &item) {
	entry[entrycount++] = item;
	RouterTablesort();
}


// 라우터 테이블 아이템 번호 반환
int CIPLayer::getEntryCount() {
	return entrycount;
}

// 아이템 삭제
void CIPLayer::delEntry(int &index) {
	int i;
	for (i = index; i < entrycount; i++) {
		entry[i] = entry[i + 1];
	}
	entrycount--;
}

// 아이템 주소값 반환
ROUTE_TABLE_ITEM* CIPLayer::getEntry(int &index) {
	return &entry[index];
}


// 라우터 테이블의 아이템 정렬(=내림차순)
void CIPLayer::RouterTablesort() {
	int i;
	bool flag = true;
	ROUTE_TABLE_ITEM temp;

	while (flag) {
		flag = false;
		for (i = 0; i < entrycount - 1; i++) {
			unsigned long net1, net2;
			memcpy(&net1, entry[i].netmask.s_ip_addr, 4);
			memcpy(&net2, entry[i + 1].netmask.s_ip_addr, 4);
			// 내림차순 정렬
			if (net1 < net2)
			{
				temp = entry[i];
				entry[i] = entry[i + 1];
				entry[i + 1] = temp;
				flag = true;
			}
		}
	}
}