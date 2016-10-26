//#include "0/aDoc.h"
#pragma once

//#include "analyzerDoc.h"
#include <afxwin.h>
#include <atltypes.h>
#include <vector>
#include "DataOutA.h"
#include "PlotData.h"
#include "ANPara.h"
#include "VPara.h"
#include "SAPara.h"

#include <iostream>

#include "p/pdflib.hpp"
#pragma comment(lib, "p/pdflib.lib")


//int pdfd(CString outfile, CanalyzerDoc *padoc);
int AddPageNumber(const std::wstring fin, const std::wstring fout);

int imgout2(
	pdflib::PDFlib &p, 
	//CanalyzerDoc *padoc, 
	CDC *pdc,
	std::vector<PlotData> &imagelist, 
	std::vector<CString> &namelist, 
	CSize szimg=CSize(800,600));

int pdfout(pdflib::PDFlib &p, const std::vector<DataOutA> &dol);

int pdfout6(pdflib::PDFlib &p, const ANPara &para, const std::vector<CString> &res, const CVPara &p2, const SAPara &p3);