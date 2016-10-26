#pragma once

#include "typedefine.h"
// SAPara command target

class SAPara : public CObject
{
public:
	SAPara();
	virtual ~SAPara();
	// VMS Volume
	double vmsvol;
	std::vector<sapitem> saplist;
	virtual void Serialize(CArchive& ar);
};


