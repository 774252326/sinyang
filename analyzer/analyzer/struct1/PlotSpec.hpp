#pragma once

#include "../funT1/colormapT.h"
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



	//PlotSpec::PlotSpec(int i, COLORREF ic)
	//{
	//	PlotSpec();
	//	winbkC=ic;
	//	SetCr(i);
	//}



	int GetCrType(void)
	{

		const int ndiv=10;
		const float bias=0.35;
		const int ncr=3;
		const int ncritm=5;
		const COLORREF crl[]={

			RGB(255,255,255),RGB(255,255,255),RGB(0,0,0),RGB(255,255,0),RGB(255,0,0),
			RGB(200,200,200),RGB(200,200,200),RGB(20,20,20),RGB(20,20,20),RGB(20,20,20),
			RGB(60,60,60),RGB(60,60,60),RGB(150,150,150),RGB(0,0,0),RGB(0,0,0),
			RGB(0,0,0)
		};



		for(int i=0;i<ncr;i++){
			int j=i*ncritm;
			if(	
				bkgndC==crl[j++]
			&& borderC==crl[j++]
			&& gridC==crl[j++]
			)
				return i;
		}
		return -1;
	};

	void PlotSpec::SetCr(int i)
	{

		const int ndiv=10;
		const float bias=0.35;
		const int ncr=3;
		const int ncritm=5;
		const COLORREF crl[]={

			RGB(255,255,255),RGB(255,255,255),RGB(0,0,0),RGB(255,255,0),RGB(255,0,0),
			RGB(200,200,200),RGB(200,200,200),RGB(20,20,20),RGB(20,20,20),RGB(20,20,20),
			RGB(60,60,60),RGB(60,60,60),RGB(150,150,150),RGB(0,0,0),RGB(0,0,0),
			RGB(0,0,0)
		};


		COLORREF nc=ContractCr(winbkC,bias+(float)(i-1)/ndiv);
		metricC=labelC=nc;
		if(i<0 || i>=ncr)
			i=0;
		i*=ncritm;
		bkgndC=crl[i++];
		borderC=crl[i++];
		gridC=crl[i++];
	};

};


