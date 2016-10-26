#pragma once

// PlotData command target


#include "LineSpec.hpp"
#include "PlotSpec.hpp"
#include "RawData.hpp"
#include "LineSeg.hpp"
#include <algorithm>
//#include "RawDataEx.hpp"
#include "../Resource.h"
///
/// \brief The PlotData class
///画图数据类
///
class PlotData : public ObjectF
{
public:

    ///
    /// \brief genPointToPlot
    /// 计算数据点在画图控件中的坐标
    /// \param px
    /// 横坐标数据
    /// \param py
    /// 纵坐标数据
    /// \param rect
    /// 控件画图区域
    /// \param pointlist
    /// 曲线各点在控件上的坐标
    /// \param xmin
    /// 横坐标数值下限
    /// \param xmax
    /// 横坐标数值上限
    /// \param ymin
    /// 纵坐标数值下限
    /// \param ymax
    /// 纵坐标数值上限
    ///
	static void genPointToPlot(const std::vector<double> &px
		, const std::vector<double> &py
		, const CRect &rect
		, std::vector<CPoint> &pointlist
		, double xmin
		, double xmax
		, double ymin
		, double ymax)
	{

		size_t nd=( (px.size()>py.size())?py.size():px.size() );
		double tx1=(double)rect.Width()/(double)(xmax-xmin);
		int tx2=rect.left-xmin*tx1;
		double ty1=-(double)rect.Height()/(double)(ymax-ymin);
		int ty2=rect.bottom-ymin*ty1;
		pointlist.assign(nd,CPoint(tx2,ty2));
		for(size_t i=0;i<nd;i++){
			pointlist[i]+=CPoint(px[i]*tx1,py[i]*ty1);
		}

	};

    ///
    /// \brief DrawSpline
    /// 画三次样条曲线
    /// \param lpPoints
    /// 数据点指针
    /// \param np
    /// 总点数
    /// \param lowlim
    /// 画图横坐标起点位置
    /// \param highlim
    /// 画图横坐标终点位置
    /// \param pDC
    ///

	static void DrawSpline( CPoint *lpPoints
		, int np
		, int lowlim
		, int highlim
		, CDC * pDC)
	{

		std::vector<double> x(np);
		std::vector<double> y(np);

		for(int i=0;i<np;i++){
			x[i]=lpPoints[i].x;
			y[i]=lpPoints[i].y;
		}

		std::vector<CPoint> pr;
		int j=x.front();
		if(j<=lowlim)
			j=lowlim+1;

		for(;j<=x.back() && j<highlim;j++){
			pr.push_back(CPoint(j,0));
		}
		splintA(x,y,pr);

		pDC->Polyline(pr.data(),pr.size());

	};

    ///
    /// \brief DrawCurve
    /// 画曲线
    /// \param pDC
    ///
    /// \param pointlist
    /// 数据点在画图控件中的坐标
    /// \param ll
    /// 分段记录
    /// \param ps
    /// 画线属性
    /// \param lborder
    /// 控件画图区域左边界
    /// \param rborder
    /// 控件画图区域右边界
    ///


