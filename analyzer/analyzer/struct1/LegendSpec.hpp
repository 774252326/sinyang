#pragma once

// LegendSpec command target

class LegendSpec : public CObject
{
public:
	bool bDock;
	int lineLength;
	int gap;
	int fontSize;
	CString fontName;
	COLORREF bkColor;
	CPoint position;

public:
	LegendSpec()
		: bDock(false)
		, lineLength(25)
		, gap(2)
		, fontSize(15)
		, fontName(_T("Arial"))
		, bkColor(RGB(255,255,255))
		, position(CPoint(0,0))
	{
	};
	virtual ~LegendSpec(){};

	void operator=(const LegendSpec &src)
	{	
		bDock=src.bDock;
		lineLength=src.lineLength;
		fontSize=src.fontSize;
		bkColor=src.bkColor;
		position=src.position;
		gap=src.gap;
		fontName=src.fontName;
	};
	LegendSpec(const LegendSpec &src){ operator=(src); };


	virtual void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{	// storing code
			ar<< bDock
				<< lineLength
				<< gap
				<< fontSize
				<< fontName
				<< bkColor
				<< position;
		}
		else
		{	// loading code
			ar >> bDock
				>> lineLength
				>> gap
				>> fontSize
				>> fontName
				>> bkColor
				>> position;
		}
	};

};


