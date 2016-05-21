#pragma once


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
};
