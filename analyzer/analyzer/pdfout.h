//#include "0/aDoc.h"
#pragma once

//#include "analyzerDoc.h"
#include <afxwin.h>
#include <atltypes.h>
#include <vector>


#include "p/pdflib.hpp"
#pragma comment(lib, "p/pdflib.lib")

//#include "DataOutA.h"
//#include "PlotData.h"
//#include "ANPara.h"
//#include "VPara.h"
//#include "SAPara.h"

#include "struct\ANPara.hpp"
#include "struct\DataOutA.hpp"
#include "struct\SAPara.hpp"
#include "struct\VPara.hpp"

#include "struct1\PlotDataEx.hpp"

#include <iostream>
CString TimeString(bool bSeg=false);

//int pdfd(CString outfile, CanalyzerDoc *padoc);
int AddPageNumber(const std::wstring fin, const std::wstring fout);

int imgout2(
	pdflib::PDFlib &p, 
	//CanalyzerDoc *padoc, 
	CDC *pdc,
	std::vector<PlotDataEx> &imagelist, 
	std::vector<CString> &namelist, 
	CSize szimg=CSize(1000,690));

int pdfout(pdflib::PDFlib &p, const std::vector<DataOutA> &dol);

int pdfout6(pdflib::PDFlib &p, 
	const ANPara &para, 
	const std::vector<CString> &res, 
	const CVPara &p2, 
	const SAPara &p3,
	bool b1=true,
	bool b2=true,
	bool b3=true,
	bool b4=true);