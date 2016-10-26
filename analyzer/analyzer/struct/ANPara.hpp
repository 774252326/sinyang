#pragma once

// ANPara command target


//AnalysisParameters
class ANPara : public CObject
{
public:


	int analysistype;
	double evaluationratio;
	//double endpointratio;
	int calibrationfactortype;
	double calibrationfactor;
	double interceptvalue;
	CString calibrationfilepath;

	ANPara()
		: analysistype(0)
		, evaluationratio(0.8)
		//, endpointratio(1)
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
		//endpointratio=src.endpointratio;
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
				//<<endpointratio
				<<calibrationfactortype
				<<calibrationfactor
				<<calibrationfilepath
				<<interceptvalue;
		}
		else
		{	// loading code
			ar>>analysistype
				>>evaluationratio
				//>>endpointratio
				>>calibrationfactortype
				>>calibrationfactor
				>>calibrationfilepath
				>>interceptvalue;
		}
	};
};


