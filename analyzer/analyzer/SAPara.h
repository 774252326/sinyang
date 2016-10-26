#pragma once

#include "sapitem.h"
#include <vector>
// SAPara command target

class SAPara : public CObject
{
public:
	SAPara();
	virtual ~SAPara();
	void operator=(const SAPara &src);
	SAPara(const SAPara &src){ operator=(src); };

	// VMS Volume
	double vmsvol;
	std::vector<sapitem> saplist;
	virtual void Serialize(CArchive& ar);
};


