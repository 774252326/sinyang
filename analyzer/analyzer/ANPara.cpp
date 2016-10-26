// ANPara.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ANPara.h"


// ANPara

ANPara::ANPara()
	: analysistype(0)
	, evaluationratio(0)
	, endpointratio(1)
	, calibrationfactortype(0)
	, calibrationfactor(0)
	, interceptvalue(0)
	, calibrationfilepath(_T(""))
{
}

ANPara::~ANPara()
{
}


//ANPara::ANPara(const ANPara &src)
//{
//	//analysistype=src.analysistype;
//	//evaluationratio=src.evaluationratio;
//	//endpointratio=src.endpointratio;
//	//calibrationfactortype=src.calibrationfactortype;
//	//calibrationfactor=src.calibrationfactor;
//	//interceptvalue=src.interceptvalue;
//	//calibrationfilepath=src.calibrationfilepath;
//}

void ANPara::operator=(const ANPara &src)
{
	analysistype=src.analysistype;
	evaluationratio=src.evaluationratio;
	endpointratio=src.endpointratio;
	calibrationfactortype=src.calibrationfactortype;
	calibrationfactor=src.calibrationfactor;
	interceptvalue=src.interceptvalue;
	calibrationfilepath=src.calibrationfilepath;
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
