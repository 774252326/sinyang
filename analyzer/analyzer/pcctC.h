#pragma once

#include <vector>
#include "sapitemA.h"
#include "VPara.h"
#include "DataOutA.h"

//one filter BYTE 
//#define pcctC_VMS 0x01
//#define pcctC_S 0x02
//#define pcctC_A 0x04
////#define pcctC_L 0x08
//#define pcctC_MORE 0x10
//#define pcctC_SAMPLE 0x20
//#define pcctC_RECOUNT 0x40
//#define pcctC_RESET_SOLUTION_AT_END 0x80

class pcctC
{
public:
	pcctC(void);
	~pcctC(void);

	std::vector< std::vector<double> > xx;
	std::vector< std::vector<double> > yy;
	//std::vector<double> Ar;


	//in cycle=0;one cycle end=1;all cycle end=2
	int addXY( std::vector<double> & x, std::vector<double> & y);
	//bool StartLoad;

	double intg(double xUpLim);
	double intg();
	void clear(void);

	////////////////////////parameters for calculation////////////////
	///////////////////////never change after initailize/////////////////////////
	//second per voltage
	//double spv;
	//double intgUpLim;
	//double xmin;
	//double xmax;
	//int nCycle;

	CVPara cvp;

	//////////////counter///////////////////////
	/*static*/ int rowCount;
	/*static*/ int stepCount;
	//////////////////////////////////////

	////////////////static parameters///////////////////////
	/////////////////update after one step complete////////////////////////
	bool bUpdateAr0;
	/////*static*/ double totalVolume;
	///*static*/ double Ar0;
	//double Sml;
	//double Aml;
	//double Lml;
	//bool bUnknown;
	//double VMSVolume;
	//double additiveVolume;
	//double addVolume;
	//CString stepName;

	DataOutA doa;

	//////////////////////////////////
	bool ReadTask(sapitemA sapi, BYTE bFlag=DOA_VMS);
	void initialPara(CVPara cvpa);
	//double TotalVolume(void);
	//double AConc(void);
	//double SConc(void);
	//double LConc(void);
private:
	//bool ConcOnce(double &ml, double endConc, double addConc);
	//bool ConcIntv(double &ml, double intvConc, double addConc);
};

