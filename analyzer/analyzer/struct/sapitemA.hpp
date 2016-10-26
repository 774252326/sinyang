#pragma once
#include "../ObjectF.hpp"
#include "../resource.h"

// sapitemA command target

class sapitemA : public ObjectF
{
public:

	double Sconc;
	double Aconc;
	double Lconc;

	int addType;
	double volconc;
	double endRatio;
	//bool bSample;


	sapitemA()
		: Sconc(0)
		, Aconc(0)
		, Lconc(0)
		, addType(0)
		, volconc(0)
		, endRatio(0)
		//, bSample(true)
	{
	};
	virtual ~sapitemA(){};

	void operator=(const sapitemA &src){
		Sconc=src.Sconc;
		Aconc=src.Aconc;
		Lconc=src.Lconc;
		addType=src.addType;
		volconc=src.volconc;
		endRatio=src.endRatio;
		//bSample=src.bSample;
	};
	sapitemA(const sapitemA &src){ operator=(src); };


	virtual void Serialize(CArchive& ar){
		if (ar.IsStoring())
		{	// storing code
			ar<<Sconc
				<<Aconc
				<<Lconc
				<<addType
				<<volconc
				<<endRatio;
		}
		else
		{	// loading code
			ar>>Sconc
				>>Aconc
				>>Lconc
				>>addType
				>>volconc
				>>endRatio;
		}
	};
	bool isUnknownComposition(void){
		if( Sconc<=0 && Aconc<=0 && Lconc<=0 )
			return true;
		return false;
	};
	bool isStepEnd(double currentRatio,bool bLess=true){
		if(addType==1||addType==3){
			if( currentRatio==endRatio || ((currentRatio<endRatio)^bLess) ){
				return false;
			}
		}
		return true;
	};
	bool isMixedComposition(void){
		if( (Sconc>0 && Aconc>0)
			||(Sconc>0 && Lconc>0)
			||(Lconc>0 && Aconc>0)
			){
				return true;
		}
		return false;
	};

	bool isSingleComposition(void){
		if(isMixedComposition()||isUnknownComposition())
			return false;
		return true;
	};
	bool SetEndRatio(double er){
		if(addType==1||addType==3){
			endRatio=er;
			return true;
		}
		return false;
	};

	sapitemA StandardStep(double volume){
		sapitemA sia(*this);
		if(sia.addType==1||sia.addType==3){
			sia.volconc=volume;
		}
		if(sia.addType!=4){
			sia.addType=0;
		}
		return sia;
	};

	CString SolutionDescription(){
		CString str=L"";
		if(addType==4){
			str.LoadStringW(IDS_STRING_VIRGIN_MAKEUP);
		}
		else{
			if(isUnknownComposition()){
				str.LoadStringW(IDS_STRING_SAMPLE);
			}
			else{
				CString strt=L"";
				if(Sconc>0){
					strt.LoadStringW(IDS_STRING_S);
					str+=strt;
				}
				
								if(Aconc>0){
					strt.LoadStringW(IDS_STRING_A);
					str+=strt;
				}
				
								if(Lconc>0){
					strt.LoadStringW(IDS_STRING_L);
					str+=strt;
				}
				
				if(isMixedComposition()){
					strt.LoadStringW(IDS_STRING_MIX);
					str+=strt;
				}
				
			}
		}
		return str;
	};

};


