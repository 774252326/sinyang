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
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	double xmin,xmax,ymin,ymax;
	CString xlabel,ylabel;
	std::vector< std::vector<double> > xlist;
	std::vector< std::vector<double> > ylist;
	std::vector<COLORREF> clist;

	void genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, const CRect &rect, std::vector<CPoint> &pointlist);
	void drawRectangle(CRect rect, CDC * pDC, COLORREF insidecolor, COLORREF bordercolor);
	CRect DrawXYAxis(CRect rect, CDC* pdc);
	void updatePlotRange(const std::vector<double> &x, const std::vector<double> &y);
	afx_msg void OnPaint();

	void plot2d(const std::vector<double> &x, const std::vector<double> &y, const CString &xla, const CString &yla );

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CPoint m_mouseDownPoint;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


