#pragma once
#include "legenddlga.h"
class LegendDlgB :
	public LegendDlgA
{
public:
	LegendDlgB(CWnd* pParent);
	virtual ~LegendDlgB(void);
	CWnd *pwnd;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	virtual BOOL OnInitDialog();
	void Position(void);
	CPoint GetPos(void);
	CPoint CalAlignPos(bool bTop, bool bLeft, int axisWidth=2);
};

