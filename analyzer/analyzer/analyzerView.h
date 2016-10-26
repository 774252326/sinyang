
// analyzerView.h : interface of the CanalyzerView class
//

#pragma once

#include "resource.h"
#include "analyzerDoc.h"

class CanalyzerView : public CFormView
{
protected: // create from serialization only
	CanalyzerView();
	DECLARE_DYNCREATE(CanalyzerView)

public:
	enum{ IDD = IDD_ANALYZER_FORM };

// Attributes
public:
	CanalyzerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

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
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	CSpinButtonCtrl m_spBtn;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CSize spBtnSize;
	int metricGridLong;
	int metricGridShort;
	int gap0;
	//int lidx;
	double xmin,xmax,ymin,ymax;
	CPoint m_mouseDownPoint;

	void GetPlotRect(CRect & plotRect
		, int labelSize
		, int metricSize);
	void genPointToPlot(const std::vector<double> &px
		, const std::vector<double> &py
		, const CRect &rect
		, std::vector<CPoint> &pointlist);
	CRect DrawXYAxis(CRect rect
		, CDC * pdc
		, const PlotSpec & psp
		, const CString & xlab
		, const CString & ylab);
	void DrawGridLine(const CRect & rect
		, CDC * pdc
		, int gridType
		, COLORREF gridC
		, const std::vector<long> & gridH
		, const std::vector<long> & gridV);
	CRect DrawMetric(const CRect & rect
		, CDC * pdc
		, int metricSize
		, COLORREF metricC
		, COLORREF gridC
		, const std::vector<long> & gridH
		, const std::vector<long> & gridV
		, const std::vector<double> & gridx
		, const std::vector<double> & gridy);
	CRect DrawLabel(const CRect & rect
		, CDC * pdc
		, CPoint centerP
		, int labelSize
		, COLORREF labelC
		, const CString & xlab
		, const CString & ylab);
	void drawRectangle(const CRect &rect
		, CDC * pDC
		, COLORREF insidecolor
		, COLORREF bordercolor);
	void DrawSpline( CPoint * lpPoints
		, int np
		, int lowlim
		, int highlim
		, CDC * pDC);

	CRect DrawLegend(CRect rect
		, CDC* pDC
		, const std::vector<LineSpec> &ps
		, COLORREF bkColor);

	void DrawCurve(CDC* pDC
	, std::vector<CPoint> &pointlist
	, const std::vector<DWORD> &ll
	, const std::vector<LineSpec> &ps
	, int lborder
	, int rborder);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void MoveUpdate(int mtsz
		, int lbsz
		, CPoint point);
	void WheelUpdate(int mtsz
		, int lbsz
		, CPoint point
		, double k1);
};

#ifndef _DEBUG  // debug version in analyzerView.cpp
inline CanalyzerDoc* CanalyzerView::GetDocument() const
   { return reinterpret_cast<CanalyzerDoc*>(m_pDocument); }
#endif