	static void DrawCurve(CDC* pDC
		, std::vector<CPoint> &pointlist
		, const std::vector<DWORD> &ll
		, const std::vector<LineSpec> &ps
		, int lborder
		, int rborder)
	{
		//std::vector<CPoint> pointlist;
		CPen pen;
		CPen * pOldPen;
		//int lastWidth=4;
		COLORREF oc;

		//genPointToPlot(xll,yll,rect,pointlist);
		size_t si=0;
		CPoint *pp=pointlist.data();

		for(size_t j=0;j<ll.size();j++){

			if(ps[j].lineType>=0 && ps[j].lineType<5){	
				pen.CreatePen(ps[j].lineType,ps[j].lineWidth,ps[j].colour);
				pOldPen=pDC->SelectObject(&pen);
				if(ll[j]>2){
					if(ps[j].smoothLine==1){
						DrawSpline(&pp[si],ll[j],lborder,rborder,pDC);
					}
					if(ps[j].smoothLine==2){
						pDC->PolyBezier(&pp[si],ll[j]);
					}
					if(ps[j].smoothLine==0){
						pDC->Polyline(&pp[si],ll[j]);
					}
				}
				else{
					if(ll[j]==2){
						pDC->Polyline(&pp[si],ll[j]);
					}				
				}
				pDC->SelectObject(pOldPen);
				pen.DeleteObject();
			}


			if(ps[j].dotSize==1){
				for(size_t i=0;i<ll[j];i++){
					pDC->SetPixelV(pointlist[i+si],ps[j].colour);	
				}
			}
			if(ps[j].dotSize>1){
				CSize ppoc=CSize(ps[j].dotSize-1,ps[j].dotSize-1);
				CRect prect(0,0,1,1);
				prect.InflateRect(ppoc);

				oc=pDC->GetBkColor();
				for(size_t i=0;i<ll[j];i++){
					prect.MoveToXY(pointlist[i+si]-ppoc);
					//drawRectangle(prect,pDC,ps[j].colour,ps[j].colour);
					pDC->FillSolidRect(&prect,ps[j].colour);
				}
				pDC->SetBkColor(oc);
			}

			si+=ll[j];

			if(ps[j].traceLast){
				CRect prect(0,0,1,1);
				CSize ppoc=CSize(ps[j].lastWidth,ps[j].lastWidth);
				prect.InflateRect(ppoc);

				oc=pDC->GetBkColor();
				prect.MoveToXY(pointlist[si-1]-ppoc);
				//drawRectangle(prect,pDC,ps.back().colour,ps.back().colour);
				pDC->FillSolidRect(&prect,ps[j].colour);
				pDC->SetBkColor(oc);
			}



		}

		//if(ps.back().traceLast){
		//	CRect prect(0,0,1,1);
		//	CSize ppoc=CSize(ps.back().lastWidth,ps.back().lastWidth);
		//	prect.InflateRect(ppoc);

		//	oc=pDC->GetBkColor();
		//	prect.MoveToXY(pointlist.back()-ppoc);
		//	//drawRectangle(prect,pDC,ps.back().colour,ps.back().colour);
		//	pDC->FillSolidRect(&prect,ps.back().colour);
		//	pDC->SetBkColor(oc);
		//}

		//////////////////////////////fast///////////////////////////
		//genPointToPlot(xll,yll,rect,pointlist);
		//pen.CreatePen(PS_SOLID,1,clist[0]);
		//pOldPen=pDC->SelectObject(&pen);
		//pDC->PolyPolyline(pointlist.data(),ll.data(),ll.size());
		////dc.PolyDraw(pointlist.data(),styl.data(),pointlist.size());
		//pDC->SelectObject(pOldPen);
		//pen.DeleteObject();
		//////////////////////////////////////////////////////////////////


	};

    ///
    /// \brief DrawData
    /// 作图
    /// \param plotrect
    /// 控件区域
    /// \param pDC
    /// \param ps
    /// 图纸属性
    /// \param ls
    /// 画线属性
    /// \param raw
    /// 数据曲线
    /// \param xmin
    /// \param xmax
    /// \param ymin
    /// \param ymax
    ///



