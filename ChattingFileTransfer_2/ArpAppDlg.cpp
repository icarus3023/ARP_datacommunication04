#include "stdafx.h"
#include "arp.h"
#include "ArpAppDlg.h"
#include "ProxyDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArpAppDlg dialog

CArpAppDlg::CArpAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArpAppDlg::IDD, pParent),
	CBaseLayer("ChatDlg"),
	m_bSendReady(FALSE)
	, CGratitousDevAddr(_T(""))
{
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_LayerMgr.AddLayer(this);
	m_LayerMgr.AddLayer(new CNILayer("NI"));
	m_LayerMgr.AddLayer(new CEthernetLayer("Ethernet"));
	m_LayerMgr.AddLayer(new CArpLayer("ARP"));
	m_LayerMgr.AddLayer(new CIPLayer("IP"));

	//////////////////////// fill the blank ///////////////////////////////
	m_LayerMgr.ConnectLayers("NI ( *Ethernet ( *IP *ARP ( *IP ( *ChatDlg ) ) )");
	///////////////////////////////////////////////////////////////////////

	m_IP = (CIPLayer*)mp_UnderLayer;
	m_ARP = (CArpLayer *)m_LayerMgr.GetLayer("ARP");
	m_ETH = (CEthernetLayer *)m_LayerMgr.GetLayer("Ethernet");
	m_NI = (CNILayer *)m_LayerMgr.GetLayer("NI");
}

void CArpAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHAT, m_ListArpTable);
	DDX_Control(pDX, IDC_EDIT_DSTIP, m_unDstIPAddr);
	DDX_Control(pDX, IDC_COMBO_ENETADDR, m_ComboEnetName);
	DDX_Control(pDX, IDC_LIST1, CProxyList);
	DDX_Text(pDX, IDC_GRATITUDEARPEDIT, CGratitousDevAddr);
	DDV_MaxChars(pDX, CGratitousDevAddr, 12);
	DDX_Control(pDX, IDC_GRATITUDEARPEDIT, CGratitousContol);
}

BEGIN_MESSAGE_MAP(CArpAppDlg, CDialog)

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnSendMessage)
	ON_BN_CLICKED(IDC_BUTTON_ADDR, OnButtonAddrSet)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ITEMDELETE, &CArpAppDlg::OnBnClickedItemdelete)
	ON_BN_CLICKED(IDC_ALLDELETE, &CArpAppDlg::OnBnClickedAlldelete)
	ON_BN_CLICKED(IDC_PROXYTABLEADD, &CArpAppDlg::OnBnClickedProxytableadd)
	ON_BN_CLICKED(IDC_PROXYTABLEDELETE, &CArpAppDlg::OnClickedProxytabledelete)
	ON_BN_CLICKED(IDC_GRATITUDEARPSEND, &CArpAppDlg::OnBnClickedGratitudearpsend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArpAppDlg message handlers

BOOL CArpAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetRegstryMessage();
	SetDlgState(IPC_INITIALIZING);
	SetDlgState(CFT_COMBO_SET);

	return TRUE;
}
void CArpAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		if (MessageBox("Are you sure ?",
			"Question",
			MB_YESNO | MB_ICONQUESTION)
			== IDNO)
			return;
		else EndofProcess();
	}

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CArpAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CArpAppDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CArpAppDlg::OnButtonAddrSet()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_bSendReady) {
		SetDlgState(IPC_ADDR_RESET);
		SetDlgState(IPC_INITIALIZING);
	}
	else {
		device = m_NI->GetAdapterObject(m_ComboEnetName.GetCurSel());
		adapter = PacketOpenAdapter(device->name);

		OidData = (PPACKET_OID_DATA)malloc(sizeof(PACKET_OID_DATA));
		OidData->Oid = 0x01010101;
		OidData->Length = 6;
		PacketRequest(adapter, FALSE, OidData);

		for (pcap_addr_t *a = device->addresses; a != NULL; a = a->next) {
			if (a->addr->sa_family == AF_INET) {
				IpAddress = inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr);
			}
		}

		sIP = IpAddress;
		sIP.Replace(_T('.'), NULL);
		resolveAddr = ResolveAddr(sIP, 4);
		for (int i = 0; i < 4; i++)
		{
			src_ip[3 - i] = resolveAddr >> (i * 8);
		}

		m_IP->SetSrcIPAddress(src_ip);
		m_ARP->SetSourceAddress(src_ip);
		m_ARP->setSrcHd((unsigned char*)OidData->Data);
		m_ETH->SetEnetSrcAddress(OidData->Data);
		m_NI->SetAdapterNumber(m_ComboEnetName.GetCurSel());

		m_NI->PacketStartDriver();

		SetDlgState(IPC_ADDR_SET);
		SetDlgState(IPC_READYTOSEND);
	}
	m_bSendReady = !m_bSendReady;
	SetTimer(1, 1000, NULL);
}

