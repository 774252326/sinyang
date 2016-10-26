#include "StdAfx.h"
#include "analyzerViewR.h"


CanalyzerViewR::CanalyzerViewR(void)
	: CanalyzerView(1)
{
	//lri=1;
}


CanalyzerViewR::~CanalyzerViewR(void)
{
}

//int CanalyzerViewR::AddPlot(const PlotData & pda)
//{
//	CanalyzerDoc* pDoc=GetDocument();
//	pDoc->rp.push_back(pda);
//	int newi=pDoc->rp.size()-1;
//	SetSpin(newi);
//	return newi;
//}