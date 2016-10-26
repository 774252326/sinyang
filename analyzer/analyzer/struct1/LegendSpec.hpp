#pragma once
#include "../ObjectF.hpp"

// LegendSpec command target
///
/// \brief inv
/// 反色
/// \param oc
/// \return
///
inline COLORREF inv(const COLORREF &oc){
	return (oc^0x00ffffff);
};

///
/// \brief The LegendSpec class
///图例属性

class LegendSpec : public ObjectF
{

public:
    ///
    /// \brief DrawLegend
    /// 画图例
    /// \param pDC
    ///
    /// \param ps
    /// 线的属性
    /// \param pfont
    /// 字体
    /// \param bkColor
    /// 背景颜色
    /// \param lc
    /// 线长
    /// \param gap
    /// 边沿留空
    /// \param position
    ///起始位置

	static void DrawLegend(CDC* pDC
		, const std::vector<LineSpec> &ps
		, CFont *pfont
		, COLORREF bkColor
		, int lc
		, int gap
		, CPoint position)
	{

		//int lineWidth=1;
		//int lc=25;
		//int gap=2;
		//int Hmax=15;
		//CString fontName=L"Arial";

		//CFont font;
		//CRect borderrect=GetLegendBorder(rect,pDC,ps,&font,lc,gap,Hmax,fontName,bAlignLeft,bAlignTop);

		//CRect legendrect(borderrect);
		//legendrect.DeflateRect(gap,0,gap,0);


		CFont *pOldFont;
		CString str;
		CPen pen;
		CPen * pOldPen;
		CSize sz;
		CPoint textLocate;
		COLORREF oc;

		////////////////////////////////////////////////////////

		//drawRectangle(borderrect,pDC,bkColor,inv(bkColor));

		//pDC->FillSolidRect(legendrect,bkColor);
		//pDC->FillSolidRect(borderrect,bkColor);

		textLocate=CPoint(gap+position.x,position.y);
		pOldFont=pDC->SelectObject(pfont);	
		oc=pDC->SetTextColor(inv(bkColor));

		pDC->SetBkColor(bkColor);
		pDC->SetBkMode(TRANSPARENT);

		for(size_t i=0;i<ps.size();i++){

			sz=pDC->GetTextExtent(ps[i].name);

			if(ps[i].lineType>=0){
				pen.CreatePen(ps[i].lineType
					, ps[i].lineWidth
					, ps[i].colour);
				pOldPen=pDC->SelectObject(&pen);
				pDC->MoveTo(textLocate.x
					,textLocate.y+sz.cy/2);
				pDC->LineTo(textLocate.x+lc
					,textLocate.y+sz.cy/2);
				pDC->SelectObject(pOldPen);
				pen.DeleteObject();
			}

			if(ps[i].dotSize==1){
				pDC->SetPixelV(textLocate.x+lc/2
					,textLocate.y+sz.cy/2
					,ps[i].colour);
			}

			if(ps[i].dotSize>1){
				CRect prect(0,0,1,1);
				prect.InflateRect(ps[i].dotSize-1,ps[i].dotSize-1);
				prect.MoveToXY(textLocate.x+lc/2-ps[i].dotSize+1
					,textLocate.y+sz.cy/2-ps[i].dotSize+1);
				pDC->FillSolidRect(&prect,ps[i].colour);
			}

			pDC->TextOutW(textLocate.x+lc+gap
				,textLocate.y
				,ps[i].name);

			textLocate.y+=sz.cy;

		}

		pDC->SetBkColor(bkColor);
		pDC->SetTextColor(oc);
		pDC->SelectObject(pOldFont);
		//font.DeleteObject();

		//return borderrect;
	};

