#pragma once
//#include "../struct1\\plotdata.hpp"

//#include "LegendDlg.h"
#include "atltypes.h"
//#include "../struct1\\legendspec.hpp"
//#include "LegendDlgB.h"
//#include "../struct1\\LegendCondition.hpp"

#include "../struct1/PlotDataEx.hpp"

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
	//double xmin;
	//double xmax;
	//double ymin;
	//double ymax;
	//
	//PlotData pd;
	//LegendSpec lgs;

	//LegendCondition lgc;
	PlotDataEx pdex;

	size_t selectPIdx;
	double pct;
	bool bMouseCursor;	
	double zoomrate;


	void ResetRange(void);
	void SetLegend(void);

	

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);


	CToolTipCtrl m_tool;
	CDialogEx *td;
	CPoint wndPosition;
	CPoint m_mouseDownPoint;

	void ShowLegend(bool bShow);
	

public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


