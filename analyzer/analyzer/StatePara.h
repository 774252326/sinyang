#pragma once

// StatePara command target

#include "struct\DataOutA.hpp"



class StatePara : public CObject
{
public:
	std::vector<DataOutA> dol;

	double Ar0;
public:
	StatePara();
	virtual ~StatePara();

	void operator=(const StatePara &src){
		Ar0=src.Ar0;
		dol.assign(src.dol.begin(),src.dol.end());
	};
	StatePara(const StatePara &src){ operator=(src); };


	double Ratio(size_t idx);
};