    ///
    /// \brief DrawLegend
    /// 画图例
    /// \param pDC
    /// \param ls
    /// 线的属性
    /// \param lineLength
    /// 线长
    /// \param gap
    /// 边沿留空宽度
    /// \param fontSize
    /// 字体大小
    /// \param fontName
    /// 字体名
    /// \param bkColor
    /// 底色
    /// \param position
    /// 起始位置
    ///
	static void DrawLegend(CDC* pDC
		, const std::vector<LineSpec> &ls
		, int lineLength
		, int gap
		,int fontSize
		,CString fontName
		,COLORREF bkColor
		, CPoint position)
	{
		CFont font;
		font.CreateFont(
			fontSize,                   // nHeight
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


		DrawLegend(pDC
			, ls
			, &font
			, bkColor
			, lineLength
			, gap
			, position);

	};


    ///
    /// \brief GetLegendTextExtent
    /// 计算图例文字部分占用大小
    /// \param pDC
    /// \param ps
    /// 曲线属性
    /// \param pfont
    /// 字体
    /// \return
    ///
	static CSize GetLegendTextExtent(CDC* pDC
		, const std::vector<LineSpec> &ps
		, CFont* pfont)
	{

		CSize rectsz(0,0);
		CFont *pOldFont;
		pOldFont=pDC->SelectObject(pfont);	


		for(size_t i=0;i<ps.size();i++){
			CSize sz=pDC->GetTextExtent(ps[i].name);
			if(sz.cx>rectsz.cx)
				rectsz.cx=sz.cx;
			rectsz.cy+=sz.cy;
		}

		pDC->SelectObject(pOldFont);

		return rectsz;
	};

    ///
    /// \brief GetLegendExtent
    /// 计算画图例所需画板大小
    /// \param pDC
    /// \param ps
    /// 曲线属性
    /// \param pfont
    /// 字体
    /// \param lc
    /// 线长
    /// \param gap
    /// 边沿留空宽度
    /// \param metricH
    /// 字体高度
    /// \param fontName
    /// 字体名
    /// \return
    ///
	static CSize GetLegendExtent(CDC* pDC
		, const std::vector<LineSpec> &ps
		, CFont* pfont
		, int lc
		, int gap
		, int metricH
		, CString fontName
		)
	{
		pfont->DeleteObject();

		pfont->CreateFont(
			metricH,                   // nHeight
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

		CSize LTsz=GetLegendTextExtent(pDC,ps,pfont);

		LTsz.cx+=lc+gap*3;

		return LTsz;

	};

    ///
    /// \brief GetAutoFontSize
    /// 自动调整字体大小
    /// \param pDC
    /// \param ls
    /// 曲线属性
    /// \param lineLength
    /// 线长
    /// \param gap
    /// 边沿留空宽度
    /// \param minSize
    /// 字体大小下限
    /// \param maxSize
    /// 字体大小上限
    /// \param fontName
    /// 字体名
    /// \param borderSize
    /// 限框
    /// \return
    ///字体高度
	static int GetAutoFontSize(CDC* pDC
		, const std::vector<LineSpec> &ls
		//, CFont* pfont
		, int lineLength
		, int gap
		, int minSize
		, int maxSize
		, CString fontName
		, CSize borderSize
		)
	{

		CFont font;
		int uf=maxSize;

		CSize szu=GetLegendExtent(pDC,ls,&font,lineLength,gap,uf,fontName);
		if(szu.cx<=borderSize.cx && szu.cy<=borderSize.cy){
			TRACE("\nmax\n");
			return maxSize;
		}

		int lf=minSize;
		CSize szl=GetLegendExtent(pDC,ls,&font,lineLength,gap,lf,fontName);
		if(szl.cx>borderSize.cx || szl.cy>borderSize.cy){
			TRACE("\nmin\n");
			return minSize;
		}

		while(uf-lf>1){
			int tf=(uf+lf)/2;
			CSize szt=GetLegendExtent(pDC,ls,&font,lineLength,gap,tf,fontName);
			if(szt.cx<=borderSize.cx && szt.cy<=borderSize.cy){
				lf=tf;
				szl=szt;
			}
			else{
				uf=tf;
			}
		}

		return lf;

	};



public:
    ///
    /// \brief bDock
    ///为真
    /// 图例可用鼠标移动
    /// 否则不能移动
    ///
	bool bDock;
    ///
    /// \brief lineLength
    ///线长
	int lineLength;
    ///
    /// \brief gap
    ///边沿留空宽度
	int gap;
    ///
    /// \brief fontSize
    ///字体大小
	int fontSize;
    ///
    /// \brief fontName
    ///字体名
	CString fontName;
    ///
    /// \brief bkColor
    ///底色
	COLORREF bkColor;
    ///
    /// \brief position
    ///图例起始位置
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

	void Draw(CDC* pDC
		, const std::vector<LineSpec> &ls
		, CPoint pos=CPoint(0,0))
	{
		DrawLegend(pDC, ls, lineLength, gap, fontSize, fontName, bkColor, pos);
	};

	CSize GetExtent(CDC* pDC
		, const std::vector<LineSpec> &ps
		)
	{
		CFont font;
		return GetLegendExtent( pDC	, ps, &font, lineLength, gap, fontSize, fontName);
	};

	int AutoFontSize(CDC* pDC
		, const std::vector<LineSpec> &ls
		, CSize borderSize
		, int maxSize
		, int minSize
		)
	{
		return GetAutoFontSize( pDC
			, ls
			, lineLength
			, gap
			, minSize
			, maxSize
			, fontName
			, borderSize
			);
	};


};


