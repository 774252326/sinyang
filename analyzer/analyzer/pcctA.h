#pragma once

#include <vector>

class pcctA
{
public:
	pcctA(void);
	~pcctA(void);

	std::vector< std::vector<double> > xx;
	std::vector< std::vector<double> > yy;
	std::vector<double> Ar;


	//in cycle=0;one cycle end=1;all cycle end=2
	int addXY( std::vector<double> & x, std::vector<double> & y);
	//bool StartLoad;
	double xmin;
	double xmax;
	double intg(double xUpLim);
	void clear(void);
	//second per voltage
	double spv;
	double intgUpLim;
	double intg();
	int nCycle;
};

