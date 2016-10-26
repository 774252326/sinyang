
// analyzerView.h : interface of the CanalyzerView class
//

#pragma once


#include "plot\PlotWnd.h"
#include "analyzerDoc.h"
#include <vector>


#define PW_LAST 0x01
#define PW_SHOW_ALL 0x02
#define PW_INIT 0x04




class CanalyzerView : public CView
{
public:
	static void CopyWndToClipboard( CWnd *pWnd )
	{
		CBitmap  bitmap;
		CClientDC dc(pWnd);
		CDC  memDC;
		CRect rect;

		memDC.CreateCompatibleDC(&dc); 

		pWnd->GetWindowRect(rect);

		bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height() );

		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
		memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY); 

		pWnd->OpenClipboard() ;
		EmptyClipboard() ;
		SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle() ) ;
		CloseClipboard () ;

		memDC.SelectObject(pOldBitmap);
		bitmap.Detach();
	};


protected: // create from serialization only
	CanalyzerView();
	DECLARE_DYNCREATE(CanalyzerView)

// Attributes
public:
	CanalyzerDoc* GetDocument() const;
	PlotWnd pw;
	CSpinButtonCtrl m_spBtn;
	CSize spBtnSize;
	std::vector<PlotDataEx> pdl;
// Operations
public:
	int UpdateSpinButton(int np, bool bLast);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CanalyzerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnControls1();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnOptionsPlotsettings();
protected:
	afx_msg LRESULT OnMessageChangeApplook(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageUpdateView(WPARAM wParam, LPARAM lParam);
	void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewFitwindow();
	//afx_msg LRESULT OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEditCopy();
	afx_msg void OnViewDatacursor();
	afx_msg void OnUpdateViewDatacursor(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in analyzerView.cpp
inline CanalyzerDoc* CanalyzerView::GetDocument() const
   { return reinterpret_cast<CanalyzerDoc*>(m_pDocument); }
#endif