DWORD CArpAppDlg::ResolveAddr(CString& sIP, int nIndex)
{
	DWORD *dwTemp = (DWORD*)malloc(sizeof(DWORD)*nIndex);
	LPCTSTR lp = LPSTR(LPCTSTR(sIP));
	DWORD dwIP = 0;
	for (int i = 0; i < nIndex; i++)
	{
		dwTemp[i] = _ttol(lp);
		lp += ((_tcslen(lp) + 1) * sizeof(TCHAR));
		dwIP = dwIP | dwTemp[i] << ((nIndex - 1) - i) * 8;
	}
	return dwIP;
}

void CArpAppDlg::SetRegstryMessage()
{
}

void CArpAppDlg::OnSendMessage()
{
	UpdateData(TRUE);

	if (!m_unDstIPAddr.IsBlank())
	{
		SendData();
		(CEdit*)GetDlgItem(IDC_EDIT_DSTIP)->SetFocus();
	}
	UpdateData(FALSE);
}
void CArpAppDlg::SendData()
{
	m_unDstIPAddr.GetAddress(dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3]);
	m_IP->SetDstIPAddress(dst_ip);
	m_ARP->SetDestinAddress(dst_ip);

	desaddress[0] = 0xff, desaddress[1] = 0xff, desaddress[2] = 0xff, desaddress[3] = 0xff,
		desaddress[5] = 0xff, desaddress[4] = 0xff;
	dst_ip_cache.S_un.s_un_byte.i0 = dst_ip[0];
	dst_ip_cache.S_un.s_un_byte.i1 = dst_ip[1];
	dst_ip_cache.S_un.s_un_byte.i2 = dst_ip[2];
	dst_ip_cache.S_un.s_un_byte.i3 = dst_ip[3];

	dst_ethernet.S_un.s_un_byte.e0 = 0xff;
	dst_ethernet.S_un.s_un_byte.e1 = 0xff;
	dst_ethernet.S_un.s_un_byte.e2 = 0xff;
	dst_ethernet.S_un.s_un_byte.e3 = 0xff;
	dst_ethernet.S_un.s_un_byte.e4 = 0xff;
	dst_ethernet.S_un.s_un_byte.e5 = 0xff;

	m_ETH->SetEnetDstAddress(desaddress);

	if (!m_ARP->isSameTable(dst_ip))
	{
		m_ARP->InsertTable(dst_ip_cache, dst_ethernet);
		msg.Format(_T("%d.%d.%d.%d\t???????\tInComplete"), dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3]);
		m_ListArpTable.AddString(msg);
	}

	mp_UnderLayer->Send(NULL, 0);
}

BOOL CArpAppDlg::Receive(unsigned char *ppayload)
{
	int len_ppayload = strlen((char *)ppayload);
	return TRUE;
}

