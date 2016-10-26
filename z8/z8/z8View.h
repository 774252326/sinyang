
// z8View.h : interface of the Cz8View class
//

#pragma once

#include "resource.h"
#include "pcct.h"


class Cz8View : public CFormView
{
protected: // create from serialization only
	Cz8View();
	DECLARE_DYNCREATE(Cz8View)

public:
	enum{ IDD = IDD_Z8_FORM };

// Attributes
public:
	Cz8Doc* GetDocument() const;

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
	virtual ~Cz8View();
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
	afx_msg void OnFileOpen();
	CString m_filePath;
	virtual void OnDraw(CDC* /*pDC*/);
	pcct dt1;
	bool isLoad;
	afx_msg void OnPaint();
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	//CRect DrawXYAxis(CRect rect, CPaintDC* pdc);
	CRect DrawXYAxis(CRect rect, CDC* pdc);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	long ci;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	double *x;
	double *y;
	CString windowTitle;
	// update xmin,xmax,ymin,ymax
	void updatePlotRange(std::vector<double> x, std::vector<double> y);

	std::vector<pcct> dtlist;
	std::vector<CPoint> genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, CRect rect);
	std::vector<COLORREF> cllist;
	double xmouse;
	double ymouse;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in z8View.cpp
inline Cz8Doc* Cz8View::GetDocument() const
   { return reinterpret_cast<Cz8Doc*>(m_pDocument); }
#endif

