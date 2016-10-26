#pragma once

// CVPara command target

//CVParameters

class CVPara : public CObject
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

	CVPara()
		: lowelimit(-0.23)
		, highelimit(1.57)
		, scanrate(0.1)
		, combochoice(0)
		, noofcycles(4)
		, variationtolerance(0.01)
		, rotationrate(2000)
		, endintegratione(1)
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
				<<endintegratione;
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
				>>endintegratione;
		}
	};
};


