#pragma once

#include "../funT1/calgridT.h"
#include "../funT1/xRescaleT.h"
#include "../funT1/colormapT.h"
// PlotSpec command target


class PlotSpec : public CObject
{
public:

	static void DrawGridLine(const CRect & rect
		, CDC * pdc
		, int gridType
		, COLORREF gridC
		, int lineWidth
		, const std::vector<long> & gridH
		, const std::vector<long> & gridV)
	{

		//draw xy grid line
		if(gridType>=0 && gridType<5){

			CPen pen;
			CPen * pOldPen;

			pen.CreatePen(gridType, lineWidth, gridC);
			pOldPen=pdc->SelectObject(&pen);

			std::vector<DWORD> npo(gridV.size()+gridH.size(),2);
			std::vector<CPoint> gridline(npo.size()*2);

			//int offset=lineWidth/2;

			for(size_t i=0;i<gridV.size();i++){
				gridline[i*2].x=gridV[i];
				gridline[i*2].y=rect.bottom;
				gridline[i*2+1].x=gridV[i];
				gridline[i*2+1].y=rect.top;
			}
			for(size_t i=0;i<gridH.size();i++){
				gridline[(i+gridV.size())*2].x=rect.left;
				gridline[(i+gridV.size())*2].y=gridH[i];
				gridline[(i+gridV.size())*2+1].x=rect.right;
				gridline[(i+gridV.size())*2+1].y=gridH[i];
			}	
			pdc->PolyPolyline(gridline.data(),npo.data(),npo.size());
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();
		}


	};


