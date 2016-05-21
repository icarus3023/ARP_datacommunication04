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

	return bSuccess;
}

BOOL CIPLayer::Receive(unsigned char* ppayload)
{
	PIPLayer_HEADER pFrame = (PIPLayer_HEADER) ppayload ;
	
	BOOL bSuccess = FALSE ;

	for (int i = 0; i < static_table.GetSize(); i++) {
		if (memcmp((char*)static_table[i].cache_ipaddr, (char*)((short)pFrame->ip_dst & (short)static_table[i].cache_netmaskaddr),4) == 0) { // static router�� ��Ʈ��ũ ���̵� �۽����� �������ּҸ� ��������� ��Ʈ��ũ �ּҿ� ������
			if (static_table[i].cache_flag==0) this->SetDstIPAddress(pFrame->ip_dst);			
			else if (static_table[i].cache_flag == 1) this->SetDstIPAddress(static_table[i].cache_gatewayaddr);	
			if (mp_aUpperLayer[1]->Receive((unsigned char*)static_table[i].static_interface)) {  // mp_aUpperLayer[1] is CArpAppDlg Layer
				bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, IP_HEADER_SIZE);	//UnderLayer is Ethernet Layer
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
	
	
	//��������Ŷ�� ����� ���̺� �ִ��� �˻�. -> �ش� ��Ŷ�� gateway���ִ��� up���ִ��� (flag)�� ���� �˻�.
	//-> subnet�� ��Ʈ��ũ id�� up�� ������ �ش� ��Ʈ��ũid�� arp����, gateway�϶��� �ٸ� router�� �̵� �ٸ�����Ϳ� �ش� ��Ʈ��ũid�� arp����, reply�� ���� ping��Ŷ���� reply�� ����.
	return bSuccess ;
}
