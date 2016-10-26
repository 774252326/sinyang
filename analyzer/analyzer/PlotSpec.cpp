// PlotSpec.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotSpec.h"
#include "colormapT.h"

const int ncr=3;
const int ncritm=5;
const COLORREF crl[]={

	RGB(255,255,255),RGB(255,255,255),RGB(0,0,0),RGB(0,0,0),RGB(0,0,0),
	RGB(200,200,200),RGB(200,200,200),RGB(100,100,100),RGB(100,100,100),RGB(100,100,100),
	RGB(60,60,60),RGB(60,60,60),RGB(120,120,120),RGB(120,120,120),RGB(120,120,120),



//	RGB(255,203,153),RGB(254,255,153),RGB(254,255,153),RGB(254,255,153),RGB(154,204,153),
//RGB(255,154,102),RGB(255,254,206),RGB(255,254,206),RGB(255,254,206),RGB(154,204,153),
//RGB(154,102,1),RGB(255,204,51),RGB(255,204,51),RGB(255,204,51),RGB(255,254,206),
//RGB(153,203,52),RGB(254,153,0),RGB(254,153,0),RGB(254,153,0),RGB(255,204,0),
//RGB(255,153,53),RGB(255,254,206),RGB(255,254,206),RGB(255,254,206),RGB(0,152,101),
//RGB(255,154,102),RGB(154,102,1),RGB(154,102,1),RGB(154,102,1),RGB(205,204,2),
//RGB(255,203,153),RGB(204,255,154),RGB(204,255,154),RGB(204,255,154),RGB(204,204,204),
//RGB(254,153,0),RGB(255,254,206),RGB(255,254,206),RGB(255,254,206),RGB(52,104,154),
//RGB(255,254,206),RGB(205,153,52),RGB(205,153,52),RGB(205,153,52),RGB(54,102,104),
//RGB(255,153,53),RGB(153,203,52),RGB(153,203,52),RGB(153,203,52),RGB(202,102,152),
//RGB(255,102,0),RGB(253,255,103),RGB(253,255,103),RGB(253,255,103),RGB(0,152,101),
//RGB(204,102,2),RGB(153,153,153),RGB(153,153,153),RGB(153,153,153),RGB(202,205,52),
//RGB(255,203,153),RGB(255,254,206),RGB(255,254,206),RGB(255,254,206),RGB(153,205,255),
//RGB(202,205,52),RGB(254,255,153),RGB(254,255,153),RGB(254,255,153),RGB(205,153,52),
//RGB(254,153,0),RGB(255,255,1),RGB(255,255,1),RGB(255,255,1),RGB(1,153,203),
//RGB(255,153,53),RGB(255,255,1),RGB(255,255,1),RGB(255,255,1),RGB(50,102,204),
//RGB(153,1,52),RGB(204,254,103),RGB(204,254,103),RGB(204,254,103),RGB(254,153,0),
//RGB(204,102,2),RGB(202,205,52),RGB(202,205,52),RGB(202,205,52),RGB(52,104,154),
//
//
//	RGB(255,254,206),RGB(206,254,254),RGB(206,254,254),RGB(206,254,254),RGB(254,204,203),
//	RGB(203,153,152),RGB(255,254,206),RGB(255,254,206),RGB(255,254,206),RGB(204,203,155),
//	RGB(255,154,102),RGB(255,102,104),RGB(255,102,104),RGB(255,102,104),RGB(254,204,203),
//	RGB(205,51,53),RGB(204,204,204),RGB(204,204,204),RGB(204,204,204),RGB(1,53,103),
//	RGB(255,102,104),RGB(255,255,1),RGB(255,255,1),RGB(255,255,1),RGB(0,102,202),
//	RGB(154,51,52),RGB(204,152,102),RGB(204,152,102),RGB(204,152,102),RGB(2,52,1),
//	RGB(155,203,205),RGB(255,203,153),RGB(255,203,153),RGB(255,203,153),RGB(254,204,203),
//	RGB(254,204,203),RGB(254,255,153),RGB(254,255,153),RGB(254,255,153),RGB(206,204,254),
//	RGB(204,152,102),RGB(103,103,103),RGB(103,103,103),RGB(103,103,103),RGB(203,153,152),
//	RGB(154,51,52),RGB(205,204,2),RGB(205,204,2),RGB(205,204,2),RGB(102,52,103),
//	RGB(204,0,51),RGB(52,52,52),RGB(52,52,52),RGB(52,52,52),RGB(205,204,2),
//	RGB(254,0,52),RGB(53,52,154),RGB(53,52,154),RGB(53,52,154),RGB(205,204,2),
//	RGB(255,152,153),RGB(153,103,154),RGB(153,103,154),RGB(153,103,154),RGB(254,204,203),
//	RGB(2,154,203),RGB(204,204,204),RGB(204,204,204),RGB(204,204,204),RGB(255,102,104),
//	RGB(253,103,102),RGB(253,255,103),RGB(253,255,103),RGB(253,255,103),RGB(153,204,103),
//	RGB(204,203,155),RGB(103,103,103),RGB(103,103,103),RGB(103,103,103),RGB(203,153,152),
//	RGB(53,103,52),RGB(153,1,52),RGB(153,1,52),RGB(153,1,52),RGB(255,203,153),
//
//	white,black,black,black,blue,
//	black,white,white,white,blue,
//	RGB(77,77,77),RGB(242,242,242),RGB(242,242,242),RGB(242,242,242),RGB(242,242,242),
//	RGB(239,239,239),RGB(86,86,86),RGB(86,86,86),RGB(86,86,86),RGB(86,86,86),

	//RGB(204,0,51),RGB(1,1,1),RGB(1,1,1),RGB(1,1,1),RGB(0,51,153)

	RGB(0,0,0)
};

