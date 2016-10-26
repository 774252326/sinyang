#pragma once

// PlotData command target

#include <vector>
#include "linespec.h"
#include "PlotSpec.h"

class PlotData : public CObject
{
public:
	PlotData();

	void operator=(const PlotData &src);
	PlotData(const PlotData &src){ operator=(src); };

	virtual ~PlotData();
	virtual void Serialize(CArchive& ar);

	CString xlabel;
	CString ylabel;
	PlotSpec psp;

	std::vector<double> xll;
	std::vector<double> yll;
	std::vector<DWORD> ll;
	std::vector<LineSpec> ps;



	bool CheckData(void);
	void AddNew(const std::vector<double> &x, const std::vector<double> &y, const LineSpec &plotsp, const CString &xla=NULL, const CString &yla=NULL);
	void AddFollow(const std::vector<double> &x, const std::vector<double> &y);

	void clear(void);

	BOOL SaveFile(CString fp);
	BOOL ReadFile(CString fp);
	void AppendData(const PlotData & pda);
	void GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y);
	PlotData ExtractLastCycle(double xmax);
	void SetSpec(const CString & xla, const CString & yla, const PlotSpec & psp0);
};


