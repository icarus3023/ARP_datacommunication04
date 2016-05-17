#include "stdafx.h"
#include "ProxyDlg.h"
#include "ArpAppDlg.h"
#include <stdlib.h>
#include "arp.h"
#include "afxdialogex.h"

CProxyDlg::CProxyDlg()
{
}

CProxyDlg::~CProxyDlg()
{
}
// ProxyDlg.cpp : implementation file
//



// ProxyDlg dialog

IMPLEMENT_DYNAMIC(ProxyDlg, CDialogEx)

ProxyDlg::ProxyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROXYDLG, pParent)
	, CDeviceAddress(_T(""))
{

}

ProxyDlg::~ProxyDlg()
{
}

void ProxyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, ProxyWindow);
	DDX_Control(pDX, IDC_PROXYDEVICE, CDeviceName);
	DDX_Control(pDX, IDC_PROXYETHERNET, CEthernetAddress);
	DDX_Control(pDX, IDC_PROXYIP, CIPAddress);
	DDX_Text(pDX, IDC_PROXYETHERNET, CDeviceAddress);
	DDV_MaxChars(pDX, CDeviceAddress, 12);
}


BEGIN_MESSAGE_MAP(ProxyDlg, CDialogEx)
	ON_BN_CLICKED(ID_PROXYADD, &ProxyDlg::OnBnClickedProxyadd)
	ON_BN_CLICKED(ID_PROXYCANCEL, &ProxyDlg::OnBnClickedProxycancel)
END_MESSAGE_MAP()


// ProxyDlg message handlers
void ProxyDlg::OnBnClickedProxyadd()
{
	CArpAppDlg* pApp = (CArpAppDlg*)AfxGetMainWnd();

	CIPAddress.GetAddress(proxy_ip_addr[0], proxy_ip_addr[1], proxy_ip_addr[2], proxy_ip_addr[3]);
	CDeviceName.GetWindowTextA(device_name);

	CEthernetAddress.GetWindowTextA(CDeviceAddress);
	proxyDeviceAddressStr = LPSTR(LPCTSTR(CDeviceAddress));
	sscanf(proxyDeviceAddressStr, "%02x%02x%02x%02x%02x%02x",
		&proxyOidData[0], &proxyOidData[1], &proxyOidData[2],
		&proxyOidData[3], &proxyOidData[4], &proxyOidData[5]);

	memcpy(dst_ethernet.S_un.s_ether_addr, proxyOidData, 6);

	
	MsgHeader.Format(_T("%d.%d.%d.%d\t%s\t%02x:%02x:%02x:%02x:%02x:%02x"), proxy_ip_addr[0], proxy_ip_addr[1], proxy_ip_addr[2], proxy_ip_addr[3]
		, device_name, proxyOidData[0], proxyOidData[1], proxyOidData[2],
		proxyOidData[3], proxyOidData[4], proxyOidData[5]);

	proxyIpCache.S_un.s_un_byte.i0 = proxy_ip_addr[0];
	proxyIpCache.S_un.s_un_byte.i1 = proxy_ip_addr[1];
	proxyIpCache.S_un.s_un_byte.i2 = proxy_ip_addr[2];
	proxyIpCache.S_un.s_un_byte.i3 = proxy_ip_addr[3];

	pApp->GetArpLayer()->InsertProxyTable(proxyIpCache, dst_ethernet);


	pApp->CProxyList.AddString(MsgHeader);
	OnOK();
}

void ProxyDlg::OnBnClickedProxycancel()
{
	OnOK();
}
