#pragma once
#include "../ObjectF.hpp"
// ANPara command target


//AnalysisParameters
///
/// \brief The ANPara class
///分析参数
///
class ANPara : public ObjectF
{
public:
    ///
    /// \brief GetStepCount
    /// 返回各分析方法的大步骤数目
    /// \param atype
    /// 分析方法
    /// \return
    ///大步骤数目
	static size_t GetStepCount(int atype)
	{
		switch(atype){
        case 0://清洗电极
			return 1;
        case 1://DT-测标定曲线
			return 2;
        case 2://DT-测样品
			return 2;
        case 3://LAT-测截距
			return 2;
        case 4://LAT-测样品
			return 4;
        case 5://RC-测响应曲线
			return 3;
        case 6://RC-测样品
			return 3;
        case 7://SAR-测关系曲线
			return 2;
        case 8://SAR-测样品
			return 3;
        case 9://PAL-测响应曲线
			return 2;
        case 10://PAL-测样品
			return 3;
        case 11://LRT-测响应曲线
			return 3;
        case 12://LRT-测样品
			return 4;
		default:
			return 0;
		}
	};
public:
    ///
    /// \brief analysistype
    ///分析方法
    /// 数值对应的具体方法见GetStepCount注释
	int analysistype;
    ///
    /// \brief evaluationratio
    ///评定比值
	double evaluationratio;
    ///
    /// \brief calibrationfactortype
    ///指示下拉框选中项
    /// 仅用于DT-测样品
	int calibrationfactortype;
    ///
    /// \brief calibrationfactor
    ///校正因子
    /// 仅用于DT-测样品
	double calibrationfactor;
    ///
    /// \brief interceptvalue
    ///截距
    /// 仅用于LAT-测样品
	double interceptvalue;
    ///
    /// \brief calibrationfilepath
    ///标定曲线文件路径
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


