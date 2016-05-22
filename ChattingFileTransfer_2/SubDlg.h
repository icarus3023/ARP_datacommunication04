#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <pcap.h>
#include <WinSock2.h>
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"

// SubDlg dialog

class SubDlg : public CDialog
{
	DECLARE_DYNAMIC(SubDlg)

public:
	SubDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SubDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	IP_ADDR		DestAddr;
	IP_ADDR		NetAddr;
	IP_ADDR		GateAddr;
	afx_msg void OnBnClickedOk();
	void StaticSetDlgState();
	BOOL OnInitDialog();

private:
	unsigned char destAddr[4];
	unsigned char netAddr[4];
	unsigned char gateAddr[4];

	CString MsgHeader;

	

public:
	CIPAddressCtrl CDestAddr;
	CIPAddressCtrl CNetAddress;
	CIPAddressCtrl CGateAddr;
	CButton CUpCheck;
	CButton CGateCheck;
//	CComboBox CStaticCombo;
	CComboBox CStaticCombo;
};