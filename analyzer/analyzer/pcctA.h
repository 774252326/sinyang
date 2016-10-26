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



	bool addXY( std::vector<double> & x, std::vector<double> & y);
	bool StartLoad;
	double xmin;
	double xmax;
	double intg(const double & xUpLim);
	void clear(void);
};

