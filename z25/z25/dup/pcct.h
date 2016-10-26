#pragma once

#include <vector>
#include <algorithm>

typedef struct SEGMENT{
	double Ep;
	double ip;
	double Ah;
	wchar_t EpUnit;
	wchar_t ipUnit;
	wchar_t AhUnit;
} segment;

class pcct
{
public:
	pcct(void);
	~pcct(void);

	std::vector<segment> segmentList;
	//Potential/V, Current/A, Charge/C, Time/s
	std::vector<double> potential;
	std::vector<double> current;
	std::vector<double> charge;
	std::vector<double> time;


	//int readFile(LPCTSTR lpszFileName, LPCTSTR header);
	int readFile(LPCTSTR lpszFileName);

	
	void clear(void);
	void seperate(void);
	std::vector<long> xBreakIndex;
	double intg(double xtop, size_t xibegin, size_t xiend);
	double AR;
	CString segmentinfo;
	std::vector<CString> label;
	CString FilePath;
	CString FileName;
	std::vector<CString> seginfo;
	
	void copy(pcct a);
	void copyPrefix(pcct a);
	void copyData(pcct a);
	size_t popData(std::vector<double> & x, std::vector<double> & y, size_t nd);

	void TomA(void);

	double addVolume;
	CString stepName;
	///////////////////////////////////////////////////////
	int readFile1(LPCTSTR lpszFileName);
	double intg1();

	double intg2(double xtop);


	void SetTimeIntv(double intv=0);
};

