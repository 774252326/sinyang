#pragma once
#include "afxwin.h"



#define MESSAGE_BUSY WM_USER+1
#define MESSAGE_WAIT_RESPONSE WM_USER+2
#define MESSAGE_OVER WM_USER+3

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

	virtual void OnDrawText(
   CDC* pDC,
   CRect rect,
   const CString& strText 
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
	void ShowMessageRunning(void);
	void ShowMessage(CString str);
	void ShowMessageWithButton(CString str, double xv, bool bFlash);
	int ecWidth;
protected:
	afx_msg LRESULT OnMessageBusy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageWaitResponse(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageOver(WPARAM wParam, LPARAM lParam);
};


