#pragma once
#include "plotdata.hpp"

//#include "LegendDlg.h"
#include "atltypes.h"
#include "legendspec.hpp"
//#include "LegendDlgB.h"
#include "LegendCondition.hpp"
// PlotWnd




class PlotWnd : public CWnd
{
	DECLARE_DYNAMIC(PlotWnd)

public:
	PlotWnd();
	virtual ~PlotWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double pct;
	size_t selectPIdx;

	bool bMouseCursor;
	
	double zoomrate;
	PlotData pd;
	LegendSpec lgs;



	LegendCondition lgc;


	void ResetRange(void);
	void SetLegend(void);
	afx_msg void OnMove(int x, int y);
	CRect GetWindowPlotRect(bool bWnd=false);

protected:
	CPoint m_mouseDownPoint;
	CPoint wndPosition;	
	CDialogEx *td;

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CToolTipCtrl m_tool;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void ShowLegend(bool bShow);
	


};


