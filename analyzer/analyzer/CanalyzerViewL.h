#pragma once

//#include "resource.h"
//#include "analyzerDoc.h"
#include "analyzerview.h"
#include "analyzerDoc.h"

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
	void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);

	
	bool updatePlotRange(int plotIndex, bool flg=true);
	bool updatePlotRange(bool flg=true);

	int AddPlot(const PlotData & pda);
	afx_msg void OnViewFitwindow();
	afx_msg void OnOptionsPlotsettings();
};

