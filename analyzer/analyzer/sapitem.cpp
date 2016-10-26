// sapitem.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "sapitem.h"


// sapitem

sapitem::sapitem()
	: Sconc(0)
	, Aconc(0)
	, Lconc(0)
	, addType(0)
	, volconc(0)
{
}

sapitem::~sapitem()
{
}

sapitem::sapitem(const sapitem &src)
{
	Sconc=src.Sconc;
	Aconc=src.Aconc;
	Lconc=src.Lconc;
	addType=src.addType;
	volconc=src.volconc;	
}

void sapitem::operator=(const sapitem &src)
{
	Sconc=src.Sconc;
	Aconc=src.Aconc;
	Lconc=src.Lconc;
	addType=src.addType;
	volconc=src.volconc;	
}


// sapitem member functions


void sapitem::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<Sconc
			<<Aconc
			<<Lconc
			<<addType
			<<volconc;
	}
	else
	{	// loading code
		ar>>Sconc
			>>Aconc
			>>Lconc
			>>addType
			>>volconc;
	}
}
