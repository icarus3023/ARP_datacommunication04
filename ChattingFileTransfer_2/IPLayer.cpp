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
	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader,nlength+IP_HEADER_SIZE);
	mp_UnderLayer->SetSourceAddress(m_sHeader.ip_src);
	mp_UnderLayer->SetDestinAddress(m_sHeader.ip_dst);
	bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + IP_HEADER_SIZE);
	return bSuccess;
}

BOOL CIPLayer::Receive(unsigned char* ppayload)
{
	PIPLayer_HEADER pFrame = (PIPLayer_HEADER) ppayload ;
	
	BOOL bSuccess = FALSE;

	for (int i = 0; i < static_table.GetSize(); i++) {
		unsigned char netIp[4];
		netIp[0] = pFrame->ip_dst[0] & static_table[i].cache_netmaskaddr[0];
		netIp[1] = pFrame->ip_dst[1] & static_table[i].cache_netmaskaddr[1];
		netIp[2] = pFrame->ip_dst[2] & static_table[i].cache_netmaskaddr[2];
		netIp[3] = pFrame->ip_dst[3] & static_table[i].cache_netmaskaddr[3];

		if (memcmp((unsigned char*)static_table[i].cache_ipaddr, (unsigned char*)(netIp),4) == 0) { // static router의 네트워크 아이디가 송신측의 목적지주소를 서브넷팅한 네트워크 주소와 같으면

			if (mp_aUpperLayer[1]->Receive((unsigned char*)static_table[i].static_interface)) {  // mp_aUpperLayer[1] is CArpAppDlg Layer
				SetDstIPAddress(pFrame->ip_dst);
				SetSrcIPAddress(pFrame->ip_src);
				m_sHeader.ip_proto = 0x01;
				m_sHeader.ip_cksum = pFrame->ip_cksum;
				m_sHeader.ip_id = pFrame->ip_id;
				m_sHeader.ip_len = pFrame->ip_len;
				m_sHeader.ip_verlen = 0x45;
				mp_aUpperLayer[0]->setType(static_table[i].static_interface);
				bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)&pFrame->ip_data);
				return bSuccess;
			}
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

BOOL CIPLayer::InsertRouteTable(unsigned char* destAddr, unsigned char* netAddr, unsigned char* gateAddr, int checkFlag, int static_interface)
{
	STATIC_CACHE routeTable;
	routeTable.cache_ipaddr[0] = destAddr[0];
	routeTable.cache_ipaddr[1] = destAddr[1];
	routeTable.cache_ipaddr[2] = destAddr[2];
	routeTable.cache_ipaddr[3] = destAddr[3];
	

	routeTable.cache_netmaskaddr[0] = netAddr[0];
	routeTable.cache_netmaskaddr[1] = netAddr[1];
	routeTable.cache_netmaskaddr[2] = netAddr[2];
	routeTable.cache_netmaskaddr[3] = netAddr[3];

	routeTable.cache_gatewayaddr[0] = gateAddr[0];
	routeTable.cache_gatewayaddr[1] = gateAddr[1];
	routeTable.cache_gatewayaddr[2] = gateAddr[2];
	routeTable.cache_gatewayaddr[3] = gateAddr[3];

	routeTable.cache_flag = checkFlag;
	routeTable.static_interface = static_interface;

	static_table.Add(routeTable);
	return true;
}

