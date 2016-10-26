// ANPara.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ANPara.h"


// ANPara

ANPara::ANPara()
	: analysistype(0)
	, evaluationratio(0)
	, endpointratio(0)
	, calibrationfactortype(0)
	, calibrationfactor(0)
	, interceptvalue(0)
	, calibrationfilepath(_T(""))
{
}

ANPara::~ANPara()
{
}


// ANPara member functions


void ANPara::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<analysistype
			<<evaluationratio
			<<endpointratio
			<<calibrationfactortype
			<<calibrationfactor
			<<calibrationfilepath
			<<interceptvalue;
	}
	else
	{	// loading code
		ar>>analysistype
			>>evaluationratio
			>>endpointratio
			>>calibrationfactortype
			>>calibrationfactor
			>>calibrationfilepath
			>>interceptvalue;
	}
}
