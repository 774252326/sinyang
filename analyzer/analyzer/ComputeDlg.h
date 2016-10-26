#pragma once

#include "ParaList.h"


// ComputeDlg dialog

class ComputeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ComputeDlg)

public:
	ComputeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ComputeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ParaList m_list;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();

	void ClickCompute(void);
	void Showfml(void);
	//CDialogEx *fd;
	virtual void OnCancel();
	UINT GetFormulaImgID(void);
	afx_msg void OnPaint();

	CEdit EditEDRS_ID;
	CBitmap formulaImg;
	CButton BtnIDS_BTN_COMPUTE;
	CRect imgrc;
};
