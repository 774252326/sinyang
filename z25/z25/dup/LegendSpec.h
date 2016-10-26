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
	LegendSpec();
	virtual ~LegendSpec();

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


	virtual void Serialize(CArchive& ar);
};


