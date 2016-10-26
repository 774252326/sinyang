#pragma once

// ANPara command target


//AnalysisParameters
class ANPara : public CObject
{
public:
	ANPara();
	virtual ~ANPara();

	void operator=(const ANPara &src);
	ANPara(const ANPara &src){ operator=(src); };


	int analysistype;
	double evaluationratio;
	double endpointratio;
	int calibrationfactortype;
	double calibrationfactor;
	double interceptvalue;
	CString calibrationfilepath;
	virtual void Serialize(CArchive& ar);
};