	static CRect DrawMetric(const CRect & rect,
		CDC * pdc,
		int metricSize,
		COLORREF metricC,
		COLORREF gridC,
		int lineWidth,
		int metricGridLong,
		int metricGridShort,
		CString fontName,
		const std::vector<long> & gridH,
		const std::vector<long> & gridV,
		const std::vector<double> & gridx,
		const std::vector<double> & gridy)
	{
		//int lineWidth=1;
		CRect newrect=rect;
		int metricH=metricSize;
		int lc=metricGridLong;
		int lcs=metricGridShort;

		if(metricH>0){

			CFont font;
			CFont *pOldFont;
			CString str;
			CPen pen;
			CPen * pOldPen;
			CSize sz;

			CPoint textLocate;
			COLORREF oc;
			//CString fontName=L"Arial";
			int oldBkMode;


			pen.CreatePen(PS_SOLID, lineWidth, gridC);
			pOldPen=pdc->SelectObject(&pen);
			oc=pdc->SetTextColor(metricC);


			//draw x metric
			CRect xmrect(rect.left,rect.bottom+lc,rect.left,rect.bottom+lc);

			font.CreateFont(
				metricH,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
			pOldFont=pdc->SelectObject(&font);

			oldBkMode=pdc->SetBkMode(TRANSPARENT);
			textLocate.y=rect.bottom+lc;
			for(size_t i=0;i<gridV.size();i++){

				pdc->MoveTo(gridV[i],rect.bottom);
				pdc->LineTo(gridV[i],rect.bottom+lcs);

				str.Format(L"%g",gridx[i]);
				sz=pdc->GetTextExtent(str);
				textLocate.x=gridV[i]-sz.cx/2;

				if(xmrect.right<textLocate.x &&
					textLocate.x+sz.cx<rect.right){
						pdc->TextOutW(textLocate.x,textLocate.y,str);
						xmrect.right=textLocate.x+sz.cx;
						if(sz.cy>xmrect.Height()){
							xmrect.bottom=textLocate.y+sz.cy;
						}
						pdc->MoveTo(gridV[i],rect.bottom);
						pdc->LineTo(gridV[i],rect.bottom+lc);
				}					

			}
			newrect.bottom=xmrect.bottom;
			pdc->SetBkMode(oldBkMode);

			pdc->SelectObject(pOldFont);
			font.DeleteObject();
			////////////////////////////////////////////////////////////////////////////
			//draw y metric
			CRect ymrect(rect.left-lc,rect.bottom,rect.left-lc,rect.bottom);

			font.CreateFont(
				metricH,                        // nHeight
				0,                         // nWidth
				900,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
			pOldFont=pdc->SelectObject(&font);

			oldBkMode=pdc->SetBkMode(TRANSPARENT);
			for(size_t i=0;i<gridH.size();i++){
				pdc->MoveTo(rect.left,gridH[i]);
				pdc->LineTo(rect.left-lcs,gridH[i]);

				str.Format(L"%g",gridy[i]);
				sz=pdc->GetTextExtent(str);

				textLocate.x=rect.left-lc-sz.cy;
				textLocate.y=gridH[i]+sz.cx/2;
				if(ymrect.top>textLocate.y &&
					textLocate.y-sz.cx>rect.top){
						pdc->TextOutW(textLocate.x,textLocate.y,str);
						if(ymrect.left>textLocate.x){
							ymrect.left=textLocate.x;
						}
						ymrect.top=textLocate.y-sz.cx;

						pdc->MoveTo(rect.left,gridH[i]);
						pdc->LineTo(rect.left-lc,gridH[i]);

				}


			}

			newrect.left=ymrect.left;
			pdc->SetBkMode(oldBkMode);
			pdc->SelectObject(pOldFont);
			font.DeleteObject();
			//////////////////////////////////////////////



			pdc->SetTextColor(oc);
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();


		}


		return newrect;

	};

	static CRect DrawLabel(const CRect & rect
		, CDC * pdc
		, CPoint centerP
		, int labelSize
		, COLORREF labelC
		, CString fontName
		, const CString & xlab
		, const CString & ylab)
	{
		int labelH=labelSize;
		CRect newrect=rect;

		if(labelSize>0){

			CFont font;
			CFont *pOldFont;
			CString str;

			COLORREF oc;
			CSize sz;
			//CString fontName=L"Arial";
			int oldBkMode;

			oc=pdc->SetTextColor(labelC);

			//draw x axis label
			font.CreateFont(
				labelH,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
			//font.CreatePointFont(200,L"MS Gothic",NULL);
			pOldFont=pdc->SelectObject(&font);

			oldBkMode=pdc->SetBkMode(TRANSPARENT);

			str.Format(xlab);
			sz=pdc->GetTextExtent(str);
			pdc->TextOutW(centerP.x-(sz.cx/2),newrect.bottom,str);
			newrect.bottom+=sz.cy;

			pdc->SetBkMode(oldBkMode);

			pdc->SelectObject(pOldFont);
			font.DeleteObject();


			//draw y axis label
			font.CreateFont(
				labelH,                        // nHeight
				0,                         // nWidth
				900,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
			pOldFont=pdc->SelectObject(&font);

			oldBkMode=pdc->SetBkMode(TRANSPARENT);
			str.Format(ylab);
			sz=pdc->GetTextExtent(str);
			pdc->TextOutW(newrect.left-sz.cy-1,centerP.y+(sz.cx/2),str);
			newrect.left-=sz.cy+1;
			pdc->SetBkMode(oldBkMode);

			pdc->SelectObject(pOldFont);
			font.DeleteObject();

			pdc->SetTextColor(oc);
		}

		return newrect;
	};


	static void GetPlotRect(CRect & plotRect
		, int labelSize
		, int metricSize
		, int metricGridLong
		, int gap0
		)
	{
		//GetClientRect(&plotRect);
		//plotRect.bottom=plotRect.CenterPoint().y;

		int gap=gap0;

		if(labelSize>0){
			gap+=labelSize;
		}
		if(metricSize>0){
			gap+=metricGridLong+metricSize;
		}
		plotRect.DeflateRect(gap,gap0,gap0,gap);
	};


	static CRect DrawXYAxis(CRect rect
		, CDC * pdc
		//, const PlotSpec & psp
		//, const CString & xlab
		//, const CString & ylab
		, COLORREF bkgndC
		, COLORREF borderC
		, COLORREF gridC
		, int gridType//0~5
		,COLORREF labelC
		,int labelSize
		,COLORREF metricC
		,int metricSize	
		//,COLORREF winbkC	
		,int gap
		,int metricGridLong
		,int metricGridShort
		,CString fontName
		,CString xlabel
		,CString ylabel
		, int gridWidth
		, double xmin
		, double xmax
		, double ymin
		, double ymax)
	{
		CRect newrect=rect;

		double XMAX,XMIN,YMAX,YMIN;
		XMAX=xmax;
		XMIN=xmin;
		YMAX=ymax;
		YMIN=ymin;

		pdc->FillSolidRect(&rect,bkgndC);
		//pdc->DrawEdge(&rect,EDGE_BUMP,BF_BOTTOMLEFT);

		std::vector<double> gridx;
		calgridVT(XMAX,XMIN,gridx);
		std::vector<long> gridV(gridx.size());
		xRescaleVT(gridx.data()
			,gridx.size()
			,XMIN
			,XMAX
			,gridV.data()
			,rect.left
			,rect.right);
		std::vector<double> gridy;
		calgridVT(YMAX,YMIN,gridy);
		std::vector<long> gridH(gridy.size());
		xRescaleVT(gridy.data()
			,gridy.size()
			,YMIN
			,YMAX
			,gridH.data()
			,rect.bottom
			,rect.top);

		DrawGridLine(rect
			,pdc
			,gridType
			,gridC
			,gridWidth
			,gridH
			,gridV);

		newrect=DrawMetric(rect
			,pdc
			,metricSize
			,metricC
			,gridC
			,gridWidth
			,metricGridLong
			,metricGridShort
			,fontName
			,gridH
			,gridV
			,gridx
			,gridy);

		newrect=DrawLabel(newrect
			,pdc
			,rect.CenterPoint()
			,labelSize
			,labelC
			,fontName
			,xlabel
			,ylabel);

		return newrect;
	};




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


	void CalPlotRect(CRect & wndRect) const
	{
		//int gap0=gap;

		//if(labelSize>0){
		//	gap0+=labelSize;
		//}
		//if(metricSize>0){
		//	gap0+=metricGridLong+metricSize;
		//}
		//wndRect.DeflateRect(gap0,gap,gap,gap0);

		GetPlotRect(wndRect,  labelSize,  metricSize,  metricGridLong, gap);

	};

	CRect Draw(CRect rect
		, CDC * pdc
		, double xmin
		, double xmax
		, double ymin
		, double ymax) const
	{
		return DrawXYAxis(rect
			,pdc
			,  bkgndC
			,  borderC
			,  gridC
			,  gridType//0~5
			, labelC
			, labelSize
			, metricC
			, metricSize	
			, gap
			, metricGridLong
			, metricGridShort
			, fontName
			, xlabel
			, ylabel
			,  gridWidth
			,  xmin
			,  xmax
			,  ymin
			,  ymax);

	};

	//PlotSpec::PlotSpec(int i, COLORREF ic)
	//{
	//	PlotSpec();
	//	winbkC=ic;
	//	SetCr(i);
	//}


	int GetPlotBKCrType(void)
	{

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

	void SetPlotBKCr(int i=0)
	{
		const int ncr=3;
		const int ncritm=5;
		const COLORREF crl[]={

			RGB(255,255,255),RGB(255,255,255),RGB(0,0,0),RGB(255,255,0),RGB(255,0,0),
			RGB(200,200,200),RGB(200,200,200),RGB(20,20,20),RGB(20,20,20),RGB(20,20,20),
			RGB(60,60,60),RGB(60,60,60),RGB(150,150,150),RGB(0,0,0),RGB(0,0,0),
			RGB(0,0,0)
		};

		if(i<0 || i>=ncr)
			i=0;
		i*=ncritm;
		bkgndC=crl[i++];
		borderC=crl[i++];
		gridC=crl[i++];
	};

	void RefreshWinCr(COLORREF bkc, const int ndiv=10, const float bias=0.35)
	{
		winbkC=bkc;
		int i=GetPlotBKCrType();		
		metricC=labelC=ContractCr(winbkC,bias+(float)(i-1)/ndiv);
	}

};


