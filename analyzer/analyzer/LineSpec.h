#pragma once

// LineSpec command target

class LineSpec : public CObject
{
	public:
	LineSpec();
	virtual ~LineSpec();
	virtual void Serialize(CArchive& ar);

	COLORREF colour;
	CString name;
	int dotSize;
	bool showLine;
	int smoothLine;
	bool traceLast;	

};


