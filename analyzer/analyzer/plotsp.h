#pragma once

// plotsp command target

#include "typedefine.h"

class plotsp : public CObject
{
public:
	plotsp();
	virtual ~plotsp();
	plotspec pp;
	virtual void Serialize(CArchive& ar);

	std::vector<double> xll;
	std::vector<double> yll;
};


