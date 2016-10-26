#pragma once

#include <vector>
#include "atltypes.h"

// dlg1 form view

class dlg1 : public CFormView
{
	DECLARE_DYNCREATE(dlg1)

protected:
	dlg1();           // protected constructor used by dynamic creation
	virtual ~dlg1();

public:
	enum { IDD = IDD_DIALOG1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CPoint m_mouseDownPoint;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

public:
	void plot2d(const std::vector<double> &x, const std::vector<double> &y, const CString &xla, const CString &yla );
	void plot2d(const std::vector<double> &x, const std::vector<double> &y, const CString &xla, const CString &yla, const CString &title );
	
private:
	double xmin,xmax,ymin,ymax;
	CString xlabel,ylabel;
	std::vector< std::vector<double> > xlist;
	std::vector< std::vector<double> > ylist;

	std::vector<double> xll;
	std::vector<double> yll;
	std::vector<DWORD> ll;

	std::vector<COLORREF> clist;
	std::vector<CString> names;

	void genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, const CRect &rect, std::vector<CPoint> &pointlist);
	void drawRectangle(const CRect &rect, CDC * pDC, COLORREF insidecolor, COLORREF bordercolor);
	CRect DrawXYAxis(CRect rect, CDC* pdc);
	void updatePlotRange(const std::vector<double> &x, const std::vector<double> &y, bool flg=false);

	CRect DrawLegend(CRect rect, CDC* pDC);
	CRect DrawLegend1(CRect rect, CDC* pDC);
	void DrawPoint(CRect rect, CDC * pDC, int d);
	void DrawCurve(CRect rect, CDC* pDC);	
	void GetPlotRect(CRect & plotRect);

public:
	void showall(void);
};


