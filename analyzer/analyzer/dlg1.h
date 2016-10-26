#pragma once

#include "resource.h"  
#include <vector>
#include "atltypes.h"

//#include "typedefine.h"
#include "PlotData.h"

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

	double xmin,xmax,ymin,ymax;


	void genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, const CRect &rect, std::vector<CPoint> &pointlist);
	void drawRectangle(const CRect &rect, CDC * pDC, COLORREF insidecolor, COLORREF bordercolor);

	CRect DrawLegend1(CRect rect, CDC* pDC);


	void DrawCurveA(CRect rect, CDC* pDC);

	void GetPlotRect(CRect & plotRect);
	void DrawSpline( CPoint * lpPoints, int np, CRect rect, CDC * pDC);
	CRect DrawXYAxis1(CRect rect, CDC * pdc);
	void DrawGridLine(const CRect & rect, CDC * pdc, int gridType, COLORREF gridC, const std::vector<long> & gridH, const std::vector<long> & gridV);
	CRect DrawMetric(const CRect & rect, CDC * pdc, int metricSize, COLORREF metricC, COLORREF gridC, const std::vector<long> & gridH, const std::vector<long> & gridV, const std::vector<double> & gridx, const std::vector<double> & gridy);
	CRect DrawLabel(const CRect & rect, CDC * pdc, CPoint centerP, int labelSize, COLORREF labelC);
	int metricGridLong;
	int metricGridShort;
	CSize spBtnSize;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
public:
	//PlotData pd;

	//void smoothLine(void);
	void clear(void);
	bool updatePlotRange(int plotIndex, const std::vector<double> &x, const std::vector<double> &y, bool flg=false);
	bool updatePlotRange(int plotIndex, bool flg=true);
	bool updatePlotRange(bool flg=true);
	figspec fs;

	CSpinButtonCtrl m_spBtn;

	std::vector<PlotData> pdl;
	int AddPlot(const PlotData & pda);

	BOOL SaveFile(CString fp);
	BOOL ReadFile(CString fp);
};


