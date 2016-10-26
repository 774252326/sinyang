#pragma once
#include "analyzerview.h"
class CanalyzerViewL :
	public CanalyzerView
{
public:
	CanalyzerViewL(void);
	~CanalyzerViewL(void);
	//int AddPlot(const PlotData & pda);
	virtual void OnInitialUpdate();
};

