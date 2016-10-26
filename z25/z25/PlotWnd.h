#pragma once
#include "plotdata.hpp"

#include "LegendDlgA.h"
#include "atltypes.h"
#include "legendspec.h"

// PlotWnd

#define LEGEND_DP_ALIGN	0x01
#define LEGEND_DP_LEFT	0x02
#define LEGEND_DP_TOP	0x04
#define LEGEND_DP_FIT_RECT	0x08
#define LEGEND_DP_AUTO_RECT	0x10

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
	CPoint m_mouseDownPoint;
	bool bMouseCursor;
	size_t selectPIdx;
	double zoomrate;
	
	PlotData pd;
	LegendSpec lgs;

	void ResetRange(void);

	LegendDlgA *td;

protected:
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
public:
	afx_msg void OnMove(int x, int y);
	CPoint wndPosition;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int SetLegend(void);
	void ShowLegend(bool bShow);
	
	BYTE legendDpMode;
	CRect lgrect;
};


