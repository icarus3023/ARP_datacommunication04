#if !defined(AFX_ARPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_)
#define AFX_ARPLAYER_H__7857C9C2_B459_4DC8_B9B3_4E6C8B587B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseLayer.h"
#include <pcap.h>
#include <WinSock2.h>

#define	ARP_MAX_LENGTH 28

class CArpLayer
	: public CBaseLayer
{
private:
	inline void		ResetHeader();
	void UpdateTable();	// table을 update
	static unsigned int timer(LPVOID pParam);	//timer 지정

public:

	CArpLayer(char* pName); // ArpLayer 선언
	virtual ~CArpLayer(void);

	unsigned char	*m_ppayload;


	void	All_Delete();
	void	Item_Delete();

	BOOL	Send(unsigned char* ppayload, int nlength = 0);		//ARP Request,Reply
	BOOL	Receive(unsigned char* ppayload);	//ARP Packet이 도착하여 Table을 확인.	

	

	

	

	void	ARP_Request_SendPacket(unsigned char* pAddress); //송신지 ip주소를 보냄.
	void	ARP_Reply_SendPacket(unsigned char *pAddress); //목적지 ip주소를 보냄.

														   //추가
	void SetSourceAddress(unsigned char* pAddress);
	void SetDestinAddress(unsigned char* pAddress);


	BOOL	FindHost(unsigned char* ipAddress);	//Send,Receive할 때 사용함.


	typedef struct _ARP_HEADER {
		//send에 정의하기 위해 주석처리 했습니다.
		unsigned short	hard_type; //= 0x0001;				// 하드웨어 주소타입(ethernetlayer일 때,1) 2바이트
		unsigned short	prot_type; //= 0x0800;				// 프로토콜 타입(IPlayer일 때,0x0800) 2바이트
		unsigned char	hard_size; //= 0x06;				// 1바이트
		unsigned char	prot_size; //= 0x04;				// 1바이트

		unsigned short	op;								//op도 send에 정의해주겠습니다.

														//이렇게 주소 정의를 하는 것이 좋을 것 같습니다. BaseLayer헤더에 ETHERNET_ADDR와IP_ADDR 구조체로 정의했습니다.
		ETHERNET_ADDR	arp_src_macaddr;	//송신지 mac 주소
		IP_ADDR			arp_src_ipaddr;		//송신지 ip 주소
		ETHERNET_ADDR	arp_dst_macaddr;	//목적지 mac 주소
		IP_ADDR			arp_dst_ipaddr;		//목적지 ip 주소

											//unsigned char	src_ether_addr[6];		// src ethernet 주소
											//unsigned char	src_ip_addr[4];			// src ip 주소 long???
											//unsigned char	dest_ether_addr[6];		// dest ethernet 주소(보낼때는 모름)
											//unsigned char	dest_ip_addr[4];		// dest ip 주소(사용자가 입력한 주소 사용)
		unsigned char arp_data[ETHER_MAX_SIZE - (IP_DATA_SIZE + IP_HEADER_SIZE + ETHER_HEADER_SIZE)];

	} ARP_HEADER, *PARP_HEADER;

	//추가
	typedef struct _CACHE_ENTRY {
		IP_ADDR				cache_ipaddr;
		ETHERNET_ADDR		cache_enetaddr;
		BOOL				cache_type;
		unsigned short		cache_ttl;
	} CACHE_ENTRY;
	CArray<CACHE_ENTRY> table;
	typedef struct _PROXY_ENTRY {
		IP_ADDR				cache_ipaddr;
		ETHERNET_ADDR		cache_enetaddr;
	} PROXY_ENTRY;
	// 프록시 
	CArray<PROXY_ENTRY> proxyTable;

	void setSrcHd(unsigned char* pAddress);
	BOOL change;
	BOOL InsertTable(IP_ADDR cache_ipaddr, ETHERNET_ADDR	cache_enetaddr, BOOL cache_type = false);

	// 프록시 
	BOOL InsertProxyTable(IP_ADDR proxy_ipaddr, ETHERNET_ADDR proxyDeviceAddress);

	unsigned char* makeReplyPacket(unsigned char* ppayload /*prame*/, ETHERNET_ADDR* mac_addr, IP_ADDR* ip_addr);
	BOOL isInTableEntry(unsigned char* pAddress);
	//protected로 바꾸어 주었습니다.test
	bool isPacketGARP(const PARP_HEADER pFrame);
	bool isReplyPacketARPorGARP(const PARP_HEADER pFrame);
	bool isProxyTableEntryAndPacketRequest(const PARP_HEADER pFrame, int i);
	bool isProxyTableNotEmptyAndNotGarp(const PARP_HEADER pFrame);
	bool isReceivePacketMine(const PARP_HEADER pFrame);
	bool SendUnderLayerOp(PARP_HEADER pFrame, unsigned short op, int nlength);
protected:
	ARP_HEADER m_sHeader;
};

enum {
	ARP_REQUEST = 0x0001,
	ARP_REPLY = 0x0002
};
#endif
//CArpTable arpTable;

