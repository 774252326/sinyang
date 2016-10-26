#pragma once

// PlotSpec command target

class PlotSpec : public CObject
{
public:
	COLORREF bkgndC;
	COLORREF borderC;

	COLORREF gridC;
	int gridType;//0~5

	COLORREF labelC;
	int labelSize;

	COLORREF metricC;
	int metricSize;

	//BYTE legendPos;//0=rightbottom,1=leftbottom,2=righttop,3=lefttop
	COLORREF winbkC;	

	int gap;
	int metricGridLong;
	int metricGridShort;
	CString fontName;
	CString xlabel;
	CString ylabel;
	int gridWidth;

public:
	PlotSpec()
		: bkgndC(RGB(255,255,255))
		, borderC(RGB(255,255,255))
		, gridC(0)
		, gridType(5)
		, labelC(0)
		, labelSize(20)
		, metricC(0)
		, metricSize(20)
		//, legendPos(0)
		, winbkC(RGB(255,255,255))
		, gap(10)
		, metricGridLong(5)
		, metricGridShort(3)
		, fontName(_T("Arial"))
		, xlabel(_T(""))
		, ylabel(_T(""))	
		, gridWidth(1)
	{};
	virtual ~PlotSpec(){};

	void operator=(const PlotSpec &src)
	{	
		bkgndC=src.bkgndC;
		borderC=src.borderC;
		gridC=src.gridC;
		gridType=src.gridType;
		labelC=src.labelC;
		labelSize=src.labelSize;
		metricC=src.metricC;
		metricSize=src.metricSize;
		//legendPos=src.legendPos;
		winbkC=src.winbkC;

		gap=src.gap;
		metricGridLong=src.metricGridLong;
		metricGridShort=src.metricGridShort;
		fontName=src.fontName;
		xlabel=src.xlabel;
		ylabel=src.ylabel;
		gridWidth=src.gridWidth;
	};

	PlotSpec(const PlotSpec &src){ operator=(src); };

	virtual void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{	// storing code
			ar<< bkgndC
				<< borderC
				<< gridC
				<< gridType
				<< labelC
				<< labelSize
				<< metricC
				<< metricSize
				//<< legendPos
				<< winbkC
				<< gap
				<< metricGridLong
				<< metricGridShort
				<< fontName
				<< xlabel
				<< ylabel
				<<gridWidth;
		}
		else
		{	// loading code
			ar>> bkgndC
				>> borderC
				>> gridC
				>> gridType
				>> labelC
				>> labelSize
				>> metricC
				>> metricSize
				//>> legendPos
				>> winbkC
				>> gap
				>> metricGridLong
				>> metricGridShort
				>> fontName
				>> xlabel
				>> ylabel
				>>gridWidth;
		}
	};


	void CalPlotRect(CRect & wndRect)
	{
		int gap0=gap;

		if(labelSize>0){
			gap0+=labelSize;
		}
		if(metricSize>0){
			gap0+=metricGridLong+metricSize;
		}
		wndRect.DeflateRect(gap0,gap,gap,gap0);
	};

};


