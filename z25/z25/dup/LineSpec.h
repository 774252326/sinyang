#pragma once

// linespec command target

class LineSpec : public CObject
{
public:
	LineSpec();
	virtual ~LineSpec();

	void operator=(const LineSpec &src);
	LineSpec(const LineSpec &src){ operator=(src); };


	COLORREF colour;
	CString name;
	int dotSize;
	int lineType;//0~5
	int smoothLine;//0~2
	bool traceLast;
	virtual void Serialize(CArchive& ar);
//protected:
	int lineWidth;
	int lastWidth;
};