	static void DrawData(CRect &plotrect
		, CDC* pDC
		//, const PlotData &pd
		, const PlotSpec &ps
		, const std::vector<LineSpec> &ls
		, const RawData &raw
		, const double &xmin
		, const double &xmax
		, const double &ymin
		, const double &ymax
		)
	{

		//WaitForSingleObject(semaphoreWrite.m_hObject,INFINITE);

		pDC->FillSolidRect(plotrect,ps.winbkC);

		ps.CalPlotRect(plotrect);

		//GetPlotRect(plotrect, pd.ps.labelSize, pd.ps.metricSize, pd.ps.metricGridLong, pd.ps.gap);

		if(!plotrect.IsRectEmpty()){

			ps.Draw(plotrect
				//DrawXYAxis(plotrect
				,pDC
				//,pd.ps
				,xmin
				,xmax
				,ymin
				,ymax);

			if( !ls.empty() ){

				CRect mainrt;
				CRgn rgn;
				rgn.CreateRectRgnIndirect(&plotrect);	
				pDC->GetClipBox(&mainrt);
				pDC->SelectClipRgn(&rgn);


				std::vector<CPoint> pointlist;

				genPointToPlot(raw.xll
					,raw.yll
					,plotrect
					,pointlist
					,xmin
					,xmax
					,ymin
					,ymax);

				DrawCurve(pDC
					,pointlist
					,raw.ll
					,ls
					,plotrect.left
					,plotrect.right);


				rgn.SetRectRgn(&mainrt);
				pDC->SelectClipRgn(&rgn);
			}

			pDC->DrawEdge(&plotrect,EDGE_BUMP,BF_BOTTOMLEFT);

		}


		//ReleaseSemaphore(semaphoreWrite.m_hObject,1,NULL);
	};

public:
    ///
    /// \brief ps
    ///图纸属性
	PlotSpec ps;
    ///
    /// \brief raw
    ///数据曲线
	RawData raw;
    ///
    /// \brief ls
    ///画线属性
	std::vector<LineSpec> ls;

public:
	PlotData(){};
	PlotData(const PlotSpec &p){ps=p;};
	virtual ~PlotData(){};

	void operator=(const PlotData &src)
	{
		raw=src.raw;
		ps=src.ps;
		ls.assign(src.ls.begin(),src.ls.end());
	};

	PlotData(const PlotData &src){ operator=(src); };

	bool CheckData(void)
	{ 
		if(ls.size()==raw.ll.size())
			return raw.CheckData();
		return false;		
	}; 

	virtual void Serialize(CArchive& ar)
	{

		if (ar.IsStoring())
		{	// storing code

			if(CheckData()){
				ps.Serialize(ar);
				raw.Serialize(ar);
				for(size_t i=0;i<ls.size();i++){
					ls[i].Serialize(ar);
				}
			}
		}
		else
		{	// loading code
			ps.Serialize(ar);
			raw.Serialize(ar);

			ls.assign(raw.ll.size(),LineSpec());

			for(size_t i=0;i<ls.size();i++){
				ls[i].Serialize(ar);
			}

		}

	};


	COLORREF GetOldCr(COLORREF defaultCr=RGB(80,80,100))
	{
		return (ls.size()>1)?ls.front().colour:defaultCr;
	};
	
	COLORREF GetNewCr(COLORREF defaultCr=RGB(255,0,0))
	{
		return (ls.empty())?defaultCr:ls.back().colour;
	};

	void AddNew(const std::vector<double> &x
		, const std::vector<double> &y
		, const LineSpec &plotsp
		, const CString &xla=NULL
		, const CString &yla=NULL)
	{

		bool flg=raw.AddNew(x,y);

		if(flg){
			ls.push_back(plotsp);
			if(!xla.IsEmpty())
				ps.xlabel=xla;
			if(!yla.IsEmpty())
				ps.ylabel=yla;
		}
	};

	void Clear(void)
	{
		raw.Clear();
		ls.clear();
		if(!ps.xlabel.IsEmpty())
			ps.xlabel.Empty();
		if(!ps.ylabel.IsEmpty())
			ps.ylabel.Empty();
	};


	void AppendData(const PlotData & pda)
	{
		raw.AppendData(pda.raw);

		ls.resize(ls.size()+pda.ls.size());
		std::copy_backward(pda.ls.begin(),pda.ls.end(),ls.end());

		ps.xlabel=pda.ps.xlabel;
		ps.ylabel=pda.ps.ylabel;
	};


	void Draw(CRect &plotrect
		, CDC* pDC
		, const double &xmin
		, const double &xmax
		, const double &ymin
		, const double &ymax
		)
	{
		DrawData(plotrect, pDC, ps, ls, raw, xmin, xmax, ymin, ymax);
	};

	//bool SaveImage(CString filepath, CSize sz, CDC* pDC, double pct=0.02)
	//{

	//	CDC dcMem;   //用于缓冲作图的内存DC
	//	dcMem.CreateCompatibleDC(pDC);               //依附窗口DC创建兼容内存DC		

