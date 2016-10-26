// SAPara.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "SAPara.h"


// SAPara

SAPara::SAPara()
	: vmsvol(0)
{
}

SAPara::~SAPara()
{
}


// SAPara member functions


void SAPara::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<vmsvol
			<<saplist.size();

		for(size_t i=0;i<saplist.size();i++){
			ar<<saplist[i].Aconc
				<<saplist[i].addtype
				<<saplist[i].Lconc
				<<saplist[i].Sconc
				<<saplist[i].volconc;
		}




	}
	else
	{	// loading code
		size_t nrow;
		ar>>vmsvol
			>>nrow;

		sapitem onerow;
		saplist.assign(nrow,onerow);

		for(size_t i=0;i<nrow;i++){
			ar>>saplist[i].Aconc
				>>saplist[i].addtype
				>>saplist[i].Lconc
				>>saplist[i].Sconc
				>>saplist[i].volconc;
		}

	}
}
