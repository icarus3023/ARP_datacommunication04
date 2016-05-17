#pragma once
#include "explorer1.h"
#include "afxwin.h"
#include "afxcmn.h"
class CProxyDlg
{
public:
	CProxyDlg();
	~CProxyDlg();
};

#pragma once


// ProxyDlg dialog

class ProxyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ProxyDlg)

public:
	ProxyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProxyDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROXYDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 ProxyWindow;
	CEdit CDeviceName;
	CEdit CEthernetAddress;
	CIPAddressCtrl CIPAddress;
	afx_msg void OnBnClickedProxyadd();
	afx_msg void OnBnClickedProxycancel();
	CString CDeviceAddress;
private:
	unsigned char proxy_ip_addr[4];
	CString device_name;
	//CString ether_address;
	IP_ADDR				proxyIpCache;
	ETHERNET_ADDR		dst_ethernet;

	unsigned char proxyOidData[12];
	char* proxyDeviceAddressStr;
	CString MsgHeader;
};
