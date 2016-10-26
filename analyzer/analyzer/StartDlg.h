#pragma once


// StartDlg dialog

class StartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(StartDlg)

public:
	StartDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~StartDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_START };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
