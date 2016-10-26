// sapitemA.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "sapitemA.h"


// sapitemA

sapitemA::sapitemA()
	: Sconc(0)
	, Aconc(0)
	, Lconc(0)
	, addType(0)
	, volconc(0)
	, endRatio(0)
	//, bSample(true)
{
}

sapitemA::~sapitemA()
{
}

void sapitemA::operator=(const sapitemA &src)
{
	Sconc=src.Sconc;
	Aconc=src.Aconc;
	Lconc=src.Lconc;
	addType=src.addType;
	volconc=src.volconc;
	endRatio=src.endRatio;
	//bSample=src.bSample;
}

// sapitemA member functions


void sapitemA::Serialize(CArchive& ar)
{
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
}


bool sapitemA::isUnknownComposition(void)
{
	return ( Sconc==0 & Aconc==0 & Lconc==0 );
}


bool sapitemA::isStepEnd(double currentRatio, bool bLess)
{
	if(addType==1||addType==3){
		if( (currentRatio<endRatio)^bLess ){
			return false;
		}
	}
	return true;
}


bool sapitemA::isMixedComposition(void)
{
	if( (Sconc>0 && Aconc>0)
		||(Sconc>0 && Lconc>0)
		||(Lconc>0 && Aconc>0)
		){
			return true;
	}
	return false;
}
