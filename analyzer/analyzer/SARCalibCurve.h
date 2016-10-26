#pragma once

// SARCalibCurve command target

#include "typedefine.h"
#include <vector>

class SARCalibCurve : public CObject
{
public:
	SARCalibCurve();
	virtual ~SARCalibCurve();

	std::vector<double> sconc;
	std::vector<double> normq;
	std::vector<DWORD> ll;
	std::vector<SCP> scl;

	virtual void Serialize(CArchive& ar);
	bool CalSARLine(double evoR, double & c1, double & c0);
	bool GetStandradCalibCurve(std::vector<double> & x, std::vector<double> & y);
};


