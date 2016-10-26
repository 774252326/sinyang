// Value.cpp : implementation file
//

#include "stdafx.h"
#include "z27.h"
#include "Value.h"


// Value

Value::Value()
	: LogoId(0)
	, raw(0)
	, correction(0)
{
}

Value::~Value()
{
}


// Value member functions


void Value::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<LogoId
			<<raw
			<<correction;
	}
	else
	{	// loading code
		ar>>LogoId
			>>raw
			>>correction;
	}
}


double Value::Output(void)
{
	return raw*correction;
}
