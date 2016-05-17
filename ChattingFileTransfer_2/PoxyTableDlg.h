#pragma once
#include "afxcmn.h"


// CPoxyTableDlg dialog

class CPoxyTableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPoxyTableDlg)

public:
	CPoxyTableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPoxyTableDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROXYTABLEADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CIPAddressCtrl m_unSrcIPAddr;
};
