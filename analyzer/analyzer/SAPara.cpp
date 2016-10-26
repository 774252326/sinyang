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

void SAPara::operator=(const SAPara &src)
{
	vmsvol=src.vmsvol;
	saplist.assign(src.saplist.begin(),src.saplist.end());
}


// SAPara member functions


void SAPara::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<vmsvol
			<<saplist.size();

		for(size_t i=0;i<saplist.size();i++){
			saplist[i].Serialize(ar);
		}
	}
	else
	{	// loading code
		size_t nrow;
		ar>>vmsvol
			>>nrow;

		saplist.assign(nrow,sapitem());

		for(size_t i=0;i<nrow;i++){
			saplist[i].Serialize(ar);
		}
	}
}
