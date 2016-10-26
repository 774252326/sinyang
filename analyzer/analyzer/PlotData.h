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

	std::vector< std::vector<double> > xlist;
	std::vector< std::vector<double> > ylist;

	std::vector<double> xll;
	std::vector<double> yll;
	std::vector<DWORD> ll;
	//std::vector<LineSpec> ps;
	std::vector<plotspec> ps;
	bool CheckData(void);
	void AddNew(const std::vector<double> &x, const std::vector<double> &y, const plotspec &plotsp, const CString &xla=NULL, const CString &yla=NULL);
	void AddFollow(const std::vector<double> &x, const std::vector<double> &y);
	void clear(void);
	BOOL SaveFile(CString fp);
	BOOL ReadFile(CString fp);
	void AppendData(const PlotData & pda);
	void GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y);
	void ExtractLastCycle(double xmax, CString fp);
};


