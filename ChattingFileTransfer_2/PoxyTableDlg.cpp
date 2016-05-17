// PoxyTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ipc.h"
#include "PoxyTableDlg.h"
#include "afxdialogex.h"


// CPoxyTableDlg dialog

IMPLEMENT_DYNAMIC(CPoxyTableDlg, CDialogEx)

CPoxyTableDlg::CPoxyTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROXYTABLEADD, pParent)
{

}

CPoxyTableDlg::~CPoxyTableDlg()
{
}

void CPoxyTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddr);
}


BEGIN_MESSAGE_MAP(CPoxyTableDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPoxyTableDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPoxyTableDlg message handlers


void CPoxyTableDlg::OnBnClickedOk()
{
		UpdateData( TRUE ) ;
		/*
	if ( !m_stMessage.IsEmpty() )
	{
		SetTimer(1,3000,NULL);

		SendData( ) ;

		m_stMessage = "" ;

		(CEdit*) GetDlgItem( IDC_EDIT_MSG )->SetFocus( ) ;
	}
	UpdateData( FALSE ) ;*/
	CDialogEx::OnOK();
}
