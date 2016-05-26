// SubDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SubDlg.h"
#include "ArpAppDlg.h"
#include "afxdialogex.h"


// SubDlg dialog

IMPLEMENT_DYNAMIC(SubDlg, CDialog)

SubDlg::SubDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

SubDlg::~SubDlg()
{

}

void SubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, CDestAddr);
	DDX_Control(pDX, IDC_IPADDRESS2, CNetAddress);
	DDX_Control(pDX, IDC_IPADDRESS3, CGateAddr);
	DDX_Control(pDX, IDC_CHECK1, CUpCheck);
	DDX_Control(pDX, IDC_CHECK2, CGateCheck);
	//  DDX_Control(pDX, IDC_COMBO1, CStaticCombo);
	DDX_Control(pDX, IDC_COMBO1, CStaticCombo);
	DDX_Control(pDX, IDC_EDIT1, CMetric);
}


BEGIN_MESSAGE_MAP(SubDlg, CDialog)
	ON_BN_CLICKED(IDOK, &SubDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SubDlg message handlers


BOOL SubDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	StaticSetDlgState();
	return TRUE;
}

void SubDlg::StaticSetDlgState() // 다이얼로그 초기화 부분
{
	int i;
	CString device_description;
	CArpAppDlg* pApp = (CArpAppDlg*)AfxGetMainWnd();
	CComboBox* sCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);

	for (i = 0; i < 10; i++) {
		if (!pApp->GetNILayer()->GetAdapterObject(i))
			break;
		else {
			device_description = pApp->GetNILayer()->GetAdapterObject(i)->description;
			device_description.Trim();
			sCombo->AddString(device_description);
		}
	}
}

void SubDlg::OnBnClickedOk()
{
	CArpAppDlg* pApp = (CArpAppDlg*)AfxGetMainWnd();
	int flag;
	int device_interface;

	CDestAddr.GetAddress(destAddr[0], destAddr[1], destAddr[2], destAddr[3]);
	CNetAddress.GetAddress(netAddr[0], netAddr[1], netAddr[2], netAddr[3]);
	CGateAddr.GetAddress(gateAddr[0], gateAddr[1], gateAddr[2], gateAddr[3]);
	
	MsgHeader.Format(_T("%d.%d.%d.%d\t"), destAddr[0], destAddr[1], destAddr[2], destAddr[3]);
	pApp->srt_CList.InsertItem(0, _T(MsgHeader));   // 첫째행(0), 첫째열에 삽입
	
	MsgHeader.Format(_T("%d.%d.%d.%d\t"), netAddr[0], netAddr[1], netAddr[2], netAddr[3]);
	pApp->srt_CList.SetItem(0, 1, LVIF_TEXT, _T(MsgHeader), NULL, NULL, NULL, NULL); // 

	MsgHeader.Format(_T("%d.%d.%d.%d\t"), gateAddr[0], gateAddr[1], gateAddr[2], gateAddr[3]);
	pApp->srt_CList.SetItem(0, 2, LVIF_TEXT, _T(MsgHeader), NULL, NULL, NULL, NULL); 

	if ( IsDlgButtonChecked(IDC_CHECK1) && IsDlgButtonChecked(IDC_CHECK2)) {
		pApp->srt_CList.SetItem(0, 3, LVIF_TEXT, _T("UG"), NULL, NULL, NULL, NULL);
		flag = 1;
	}
	else if(IsDlgButtonChecked(IDC_CHECK1)){
		pApp->srt_CList.SetItem(0, 3, LVIF_TEXT, _T("U"), NULL, NULL, NULL, NULL);
		flag = 0;
	}
	
	
	CString deviceName;
	device_interface = CStaticCombo.GetCurSel();
	CStaticCombo.GetLBText(device_interface, deviceName);
	pApp->srt_CList.SetItem(0, 4, LVIF_TEXT, _T(deviceName), NULL, NULL, NULL, NULL);


	CString metric;
	CMetric.GetWindowTextA(metric);
	pApp->srt_CList.SetItem(0, 5, LVIF_TEXT, _T(metric), NULL, NULL, NULL, NULL);

	pApp->GetIPLayer()->InsertRouteTable(destAddr, netAddr, gateAddr, flag, device_interface);
	
	CDialog::OnOK();
}