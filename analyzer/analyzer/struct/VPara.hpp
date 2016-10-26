#pragma once
#include "../ObjectF.hpp"
// CVPara command target

//CVParameters

class CVPara : public ObjectF
{
public:
	double lowelimit;
	double highelimit;
	double scanrate;
	int combochoice;
	int noofcycles;
	double variationtolerance;
	double rotationrate;
	double endintegratione;
	double startintegratione;
	double samplefrequecy;
	double quiettime;

	CVPara()
		: lowelimit(-0.23)
		, highelimit(1.57)
		, scanrate(0.1)
		, combochoice(0)
		, noofcycles(4)
		, variationtolerance(0)
		, rotationrate(2000)
		, endintegratione(1)
		, startintegratione(0)
		, samplefrequecy(100)
		, quiettime(1)
	{
	};

	virtual ~CVPara(){};

	void operator=(const CVPara &src){
		lowelimit=src.lowelimit;
		highelimit=src.highelimit;
		scanrate=src.scanrate;
		combochoice=src.combochoice;
		noofcycles=src.noofcycles;
		variationtolerance=src.variationtolerance;
		rotationrate=src.rotationrate;
		endintegratione=src.endintegratione;
		startintegratione=src.startintegratione;
		samplefrequecy=src.samplefrequecy;
		quiettime=src.quiettime;
	};

	CVPara(const CVPara &src){ operator=(src); };

	virtual void Serialize(CArchive& ar){
		if (ar.IsStoring())
		{	// storing code
			ar<<lowelimit
				<<highelimit
				<<scanrate
				<<combochoice
				<<noofcycles
				<<variationtolerance
				<<rotationrate
				<<endintegratione
				<<startintegratione;
		}
		else
		{	// loading code
			ar>>lowelimit
				>>highelimit
				>>scanrate
				>>combochoice
				>>noofcycles
				>>variationtolerance
				>>rotationrate
				>>endintegratione
				>>startintegratione;
		}
	};


	double EInterval() const
	{
		return scanrate/samplefrequecy;
	};

};


