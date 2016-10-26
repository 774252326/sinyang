#pragma once

#include "../funT1/calgridT.h"
#include "../funT1/xRescaleT.h"
#include "../funT1/colormapT.h"
// PlotSpec command target
#include "../ObjectF.hpp"
///
/// \brief The PlotSpec class
///图纸属性
/// 包括坐标轴和底色等

class PlotSpec : public ObjectF
{
public:
    ///
    /// \brief DrawGridLine
    /// 画网格
    /// \param rect
    /// 作图区域
    /// \param pdc
    /// \param gridType
    /// 网格线型
    /// \param gridC
    /// 网格颜色
    /// \param lineWidth
    /// 网格线宽
    /// \param gridH
    /// 网格位置
    /// \param gridV
    ///网格位置
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

    ///
    /// \brief DrawMetric
    /// 画坐标轴刻度
    /// \param rect
    /// 图纸画线区域
    /// \param pdc
    ///
    /// \param metricSize
    /// 刻度字体大小
    /// \param metricC
    /// 刻度字体颜色
    /// \param gridC
    /// 刻度线颜色
    /// \param lineWidth
    /// 刻度线宽
    /// \param metricGridLong
    /// 长刻度线长度
    /// 对应有标数字的刻度
    /// \param metricGridShort
    /// 短刻度线长度
    /// 对应无标数字的刻度
    /// \param fontName
    /// 刻度字体名
    /// \param gridH
    /// 纵坐标轴刻度位置
    /// \param gridV
    /// 横坐标轴刻度位置
    /// \param gridx
    /// 横坐标轴刻度数值
    /// \param gridy
    /// 纵坐标轴刻度数值
    /// \return
    ///图纸画线加上画坐标轴的区域
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

    ///
    /// \brief DrawLabel
    /// 画坐标名
    /// \param rect
    /// 图纸画线加上画坐标轴的区域
    /// \param pdc
    ///
    /// \param centerP
    /// 中心点
    /// 用于定位坐标名
    /// \param labelSize
    /// 坐标名字体高度
    /// \param labelC
    /// 坐标名颜色
    /// \param fontName
    /// 坐标名字体
    /// \param xlab
    /// 横坐标名
    /// \param ylab
    /// 纵坐标名
    /// \return
    ///图纸画线加上画坐标轴再加上画坐标名的区域
    ///
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

    ///
    /// \brief GetPlotRect
    /// 计算控件中可用于画线的区域
    /// \param plotRect
    /// 输入控件可用区域
    /// 输出控件中可用于画线的区域
    /// \param labelSize
    /// 坐标名字体高度
    /// \param metricSize
    /// 刻度字体高度
    /// \param metricGridLong
    /// 长刻度线长度
    /// \param gap0
    /// 边沿留空宽度
    ///
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

    ///
    /// \brief DrawXYAxis
    /// 画坐标
    /// \param rect
    /// 控件画线区域
    /// \param pdc
    ///
    /// \param bkgndC
    /// 画线区域的底色
    /// \param borderC
    /// 没用到
    /// \param gridC
    /// 网格颜色
    /// \param gridType
    /// 网格线型
    /// \param labelC
    /// 坐标名颜色
    /// \param labelSize
    /// 坐标名字体高度
    /// \param metricC
    /// 刻度字体颜色
    /// \param metricSize
    /// 刻度字体高度
    /// \param gap
    /// 没用到
    /// \param metricGridLong
    /// 长刻度线长度
    /// \param metricGridShort
    /// 短刻度线长度
    /// \param fontName
    /// 字体名
    /// \param xlabel
    /// 横坐标名
    /// \param ylabel
    /// 纵坐标名
    /// \param gridWidth
    /// 网格线宽
    /// \param xmin
    /// 横坐标起点
    /// \param xmax
    /// 横坐标终点
    /// \param ymin
    /// 纵坐标起点
    /// \param ymax
    /// 纵坐标终点
    /// \return
    /// 图纸画线加上画坐标轴再加上画坐标名的区域
    ///
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

    ///
    /// \brief bkgndC
    ///画线区域底色

	COLORREF bkgndC;
    ///
    /// \brief borderC
    ///没用到
	COLORREF borderC;

    ///
    /// \brief gridC
    ///网格线颜色
	COLORREF gridC;
    ///
    /// \brief gridType
    ///网格线型
	int gridType;//0~5

    ///
    /// \brief labelC
    ///坐标名颜色
	COLORREF labelC;
    ///
    /// \brief labelSize
    ///坐标名字体大小
	int labelSize;

    ///
    /// \brief metricC
    ///刻度线颜色
	COLORREF metricC;
    ///
    /// \brief metricSize
    ///刻度字体大小
	int metricSize;

	//BYTE legendPos;//0=rightbottom,1=leftbottom,2=righttop,3=lefttop
    ///
    /// \brief winbkC
    ///控件底色
    COLORREF winbkC;

    ///
    /// \brief gap
    ///边沿留空宽度
	int gap;
    ///
    /// \brief metricGridLong
    ///长刻度线长度
	int metricGridLong;
    ///
    /// \brief metricGridShort
    ///短刻度线长度
	int metricGridShort;
    ///
    /// \brief fontName
    ///字体名
	CString fontName;
    ///
    /// \brief xlabel
    ///横坐标名
	CString xlabel;
    ///
    /// \brief ylabel
    ///纵坐标名
	CString ylabel;
    ///
    /// \brief gridWidth
    ///网格线宽
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

    ///
    /// \brief GetPlotBKCrType
    /// 获得当前配色类型
    /// \return
    ///
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

    ///
    /// \brief SetPlotBKCr
    /// 设置配色类型
    /// \param i
    ///
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

    ///
    /// \brief RefreshWinCr
    /// 根据配色类型和控件底色刷新配色
    /// \param bkc
    /// \param ndiv
    /// \param bias
    ///
	void RefreshWinCr(COLORREF bkc, const int ndiv=10, const float bias=0.35)
	{
		winbkC=bkc;
		int i=GetPlotBKCrType();		
		metricC=labelC=ContractCr(winbkC,bias+(float)(i-1)/ndiv);
	};

    ///
    /// \brief SetStandradCr
    ///标准配色
	void SetStandradCr()
	{
		SetPlotBKCr();
		winbkC=bkgndC;
		metricC=labelC=gridC;
	};

};


