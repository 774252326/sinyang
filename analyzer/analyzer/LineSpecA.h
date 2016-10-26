#pragma once

// LineSpec command target

class LineSpecA : public CObject
{
	public:
	LineSpecA();
	virtual ~LineSpecA();
	virtual void Serialize(CArchive& ar);

	COLORREF colour;
	CString name;
	int dotSize;
	bool showLine;
	int smoothLine;
	bool traceLast;	

};


