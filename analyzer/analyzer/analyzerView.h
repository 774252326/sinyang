
// analyzerView.h : interface of the CanalyzerView class
//

#pragma once

#include "resource.h"
#include "analyzerDoc.h"
#include "atltypes.h"


class CanalyzerView : public CView
{
protected: // create from serialization only
	CanalyzerView();
	DECLARE_DYNCREATE(CanalyzerView)

// Attributes
public:
	CanalyzerDoc* GetDocument() const;

// Operations
public:

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
	//afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	CSpinButtonCtrl m_spBtn;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CSize spBtnSize;
	double xmin,xmax,ymin,ymax;
	CPoint m_mouseDownPoint;


	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	void clear(void);

	afx_msg void OnOptionsPlotsettings();

	afx_msg void OnViewFitwindow();

	bool updatePlotRange(int plotIndex, const std::vector<double> &x, const std::vector<double> &y, bool flg=false);
	bool updatePlotRange(int plotIndex, bool flg=true);
	bool updatePlotRange(bool flg=true);

	int AddPlot(const PlotData & pda);


	void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	void SetSpin(int i);


	CToolTipCtrl m_tool;

	virtual BOOL PreTranslateMessage(MSG* pMsg);


	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
};

#ifndef _DEBUG  // debug version in analyzerView.cpp
inline CanalyzerDoc* CanalyzerView::GetDocument() const
   { return reinterpret_cast<CanalyzerDoc*>(m_pDocument); }
#endif