	//	CBitmap bmp;           //内存中承载临时图象的位图
	//	bmp.CreateCompatibleBitmap(pDC,sz.cx,sz.cy);//创建兼容位图

	//	dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

	//	double xmin,xmax,ymin,ymax;
	//	UpdateRange(raw.xll,xmin,xmax,pct,true);
	//	UpdateRange(raw.yll,ymin,ymax,pct,true);

	//	ps.labelC=ps.gridC=ps.metricC=black;
	//	ps.bkgndC=ps.borderC=ps.winbkC=white;

	//	CRect plotrect(0,0,sz.cx,sz.cy);
	//	DrawData(plotrect,&dcMem,*this,xmin,xmax,ymin,ymax);
	//	dcMem.DeleteDC(); //删除DC

	//	CImage img;
	//	img.Attach(HBITMAP(bmp));
	//	HRESULT hResult = img.Save((LPCWSTR)filepath);

	//	bmp.DeleteObject(); //删除位图

	//	if (SUCCEEDED(hResult))
	//		return true;

	//	return false;
	//};


    ///
    /// \brief SetLineData
    /// 设置数据曲线和曲线名
    /// \param newraw
    /// 数据曲线
    /// \param namelist
    /// 曲线名
    /// \return
    ///增加的分段数量
	int SetLineData( const RawData &newraw, const std::vector<CString> &namelist )
	{
		int ndiff=(int)newraw.ll.size()-(int)raw.ll.size();

		ls.resize(newraw.ll.size());

		for(size_t i=0;i<ls.size()&&i<namelist.size();i++){
			ls[i].name=namelist[i];
		}

		raw=newraw;

		return ndiff;
	};
    ///
    /// \brief AddLineData
    /// 增加若干数据曲线和曲线名
    /// \param newraw
    /// 数据曲线
    /// \param namelist
    /// 曲线名
    /// \return
    /// 增加的分段数量
    ///
	int AddLineData( const RawData &newraw, const std::vector<CString> &namelist )
	{	

		size_t oldn=raw.ll.size();

		raw.AppendData(newraw);

		ls.resize(raw.ll.size());

		for(size_t i=0;i+oldn<ls.size()&&i<namelist.size();i++){
			ls[i+oldn].name=namelist[i];
		}
		return newraw.ll.size();
	};

    ///
    /// \brief ResetLineType
    ///重设画线属性

	void ResetLineType()
	{
		CString ptstr;
		ptstr.LoadStringW(IDS_STRING_TEST_POINT);
		CString listr;
		listr.LoadStringW(IDS_STRING_FITTING_LINE);
		CString iqstr;
		iqstr.LoadStringW(IDS_STRING_INTERCEPT_Q);
		for(size_t i=0;i<ls.size();i++){
			if(ls[i].name==ptstr || ls[i].name==iqstr){
				ls[i].dotSize=0;
				ls[i].lineType=2;
				ls[i].smoothLine=0;
				ls[i].traceLast=false;
				continue;
			}

			if(ls[i].name==listr){
				ls[i].dotSize=0;
				ls[i].lineType=0;
				ls[i].smoothLine=0;
				ls[i].traceLast=false;
				if(i>0){
					if(!(ls[i-1].name==listr || ls[i-1].name==ptstr)){
						ls[i-1].lineType=5;
					}
				}
				continue;
			}

			ls[i].dotSize=3;
			ls[i].lineType=0;
			ls[i].smoothLine=1;
			ls[i].traceLast=false;

		}			


	};


