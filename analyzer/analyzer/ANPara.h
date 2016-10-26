#pragma once

// ANPara command target

class ANPara : public CObject
{
public:
	ANPara();
	virtual ~ANPara();
	int analysistype;
	double evaluationratio;
	double endpointratio;
	int calibrationfactortype;
	double calibrationfactor;
	double interceptvalue;
	CString calibrationfilepath;
	virtual void Serialize(CArchive& ar);
};


