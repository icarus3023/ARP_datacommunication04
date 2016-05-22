// IPLayer.cpp: implementation of the CIPLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arp.h"
#include "IPLayer.h"
#include "NILayer.h"
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
	mp_UnderLayer->SetSourceAddress(m_sHeader.ip_src);
	mp_UnderLayer->SetDestinAddress(m_sHeader.ip_dst);
	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + IP_HEADER_SIZE);


	return bSuccess;
}

BOOL CIPLayer::Receive(unsigned char* ppayload)
{
	PIPLayer_HEADER pFrame = (PIPLayer_HEADER) ppayload ;
	
	BOOL bSuccess = FALSE ;
	STATIC_CACHE test;
	test.cache_ipaddr[0] = 192;
	test.cache_ipaddr[1] = 168;
	test.cache_ipaddr[2] = 0;
	test.cache_ipaddr[3] = 0;
	test.cache_netmaskaddr[0] = 255;
	test.cache_netmaskaddr[1] = 255;
	test.cache_netmaskaddr[2] = 255;
	test.cache_netmaskaddr[3] = 0;
	test.cache_flag = 0x01;
	memset(test.cache_gatewayaddr, 0, 4);
	test.static_interface = 0;
	static_table.Add(test);
	for (int i = 0; i < static_table.GetSize(); i++) {
		unsigned char netId[4];

		netId[0] = pFrame->ip_dst[0] & static_table[i].cache_netmaskaddr[0];
		netId[1] = pFrame->ip_dst[1] & static_table[i].cache_netmaskaddr[1];
		netId[2] = pFrame->ip_dst[2] & static_table[i].cache_netmaskaddr[2];
		netId[3] = pFrame->ip_dst[3] & static_table[i].cache_netmaskaddr[3];
		
		if (memcmp((unsigned char*)static_table[i].cache_ipaddr, (unsigned char*)(netId),4) == 0) { // static router의 네트워크 아이디가 송신측의 목적지주소를 서브넷팅한 네트워크 주소와 같으면
			SetDstIPAddress(pFrame->ip_dst);
			SetSrcIPAddress(pFrame->ip_src);
			m_sHeader.ip_proto = 0x01;
			m_sHeader.ip_cksum = pFrame->ip_cksum;
			m_sHeader.ip_id = pFrame->ip_id;
			m_sHeader.ip_len = pFrame->ip_len;
			m_sHeader.ip_verlen = 0x45;

			mp_aUpperLayer[0]->setType(static_table[i].static_interface);
			 
			return bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)&pFrame->ip_data);
		}
	}
	if(memcmp((char *)pFrame->ip_dst,(char *)m_sHeader.ip_src,4) ==0 &&
		memcmp((char *)pFrame->ip_src,(char *)m_sHeader.ip_src,4) !=0 && 
		memcmp((char *)pFrame->ip_src,(char *)m_sHeader.ip_dst,4) ==0 )
	{
		bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)pFrame->ip_data);
	}
	
	
	//들어오는패킷이 라우터 테이블에 있는지 검사. -> 해당 패킷이 gateway에있는지 up에있는지 (flag)를 통해 검사.
	//-> subnet한 네트워크 id가 up에 있으면 해당 네트워크id에 arp수행, gateway일때는 다른 router로 이동 다른라우터에 해당 네트워크id에 arp수행, reply가 오면 ping패킷으로 reply를 보냄.
	return bSuccess ;
}