    ///
    /// \brief SetLineColor
    /// 设置画线属性
    /// \param lastN
    /// 更改最后lastN个分段的曲线
    /// \param dotSize
    /// 点大小
    /// \param smoothType
    /// 连线类型
    /// \param lineType
    /// 线型
    ///
	void SetLineColor(int lastN, int dotSize=0, int smoothType=0, int lineType=0)
	{	
		size_t i=0;
		if(lastN>=0){
			size_t ulastN=(size_t)lastN;
			if(ulastN<ls.size())
				i=ls.size()-ulastN;
			for(;i<ls.size();i++){
				ls[i].colour=genColor(genColorvFromIndex<float>(i));
				ls[i].dotSize=dotSize;
				ls[i].lineType=lineType;
				ls[i].smoothLine=smoothType;
			}

		}


	};
    ///
    /// \brief SetLineColor
    /// 设置画线属性
    /// \param lastN
    /// 更改最后lastN个分段的曲线
    /// \param c1
    /// 最后一个分段的颜色
    /// \param c0
    /// 其余分段的颜色
    /// \param dotSize
    /// \param smoothType
    /// \param lineType
    ///
	void SetLineColor(int lastN, COLORREF c1, COLORREF c0, int dotSize=0, int smoothType=0, int lineType=0)
	{
		size_t lss=ls.size();
		if(lss<2){
			if(lastN>0 && lss>0){
				ls.back().colour=c1;

				ls.back().dotSize=dotSize;
				ls.back().lineType=lineType;
				ls.back().smoothLine=smoothType;
			}
			return;
		}

		//size_t si=(lastN<lss)?lss-lastN:0;

		size_t si=0;
		if(lastN>=0){
			size_t ulastN=(size_t)lastN;
			if(ulastN<lss)
				si=lss-ulastN;

			float rgb1[3];
			float rgb0[3];

			Getrgb(c1,rgb1);
			Getrgb(c0,rgb0);

			//float rgbi[3];		

			//for(size_t i=si;i<lss;i++){

			//	rgbi[0]=xRescale(i,si,lss-1,rgb0[0],rgb1[0]);
			//	rgbi[1]=xRescale(i,si,lss-1,rgb0[1],rgb1[1]);
			//	rgbi[2]=xRescale(i,si,lss-1,rgb0[2],rgb1[2]);

			//	ls[i].colour=GetRGB(rgbi);

			//	ls[i].dotSize=dotSize;
			//	ls[i].lineType=lineType;
			//	ls[i].smoothLine=smoothType;
			//}

			//////////////////////////////////////////////
			for(size_t i=si;i<lss-1;i++){

				//rgbi[0]=xRescale(i,si,lss-1,rgb0[0],rgb1[0]);
				//rgbi[1]=xRescale(i,si,lss-1,rgb0[1],rgb1[1]);
				//rgbi[2]=xRescale(i,si,lss-1,rgb0[2],rgb1[2]);

				//ls[i].colour=GetRGB(rgbi);


				ls[i].colour=c0;
				ls[i].dotSize=dotSize;
				ls[i].lineType=lineType;
				ls[i].smoothLine=smoothType;
			}

			ls[lss-1].colour=c1;
			ls[lss-1].dotSize=dotSize;
			ls[lss-1].lineType=lineType;
			ls[lss-1].smoothLine=smoothType;

		}



	};


	void AddPoint(double x, double y, CString name, int lineType=2)
	{
		RawData tmp;
		tmp.AddPointMark(x,y,0,(raw.yll.empty()?0:raw.yll.back()));

		std::vector<CString> namelist(1,name);
		int newn=AddLineData(tmp,namelist);
		SetLineColor(newn,(int)0,(int)0,lineType);
	};

	void AddPoint(Point2d pt, CString name, int lineType=2)
	{
		AddPoint(pt.x,pt.y,name,lineType);
	};


	void AddLine(LineSeg lis, CString name, int lineType=0)
	{
		RawData tmp;
		tmp.AddLineSeg(lis);

		std::vector<CString> namelist(1,name);
		int newn=AddLineData(tmp,namelist);
		SetLineColor(newn,(int)0,(int)0,lineType);
	};

	void AddLine(double x1, double x2, double k, double b, CString name, int lineType=0)
	{
		LineSeg lis;
		lis.Set(k,b,x1,x2);
		AddLine(lis, name, lineType);
	};

    ///
    /// \brief ShowLastLast
    ///
    /// \param flg
    /// 为真时凸出显示最后一段的最后一点
    /// 否则不显示
    ///

	void ShowLastLast(bool flg=true)
	{
		for(size_t i=0;i<ls.size();i++){
			ls[i].traceLast=false;
		}
		if( flg && !ls.empty() ){
			ls.back().traceLast=true;
		}
	};

};