// PlotSpec

PlotSpec::PlotSpec()
	: bkgndC(0)
	, borderC(0)
	, gridC(0)
	, gridType(5)
	, labelC(0)
	, labelSize(20)
	, metricC(0)
	, metricSize(15)
	, legendPos(2)
	, winbkC(0)
{
}

PlotSpec::~PlotSpec()
{
}



PlotSpec::PlotSpec(int i)
{
	SetCr(i);
	gridType=5;
	labelSize=20;
	metricSize=15;
	legendPos=2;
}



PlotSpec::PlotSpec(int i, COLORREF ic)
{
	//if(i<0)
	//	i=-i;
	//int k=i%3;

	//int j=k;
	////i--;
	//
	//BYTE b=ic>>16;
	//BYTE g=ic>>8;
	//BYTE r=ic;

	//BYTE gray=(r*38 + g*75 + b*15) >> 7;

	//if(gray>10){
	//	j--;
	//	if(gray>245){
	//		j--;
	//	}
	//}
	//BYTE bcg=gray+j*10;
	//BYTE fcg=(bcg>127)?0:255;

	//bkgndC=RGB(bcg,bcg,bcg);
	//borderC=RGB(bcg,bcg,bcg);
	//gridC=RGB(fcg,fcg,fcg);
	//metricC=RGB(fcg,fcg,fcg);
	//labelC=255<<(k*8);

	//gridType=5;
	//labelSize=20;
	//metricSize=15;
	//legendPos=2;

	//////////////////////////////////////////

	SetCr(i);

	gridType=5;
	labelSize=20;
	metricSize=15;
	legendPos=2;

	winbkC=ic;
	//PlotSpec(i);
}



void PlotSpec::operator=(const PlotSpec &src)
{
	bkgndC=src.bkgndC;
	borderC=src.borderC;
	gridC=src.gridC;
	gridType=src.gridType;
	labelC=src.labelC;
	labelSize=src.labelSize;
	metricC=src.metricC;
	metricSize=src.metricSize;
	legendPos=src.legendPos;
	winbkC=src.winbkC;
}

// PlotSpec member functions


void PlotSpec::Serialize(CArchive& ar)
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
			<< legendPos
			<< winbkC;

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
			>> legendPos
			>> winbkC;
	}
}


int PlotSpec::GetCrType(void)
{
	for(int i=0;i<ncr;i++){
		int j=i*ncritm;
		if(	bkgndC==crl[j++]
		&& borderC==crl[j++]
		&& gridC==crl[j++]
		&& metricC==crl[j++]
		&& labelC==crl[j++])
			return i;
	}

///////////////////////////////////
	//switch(labelC){
	//case RGB(255,0,0):
	//	return 0;
	//case RGB(0,255,0):
	//	return 1;
	//case RGB(0,0,255):
	//	return 2;
	//default:
	//	break;
	//}
	//

////////////////////////////////////////////
	return -1;
}


void PlotSpec::SetCr(int i)
{
	if(i<0 || i>=ncr)
		i=0;
	i*=ncritm;
	bkgndC=crl[i++];
	//i++;
	borderC=crl[i++];
	//i++;
	gridC=crl[i++];
	//i++;
	metricC=crl[i++];
	//i++;
	labelC=crl[i++];
}
