#pragma once

//#include <iostream>     // std::cout
//#include <algorithm>    // std::find
#include <vector>       // std::vector
//#include <fstream>

//AnalysisParameters
typedef struct ANP{
int analysistype;
double evaluationratio;
double endpointratio;
double calibrationfactor;
} anp;


//CVParameters
typedef struct CVP{
	double lowelimit;
	double highelimit;
	double scanrate;
	int noofcycles;
	double rotationrate;
	double endintegratione;
} cvp;

//SolutionAdditionParameters

typedef struct SAP{
	double Sconc;
	double Aconc;
	double Lconc;
	double volume;
} sap;


typedef struct PLOTSPEC{
	COLORREF colour;
	CString name;
	int dotSize;
	bool showLine;
	int smoothLine;
} plotspec;


void mreadini(wchar_t *fn, anp &p1, cvp &p2, std::vector<sap> &p3 );

void mwriteini(wchar_t *fn, const anp &p1, const cvp &p2, const std::vector<sap> &p3 );
