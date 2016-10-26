#pragma once

#include <vector>
#include "sapitemA.h"
#include "VPara.h"

class pcctB
{
public:
	pcctB(void);
	~pcctB(void);

	std::vector< std::vector<double> > xx;
	std::vector< std::vector<double> > yy;
	std::vector<double> Ar;


	//in cycle=0;one cycle end=1;all cycle end=2
	int addXY( std::vector<double> & x, std::vector<double> & y);
	//bool StartLoad;

	double intg(double xUpLim);
	double intg();
	void clear(void);

	////////////////////////parameters for calculation////////////////
	///////////////////////never change after initailize/////////////////////////
	//second per voltage
	double spv;
	double intgUpLim;
	double xmin;
	double xmax;
	int nCycle;

	//////////////counter///////////////////////
	/*static*/ int rowCount;
	/*static*/ int stepCount;
	//////////////////////////////////////

	////////////////static parameters///////////////////////
	/////////////////update after one step complete////////////////////////
	bool bUpdateAr0;
	///*static*/ double totalVolume;
	/*static*/ double Ar0;
	double Sml;
	double Aml;
	double Lml;
	bool bUnknown;
	double VMSVolume;
	double additiveVolume;
	double addVolume;
	CString stepName;
	//////////////////////////////////
	bool ReadTask(sapitemA sapi);
	void initialPara(CVPara cvp);
};

