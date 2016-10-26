#pragma once

#include <vector>
#include "sapitemA.h"
#include "VPara.h"

#define PCCTB_SAMPLE 0x00
#define PCCTB_VMS 0x01
#define PCCTB_S 0x02
#define PCCTB_A 0x04
#define PCCTB_L 0x08
#define PCCTB_MORE 0x10


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
	bool ReadTask(sapitemA sapi, BYTE bFlag=PCCTB_VMS);
	void initialPara(CVPara cvp);
	double TotalVolume(void);
	double AConc(void);
	double SConc(void);
	double LConc(void);
};

