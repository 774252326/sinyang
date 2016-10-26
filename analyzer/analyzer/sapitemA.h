#pragma once

// sapitemA command target

class sapitemA : public CObject
{
public:
	sapitemA();
	virtual ~sapitemA();

	void operator=(const sapitemA &src);
	sapitemA(const sapitemA &src){ operator=(src); };

	double Sconc;
	double Aconc;
	double Lconc;

	int addType;
	double volconc;
	double endRatio;
	//bool bSample;

	virtual void Serialize(CArchive& ar);
	bool isUnknownComposition(void);
	bool isStepEnd(double currentRatio,bool bLess=true);
	bool isMixedComposition(void);
};


