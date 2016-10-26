#pragma once

// linespec command target

class LineSpec : public CObject
{
public:
	COLORREF colour;
	CString name;
	int dotSize;
	int lineType;//0~5
	int smoothLine;//0~2
	bool traceLast;
	//protected:
	int lineWidth;
	int lastWidth;

public:
	LineSpec()
		: name(_T(""))
		, dotSize(0)
		, lineType(0)
		, smoothLine(0)
		, traceLast(false)
		, colour(0)
		, lineWidth(1)
		, lastWidth(4)
	{};
	virtual ~LineSpec(){};

	void operator=(const LineSpec &src)
	{
		colour=src.colour;
		name=src.name;
		dotSize=src.dotSize;
		lineType=src.lineType;
		smoothLine=src.smoothLine;
		traceLast=src.traceLast;
		lineWidth=src.lineWidth;
		lastWidth=src.lastWidth;
	};

	LineSpec(const LineSpec &src){ operator=(src); };

	virtual void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{	// storing code
			ar<<colour
				<<name
				<<dotSize
				<<lineType
				<<smoothLine
				<<traceLast
				<<lineWidth
				<< lastWidth;
		}
		else
		{	// loading code
			ar>>colour
				>>name
				>>dotSize
				>>lineType
				>>smoothLine
				>>traceLast
				>>lineWidth
				>> lastWidth;
		}
	};

};


