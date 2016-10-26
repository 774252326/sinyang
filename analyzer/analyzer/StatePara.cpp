// StatePara.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "StatePara.h"


// StatePara

StatePara::StatePara()
	: Ar0(0)
{
}

StatePara::~StatePara()
{
}


// StatePara member functions


double StatePara::Ratio(size_t idx)
{
	if(Ar0!=0 
		&& idx<dol.size()){
			return dol[idx].ArUse()/Ar0;
	}		
	return 0;
}