BOOL CArpAppDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (::GetDlgCtrlID(::GetFocus()) == IDC_EDIT_DSTIP)
				OnSendMessage();					return FALSE;
		case VK_ESCAPE: return FALSE;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CArpAppDlg::SetDlgState(int state) // 다이얼로그 초기화 부분
{
	UpdateData(TRUE);
	int i;
	CString device_description;

	CButton*	pChkButton = (CButton*)GetDlgItem(IDC_CHECK_TOALL);
	CButton*	pSendButton = (CButton*)GetDlgItem(IDC_BUTTON_SEND);
	CButton*	pSetAddrButton = (CButton*)GetDlgItem(IDC_BUTTON_ADDR);
	CButton*	pFileSearchButton = (CButton*)GetDlgItem(IDC_BUTTON_FILE);
	CButton*	pFileSendButton = (CButton*)GetDlgItem(IDC_BUTTON_FILESEND);

	CEdit*		pDstIPEdit = (CEdit*)GetDlgItem(IDC_EDIT_DSTIP);
	CEdit*		pFilePathEdit = (CEdit*)GetDlgItem(IDC_EDIT_FilePath);

	CComboBox*	pEnetNameCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ENETADDR);

	switch (state)
	{
	case IPC_INITIALIZING: // 첫 화면 세팅
		pSendButton->EnableWindow(FALSE);
		m_ListArpTable.EnableWindow(TRUE);
		break;
	case IPC_READYTOSEND: // Send(S)버튼을 눌렀을 때 세팅
		break;
	case IPC_WAITFORACK:	break;
	case IPC_ERROR:		break;
	case IPC_ADDR_SET:	// 설정(&O)버튼을 눌렀을 때
		pSendButton->EnableWindow(TRUE);
		pEnetNameCombo->EnableWindow(FALSE);
		break;
	case IPC_ADDR_RESET: // 재설정(&R)버튼을 눌렀을 때
		pSetAddrButton->SetWindowText("설정(&O)");
		pChkButton->EnableWindow(TRUE);
		pDstIPEdit->EnableWindow(TRUE);
		pEnetNameCombo->EnableWindow(TRUE);
		m_NI->m_thrdSwitch = FALSE;
		break;
	case CFT_COMBO_SET:
		for (i = 0; i < NI_COUNT_NIC; i++) {
			if (!m_NI->GetAdapterObject(i))
				break;
			device_description = m_NI->GetAdapterObject(i)->description;
			device_description.Trim();
			pEnetNameCombo->AddString(device_description);
			pEnetNameCombo->SetCurSel(0);
		}
		break;
	}

	UpdateData(FALSE);
}

void CArpAppDlg::EndofProcess()
{
	m_LayerMgr.DeAllocLayer();
}

void CArpAppDlg::OnTimer(UINT nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	m_ListArpTable.UpdateData(true);

	//timer가 실행될 때마다(1초) table을 검색하여 cache.type==true인 entry를 확인 후 List Table에 있다면 기존 문자열을 변경, 없다면 추가함.
	m_ListArpTable.ResetContent();
	for (int i = 0; i < m_ARP->table.GetSize(); i++)
	{
		BOOL isComplite = m_ARP->table.GetAt(i).cache_type;
		int	ArpTTL = m_ARP->table.GetAt(i).cache_ttl;
		m_ARP->table.GetAt(i).cache_ttl++;
		if (isTimeOut(isComplite, ArpTTL)) {
			m_ARP->table.RemoveAt(i); 
			i--;
			continue; 
		}
		RawIPAddr = m_ARP->table.GetAt(i).cache_ipaddr.S_un.s_ip_addr;
		type = m_ARP->table.GetAt(i).cache_type;
		ipmsg.Format(_T("%d.%d.%d.%d"), RawIPAddr[0], RawIPAddr[1], RawIPAddr[2], RawIPAddr[3]);

		if (isArpTableNotEmptyAndComplite(isComplite))
		{
			unsigned char* RawEthernetAddr = m_ARP->table.GetAt(i).cache_enetaddr.S_un.s_ether_addr;
			macmsg.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"), RawEthernetAddr[0], RawEthernetAddr[1], RawEthernetAddr[2],
				RawEthernetAddr[3], RawEthernetAddr[4], RawEthernetAddr[5]);
			msg.Format(_T("%s\t%s\tComplete"), ipmsg, macmsg);
		}
		else { msg.Format(_T("%s\t???????\tincomplete"), ipmsg); }
		m_ListArpTable.AddString(msg);
	}
	m_ListArpTable.UpdateData(false);
}

