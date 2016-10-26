#pragma once


// tipsdlg dialog

class tipsdlg : public CDialogEx
{
	DECLARE_DYNAMIC(tipsdlg)

public:
	tipsdlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~tipsdlg();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
