#pragma once

// linespec command target

class LineSpec : public CObject
{
public:
	LineSpec();
	virtual ~LineSpec();
	LineSpec(const LineSpec &src);
	void operator=(const LineSpec &src);

	COLORREF colour;
	CString name;
	int dotSize;
	int lineType;//0~5
	int smoothLine;//0~2
	bool traceLast;
	virtual void Serialize(CArchive& ar);
};