bool CArpAppDlg::isArpTableNotEmptyAndComplite(const BOOL &isComplite)
{
	return !m_ARP->table.IsEmpty() && isComplite == 1;
}

bool CArpAppDlg::isTimeOut(const BOOL &isComplite, int ArpTTL)
{
	return (isComplite == true && ArpTTL >= 12000)
		|| (isComplite == false && ArpTTL >= 1800);
}

void CArpAppDlg::OnBnClickedItemdelete()
{
	int nIndex = m_ListArpTable.GetCurSel();
	m_ListArpTable.DeleteString(nIndex);
	m_ARP->table.RemoveAt(nIndex);
}

void CArpAppDlg::OnBnClickedAlldelete()
{
	m_ListArpTable.ResetContent();
	m_ARP->table.RemoveAll();
	m_ListArpTable.ResetContent();
}

void CArpAppDlg::OnBnClickedProxytableadd()
{
	ProxyDlg proxyadd;
	proxyadd.DoModal();
}

CArpLayer* CArpAppDlg::GetArpLayer() {
	return m_ARP;
}

void CArpAppDlg::OnClickedProxytabledelete()
{
	int nIndex = CProxyList.GetCurSel();
	CProxyList.DeleteString(nIndex);
	m_ARP->proxyTable.RemoveAt(nIndex);
}

void CArpAppDlg::OnBnClickedGratitudearpsend()
{
	//CGratitudeDeviceAddress

	device = m_NI->GetAdapterObject(m_ComboEnetName.GetCurSel());
	adapter = PacketOpenAdapter(device->name);
	a = device->addresses;

	for (pcap_addr_t *a = device->addresses; a != NULL; a = a->next) {
		if (a->addr->sa_family == AF_INET) {
			IpAddress = inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr);
		}
	}

	sIP = IpAddress;
	sIP.Replace(_T('.'), NULL);

	resolveAddr = ResolveAddr(sIP, 4);

	for (int i = 0; i < 4; i++)
	{
		src_ip[3 - i] = resolveAddr >> (i * 8);
	}

	desaddress[0] = 0xff, desaddress[1] = 0xff, desaddress[2] = 0xff, desaddress[3] = 0xff,
		desaddress[5] = 0xff, desaddress[4] = 0xff;

	dst_ethernet.S_un.s_un_byte.e0 = 0xff;
	dst_ethernet.S_un.s_un_byte.e1 = 0xff;
	dst_ethernet.S_un.s_un_byte.e2 = 0xff;
	dst_ethernet.S_un.s_un_byte.e3 = 0xff;
	dst_ethernet.S_un.s_un_byte.e4 = 0xff;
	dst_ethernet.S_un.s_un_byte.e5 = 0xff;

	src_ip_cache.S_un.s_un_byte.i0 = src_ip[0];
	src_ip_cache.S_un.s_un_byte.i1 = src_ip[1];
	src_ip_cache.S_un.s_un_byte.i2 = src_ip[2];
	src_ip_cache.S_un.s_un_byte.i3 = src_ip[3];

	m_ETH->SetEnetDstAddress(desaddress);

	m_IP->SetSrcIPAddress(src_ip);
	m_IP->SetDstIPAddress(src_ip);
	m_ARP->SetSourceAddress(src_ip);
	m_ARP->SetDestinAddress(src_ip);

	CGratitousContol.GetWindowText(CGratitousDevAddr);
	macStr = LPSTR(LPCTSTR(CGratitousDevAddr));

	sscanf(macStr, "%02x%02x%02x%02x%02x%02x", &graOidData[0], &graOidData[1], &graOidData[2], &graOidData[3], &graOidData[4], &graOidData[5]);
	m_ARP->setSrcHd(graOidData);
	m_ETH->SetEnetSrcAddress(graOidData);
	mp_UnderLayer->Send(NULL, 0);
}