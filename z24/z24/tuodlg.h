#pragma once


// tuodlg dialog

class tuodlg : public CDialogEx
{
	DECLARE_DYNAMIC(tuodlg)

public:
	tuodlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~tuodlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
