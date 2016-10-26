#pragma once
#include "plotdata.hpp"


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
	CPoint m_mouseDownPoint;
	bool bMouseCursor;
	size_t selectPIdx;
	double zoomrate;
	
	PlotData pd;

	void ResetRange(void);

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
};


