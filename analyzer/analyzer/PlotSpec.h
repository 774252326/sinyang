#pragma once

// PlotSpec command target

class PlotSpec : public CObject
{
public:
	PlotSpec();
	virtual ~PlotSpec();

	void operator=(const PlotSpec &src);
	PlotSpec(const PlotSpec &src){ operator=(src); };

	PlotSpec(int i);


	COLORREF bkgndC;
	COLORREF borderC;

	COLORREF gridC;
	int gridType;//0~5

	COLORREF labelC;
	int labelSize;

	COLORREF metricC;
	int metricSize;
	virtual void Serialize(CArchive& ar);
};


