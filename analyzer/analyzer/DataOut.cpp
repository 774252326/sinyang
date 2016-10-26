// DataOut.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "DataOut.h"


// DataOut

DataOut::DataOut()
	: stepName(_T(""))
	, addVol(0)
	, totalVol(0)
	, Q(0)
	, nQ(0)
	, Use(false)
{
}

DataOut::~DataOut()
{
}



DataOut::DataOut(const DataOut &src)
{
	stepName=src.stepName;
	addVol=src.addVol;
	totalVol=src.totalVol;
	Q=src.Q;
	nQ=src.nQ;
	Use=src.Use;
}

void DataOut::operator=(const DataOut &src)
{
	stepName=src.stepName;
	addVol=src.addVol;
	totalVol=src.totalVol;
	Q=src.Q;
	nQ=src.nQ;
	Use=src.Use;
}

// DataOut member functions


void DataOut::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar	<< stepName
			<< addVol
			<< totalVol
			<< Q
			<< nQ
			<< Use;
	}
	else
	{	// loading code
		ar	>> stepName
			>> addVol
			>> totalVol
			>> Q
			>> nQ
			>> Use;
	}
}
