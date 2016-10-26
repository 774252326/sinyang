#pragma once

// PlotDataEx command target

#include "PlotData.hpp"
#include "LegendCondition.hpp"
#include "LegendSpec.hpp"


class PlotDataEx : public CObject
{
public:
	double xmin;
	double xmax;
	double ymin;
	double ymax;

	PlotData pd;
	LegendSpec lgs;
	LegendCondition lgc;

public:
	PlotDataEx()
		: xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
	{};
	PlotDataEx(const PlotSpec &p, double x1=0, double x2=1, double y1=0, double y2=1)
	{
		pd.ps=p;
		xmin=x1;
		xmax=x2;
		ymin=y1;
		ymax=y2;
	};
	virtual ~PlotDataEx(){};

	void operator=(const PlotDataEx &src)
	{
		pd=src.pd;
		lgs=src.lgs;
		lgc=src.lgc;
		xmin=src.xmin;
		xmax=src.xmax;
		ymin=src.ymin;
		ymax=src.ymax;
	};

	PlotDataEx(const PlotDataEx &src){ operator=(src); };

	virtual void Serialize(CArchive& ar)
	{

		if (ar.IsStoring())
		{	// storing code

			ar<<xmin
				<<xmax
				<<ymin
				<<ymax;
		}
		else
		{	// loading code
			ar>>xmin
				>>xmax
				>>ymin
				>>ymax;

		}

		lgs.Serialize(ar);
		lgc.Serialize(ar);
		pd.Serialize(ar);

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
};


