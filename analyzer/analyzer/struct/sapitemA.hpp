#pragma once

// sapitemA command target

class sapitemA : public CObject
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
			if( (currentRatio<endRatio)^bLess ){
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

};


