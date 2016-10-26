#pragma once
#include "analyzerview.h"
class CanalyzerViewL :
	public CanalyzerView
{
public:
	CanalyzerViewL(void);
	~CanalyzerViewL(void);
	int idx;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
};

