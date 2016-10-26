#pragma once
#include "ANPara.hpp"
#include <vector>
#include "../resource.h"
// ANParaEx command target


//AnalysisParameters
class ANParaEx : public ANPara
{
public:
	static size_t CorrectionNumber(int atype)
	{
		switch(atype){
		//case 1:
		case 2:
			return 3;
		case 4:
			return 5;
		case 6:
			return 3;
		case 8:
			return 7;
		case 10:
			return 4;
		case 12:
			return 6;
		default:
			return 0;
		}
	};
			
	static UINT GetFormulaImgID(int atype)
{
switch(atype){
	case 2:
		return IDB_BITMAP14;
	case 4:
		return IDB_BITMAP15;
	case 6:
		return IDB_BITMAP18;
	case 8:
		return IDB_BITMAP19;
	case 10:
		return IDB_BITMAP17;
	case 12:
		return IDB_BITMAP16;
	default:
		return IDB_BITMAP13;
	}

	return 0;
};

public:
	std::vector<double> correction;

	size_t CorrectionNumber() const{ return CorrectionNumber(analysistype); };
	UINT GetFormulaImgID() const{ return GetFormulaImgID(analysistype); };
	void ResetCorrection(){ correction.resize(CorrectionNumber(),1); };

	ANParaEx()
		: ANPara()
	{
		//correction.resize();
	};
	virtual ~ANParaEx(){};

	void operator=(const ANPara &src)
	{
		ANPara::operator=(src);
		ResetCorrection();
	};

	void operator=(const ANParaEx &src)
	{
		ANPara::operator=(src);
		correction.assign(src.correction.begin(),src.correction.end());
	};

	ANParaEx(const ANParaEx &src){ operator=(src); };


	virtual void Serialize(CArchive& ar)
	{
		ANPara::Serialize(ar);
		if (ar.IsStoring())
		{	// storing code
			correction.resize(CorrectionNumber());
			for(size_t i=0;i<CorrectionNumber();i++){
				ar<<correction[i];
			}
		}
		else
		{	// loading code
			correction.resize(CorrectionNumber());
			for(size_t i=0;i<CorrectionNumber();i++){
				ar>>correction[i];
			}
		}
	};



};


