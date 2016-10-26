#pragma once

#include <vector>

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



	int readFile(LPCTSTR lpszFileName);
	
	void clear(void);
};

