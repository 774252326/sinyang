#pragma once

// CVPara command target

class CVPara : public CObject
{
public:
	CVPara();
	virtual ~CVPara();
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


