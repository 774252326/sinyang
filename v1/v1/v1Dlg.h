
// v1Dlg.h : header file
//

#pragma once
#include "atlimage.h"
#include "atltypes.h"


// Cv1Dlg dialog
class Cv1Dlg : public CDialogEx
{
// Construction
public:
	Cv1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_V1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CImage img;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CPoint m_mouseDownPoint;
	CRect imgrect;
	CSize imgsz;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
