#pragma once

// CVPara command target

//CVParameters

class CVPara : public CObject
{
public:
	CVPara();
	virtual ~CVPara();

	void operator=(const CVPara &src);
	CVPara(const CVPara &src){ operator=(src); };

	double lowelimit;
	double highelimit;
	double scanrate;
	int combochoice;
	int noofcycles;
	double variationtolerance;
	double rotationrate;
	double endintegratione;
	virtual void Serialize(CArchive& ar);
};


