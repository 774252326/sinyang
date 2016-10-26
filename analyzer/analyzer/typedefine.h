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
	int calibrationfactortype;
	double calibrationfactor;
	CString calfilepath;
} anp;


//CVParameters
typedef struct CVP{
	double lowelimit;
	double highelimit;
	double scanrate;
	int combochoice;
	int noofcycles;
	double variationtolerance;
	double rotationrate;
	double endintegratione;
} cvp;

//SolutionAdditionParameters

typedef struct SAPITEM{
	double Sconc;
	double Aconc;
	double Lconc;
	int addtype;
	double volconc;
} sapitem;


typedef struct SAP{
	double vmsvol;
	std::vector<sapitem> saplist;
} sap;

typedef struct PLOTSPEC{
	COLORREF colour;
	CString name;
	int dotSize;
	bool showLine;
	int smoothLine;
	bool traceLast;
} plotspec;


void mreadini(wchar_t *fn, anp &p1, cvp &p2, sap &p3 );

void mwriteini(wchar_t *fn, const anp &p1, const cvp &p2, const sap &p3 );
