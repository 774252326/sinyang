#pragma once


// WaitDlg dialog



enum ProcessState{
	stop,
	running,
	pause
};


class WaitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WaitDlg)

public:
	WaitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~WaitDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	CString m_tips;
	virtual BOOL OnInitDialog();

	CFont  editfont;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
