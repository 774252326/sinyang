// sapitemF.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "sapitemF.h"


// sapitemF

sapitemF::sapitemF()
	: file(_T(""))
{
}

sapitemF::~sapitemF()
{
}


void sapitemF::operator=(const sapitemF &src)
{
	sia=src.sia;
	file=src.file;
}

// sapitemF member functions
