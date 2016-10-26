#pragma once
#include "ListCtrlLine.h"

// ExportDataDlg dialog

class ExportDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ExportDataDlg)

public:
	ExportDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ExportDataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	ListCtrlLine lcl;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CMFCEditBrowseCtrl m_c;
	CMFCButton m_exp;
};
