#pragma once

// PlotData command target


#include "LineSpec.hpp"
#include "PlotSpec.hpp"
#include "RawData.hpp"
#include <algorithm>
//#include "RawDataEx.hpp"

class PlotData : public CObject
{
public:
	
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
	PlotSpec ps;
	RawData raw;
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

	//	CDC dcMem;   //���ڻ�����ͼ���ڴ�DC
	//	dcMem.CreateCompatibleDC(pDC);               //��������DC���������ڴ�DC		

	//	CBitmap bmp;           //�ڴ��г�����ʱͼ���λͼ
	//	bmp.CreateCompatibleBitmap(pDC,sz.cx,sz.cy);//��������λͼ

	//	dcMem.SelectObject(&bmp);  	//��λͼѡ����ڴ�DC

	//	double xmin,xmax,ymin,ymax;
	//	UpdateRange(raw.xll,xmin,xmax,pct,true);
	//	UpdateRange(raw.yll,ymin,ymax,pct,true);

	//	ps.labelC=ps.gridC=ps.metricC=black;
	//	ps.bkgndC=ps.borderC=ps.winbkC=white;

	//	CRect plotrect(0,0,sz.cx,sz.cy);
	//	DrawData(plotrect,&dcMem,*this,xmin,xmax,ymin,ymax);
	//	dcMem.DeleteDC(); //ɾ��DC

	//	CImage img;
	//	img.Attach(HBITMAP(bmp));
	//	HRESULT hResult = img.Save((LPCWSTR)filepath);

	//	bmp.DeleteObject(); //ɾ��λͼ

	//	if (SUCCEEDED(hResult))
	//		return true;

	//	return false;
	//};


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




};


