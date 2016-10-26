#pragma once
#include "afxwin.h"


// CMFCCaptionBarA

class CMFCCaptionBarA : public CMFCCaptionBar
{
	DECLARE_DYNAMIC(CMFCCaptionBarA)

public:
	CMFCCaptionBarA();
	virtual ~CMFCCaptionBarA();

protected:
	virtual void OnDrawButton(
   CDC* pDC,
   CRect rect,
   const CString& strButton,
   BOOL bEnabled 
);

	DECLARE_MESSAGE_MAP()
public:
	CEdit ec;
	CStatic st;
	CButton bn;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int SetEdit(void);
	int HideEdit(void);
	int ShowButton(bool bShow=true);
	int SetTextA(CString str, bool bFlash=false);
	int timer;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void DoDataExchange(CDataExchange* pDX);
	double x;
};


