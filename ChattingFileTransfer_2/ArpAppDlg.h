// ArpAppDlg.h : header file
//houlsssxasd

#if !defined(AFX_ArpAppDlg_H__FE9C37CB_4C90_4772_99AD_50A9B24BCE62__INCLUDED_)
#define AFX_ArpAppDlg_H__FE9C37CB_4C90_4772_99AD_50A9B24BCE62__INCLUDED_

#include "LayerManager.h"	// Added by ClassView

#include "EthernetLayer.h"	// Added by ClassView
#include "IPLayer.h"		// Added by ClassView
#include "NILayer.h"		// Added by ClassView
#include "ArpLayer.h"
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"
#include "SubDlg.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CArpAppDlg dialog

class CArpAppDlg 
: public CDialog, 
  public CBaseLayer
{
// Construction
public:
	CArpAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CArpAppDlg)
	enum { IDD = IDD_IPC_DIALOG };
	CListBox		m_ListArpTable;		// Arp Table View
//	CIPAddressCtrl	m_unDstIPAddr;
// Destination IP Address
	
	int		interfaceNum;
	void	setType(int interfaceNum);
	//}}AFX_DATA

	// 파일 send 버튼 Control
	afx_msg void OnOffFileButton(BOOL bBool);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArpAppDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CArpAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnSendMessage();
	afx_msg void OnButtonAddrSet();
	
	afx_msg void OnTimer(UINT nIDEvent);
	bool isArpTableNotEmptyAndComplite(const BOOL &isComplite);
	bool isTimeOut(const BOOL &isComplite, int ArpTTL);
	afx_msg void OnAddFile();
	
	afx_msg	void OnComboEnetAddr();

	

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL			Receive( unsigned char* ppayload );
	inline void		SendData( );
	void			SetIpAddress(IP_ADDR * ip_cache, unsigned char * ip);
private:
	CLayerManager	m_LayerMgr;

	enum {			IPC_INITIALIZING, 
					IPC_READYTOSEND, 
					IPC_WAITFORACK,
					IPC_ERROR,
					IPC_BROADCASTMODE,
					IPC_UNICASTMODE,
					IPC_ADDR_SET,
					IPC_ADDR_RESET,
					CFT_COMBO_SET } ;

	void			SetDlgState( int state );
	inline void		EndofProcess( );
	inline void		SetRegstryMessage( );

	BOOL			m_bSendReady;

// Object App
	
	CIPLayer*		m_IP;
	CEthernetLayer*	m_ETH;
	CNILayer*		m_NI;
	CArpLayer*		m_ARP;

// Implementation
	UINT			m_wParam;
	DWORD			m_lParam;
	CString			m_unSrcEnetAddr;

// OnTimer
	unsigned char	src_ip[4];
	unsigned char	dst_ip[4];
	unsigned char	src_mac[12];
	unsigned char	dst_mac[12];
	unsigned char	graOidData[12];
	char			errbuf[PCAP_ERRBUF_SIZE];
	UCHAR			desaddress[6];

	BOOL			type;
	DWORD			resolveAddr;
	unsigned char	cache_type;
	unsigned short	cache_ttl;

	char*			macStr;
	char*			IpAddress;
	unsigned char*	RawIPAddr;

	pcap_if_t*		device;
	pcap_addr_t*	a;
	PPACKET_OID_DATA OidData;

	IP_ADDR			src_ip_cache;
	IP_ADDR			dst_ip_cache;

	ETHERNET_ADDR	dst_ethernet;

	LPADAPTER		adapter;

	CString			msg;
	CString			ipmsg;
	CString			macmsg;
	CString			sIP;
	
	
public:
	CListBox proxyTableList;	
//	CComboBox m_ComboEnetName;

	CListBox CProxyList;
	CListCtrl srt_CList;

	afx_msg void OnBnClickedItemdelete();
	afx_msg void OnBnClickedAlldelete();
	afx_msg void OnBnClickedProxytableadd();


	DWORD CArpAppDlg::ResolveAddr(CString& sIP, int nIndex);
	CArpLayer* GetArpLayer();
	CIPLayer* GetIPLayer();
	CNILayer* GetNILayer();
	afx_msg void OnClickedProxytabledelete();
	afx_msg void OnBnClickedGratitudearpsend();
	
	void SetDstEthernetAddress();
//	CEdit CGratitudeDeviceAddress;
	CString CGratitousDevAddr;
//	CEdit CGratitousContol;

	
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ArpAppDlg_H__FE9C37CB_4C90_4772_99AD_50A9B24BCE62__INCLUDED_)
