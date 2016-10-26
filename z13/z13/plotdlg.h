#pragma once


// plotdlg dialog

class plotdlg : public CDialogEx
{
	DECLARE_DYNAMIC(plotdlg)

public:
	plotdlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~plotdlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
