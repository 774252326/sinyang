#pragma once

// PlotData command target

//#include <vector>
#include "typedefine.h"
//#include "LineSpec.h"
//#include "LineSpecA.h"

class PlotData : public CObject
{
public:
	PlotData();
	virtual ~PlotData();
	virtual void Serialize(CArchive& ar);

	CString xlabel,ylabel;

	//std::vector< std::vector<double> > xlist;
	//std::vector< std::vector<double> > ylist;

	std::vector<double> xll;
	std::vector<double> yll;
	std::vector<DWORD> ll;
	//std::vector<LineSpec> ps;
	std::vector<plotspec> ps;
	bool CheckData(void);
	void AddNew(const std::vector<double> &x, const std::vector<double> &y, const plotspec &plotsp, const CString &xla, const CString &yla);
	void AddFollow(const std::vector<double> &x, const std::vector<double> &y);
	void clear(void);
	void SaveFile(CString fp);
	void ReadFile(CString fp);
	void AppendData(const PlotData & pda);
};


