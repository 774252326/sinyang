#pragma once
#include "../ObjectF.hpp"
// ANPara command target


//AnalysisParameters
class ANPara : public ObjectF
{
public:
	static size_t GetStepCount(int atype)
	{
		switch(atype){
		case 1:
			return 2;
		case 2:
			return 2;
		case 3:
			return 2;
		case 4:
			return 4;
		case 5:
			return 3;
		case 6:
			return 3;
		case 7:
			return 2;
		case 8:
			return 3;
		case 9:
			return 2;
		case 10:
			return 3;
		case 11:
			return 3;
		case 12:
			return 4;
		default:
			return 0;
		}
	};
public:
	int analysistype;
	double evaluationratio;
	int calibrationfactortype;
	double calibrationfactor;
	double interceptvalue;
	CString calibrationfilepath;

	ANPara()
		: analysistype(0)
		, evaluationratio(0.8)
		, calibrationfactortype(0)
		, calibrationfactor(1)
		, interceptvalue(1)
		, calibrationfilepath(_T(""))
	{
	};
	virtual ~ANPara(){};

	void operator=(const ANPara &src){
		analysistype=src.analysistype;
		evaluationratio=src.evaluationratio;
		calibrationfactortype=src.calibrationfactortype;
		calibrationfactor=src.calibrationfactor;
		interceptvalue=src.interceptvalue;
		calibrationfilepath=src.calibrationfilepath;
	};
	ANPara(const ANPara &src){ operator=(src); };


	virtual void Serialize(CArchive& ar){
		if (ar.IsStoring())
		{	// storing code
			ar<<analysistype
				<<evaluationratio
				<<calibrationfactortype
				<<calibrationfactor
				<<calibrationfilepath
				<<interceptvalue;
		}
		else
		{	// loading code
			ar>>analysistype
				>>evaluationratio
				>>calibrationfactortype
				>>calibrationfactor
				>>calibrationfilepath
				>>interceptvalue;
		}
	};

	size_t GetStepCount() const{ return GetStepCount(this->analysistype); };
};


