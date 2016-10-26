// PlotData.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "PlotData.h"
#include <algorithm>
#include "drawfunc.h"
#include "colormapT.h"




// PlotData

PlotData::PlotData()
	//：xlabel(_T(""))
	//, ylabel(_T(""))
{
}

PlotData::~PlotData()
{
}


void PlotData::operator=(const PlotData &src)
{
	//xlabel=src.xlabel;
	//ylabel=src.ylabel;
	//xll.assign(src.xll.begin(),src.xll.end());
	//yll.assign(src.yll.begin(),src.yll.end());
	//ll.assign(src.ll.begin(),src.ll.end());
	//ps.assign(src.ps.begin(),src.ps.end());
	//psp=src.psp;

	raw=src.raw;
	ps=src.ps;
	ls.assign(src.ls.begin(),src.ls.end());

}


PlotData::PlotData(const PlotSpec &p)
{
	ps=p;
}




// PlotData member functions


void PlotData::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{	// storing code

		if(CheckData()){
			//ar<<ll.size()
			//	<<xlabel
			//	<<ylabel;
			//psp.Serialize(ar);

			//for(size_t i=0;i<ll.size();i++){
			//	ps[i].Serialize(ar);
			//	ar<<ll[i];
			//}
			//for(size_t j=0;j<xll.size();j++){
			//	ar<<xll[j]
			//	<<yll[j];
			//}

			ps.Serialize(ar);
			raw.Serialize(ar);
			for(size_t i=0;i<ls.size();i++){
				ls[i].Serialize(ar);
			}

		}



	}
	else
	{	// loading code

		//size_t nl;
		//ar>>nl
		//	>>xlabel
		//	>>ylabel;
		//psp.Serialize(ar);

		//ps.assign(nl,LineSpec());
		//ll.assign(nl,0);
		//nl=0;
		//for(size_t i=0;i<ll.size();i++){
		//	ps[i].Serialize(ar);
		//	ar>>ll[i];
		//	nl+=ll[i];
		//}
		//xll.assign(nl,0);
		//yll.assign(nl,0);
		//for(size_t j=0;j<nl;j++){
		//	ar>>xll[j]
		//	>>yll[j];
		//}

		ps.Serialize(ar);
		raw.Serialize(ar);

		ls.assign(raw.ll.size(),LineSpec());

		for(size_t i=0;i<ls.size();i++){
			ls[i].Serialize(ar);
		}

	}

}


bool PlotData::CheckData(void)
{
	//if(ps.size()==ll.size() && xll.size()==yll.size()){
	//	size_t k=xll.size();
	//	for(size_t i=0;i<ll.size();i++){
	//		k-=ll[i];
	//	}
	//	if(k==0){
	//		return true;
	//	}
	//}

	if(ls.size()==raw.ll.size() && raw.CheckData())
		return true;

	return false;
}


void PlotData::AddNew(const std::vector<double> &x, const std::vector<double> &y, const LineSpec &plotsp, const CString &xla, const CString &yla)
{
	//if(x.size()!=y.size())
	//	return;

	//xll.resize(xll.size()+x.size());
	//std::copy_backward(x.begin(),x.end(),xll.end());
	//yll.resize(yll.size()+y.size());
	//std::copy_backward(y.begin(),y.end(),yll.end());
	//ll.push_back(x.size());

	bool flg=raw.AddNew(x,y);

	if(flg){

	ls.push_back(plotsp);

	if(!xla.IsEmpty())
		ps.xlabel=xla;
	if(!yla.IsEmpty())
		ps.ylabel=yla;

	}
}


//void PlotData::AddFollow(const std::vector<double> &x, const std::vector<double> &y)
//{
	//if(x.size()!=y.size())
	//	return;

	//xll.resize(xll.size()+x.size());
	//std::copy_backward(x.begin(),x.end(),xll.end());
	//yll.resize(yll.size()+y.size());
	//std::copy_backward(y.begin(),y.end(),yll.end());
	//ll.back()+=x.size();

	//bool flg=raw.AddFollow(x,y);

//}


void PlotData::clear(void)
{
	//xll.clear();
	//yll.clear();

	//ll.clear();
	//ps.clear();
	//
	//if(!xlabel.IsEmpty())
	//	xlabel.Empty();
	//if(!ylabel.IsEmpty())
	//	ylabel.Empty();


	raw.Clear();
	ls.clear();
	if(!ps.xlabel.IsEmpty())
		ps.xlabel.Empty();
	if(!ps.ylabel.IsEmpty())
		ps.ylabel.Empty();

}


//BOOL PlotData::SaveFile(CString fp)
//{
//	CFile theFile;
//	if( theFile.Open(fp, CFile::modeCreate | CFile::modeWrite) ){
//		CArchive archive(&theFile, CArchive::store);
//
//		//archive<<(size_t)1;
//		Serialize(archive);
//
//		archive.Close();
//		theFile.Close();
//		return TRUE;
//	}
//	return FALSE;
//}
//
//
//BOOL PlotData::ReadFile(CString fp)
//{
//	CFile theFile;
//	if( theFile.Open(fp, /*CFile::modeCreate |*/ CFile::modeRead) ){
//		CArchive archive(&theFile, CArchive::load);
//		//size_t n;
//		//archive >> n; 
//		Serialize(archive);
//		archive.Close();
//		theFile.Close();
//		return TRUE;
//	}
//	return FALSE;
//}

void PlotData::AppendData(const PlotData & pda)
{
	//xll.resize(xll.size()+pda.xll.size());
	//std::copy_backward(pda.xll.begin(),pda.xll.end(),xll.end());
	//yll.resize(yll.size()+pda.yll.size());
	//std::copy_backward(pda.yll.begin(),pda.yll.end(),yll.end());
	//ll.resize(ll.size()+pda.ll.size());
	//std::copy_backward(pda.ll.begin(),pda.ll.end(),ll.end());

	raw.AppendData(pda.raw);

	ls.resize(ls.size()+pda.ls.size());
	std::copy_backward(pda.ls.begin(),pda.ls.end(),ls.end());

	ps.xlabel=pda.ps.xlabel;
	ps.ylabel=pda.ps.ylabel;


}

//void PlotData::GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y) const 
//{
	//if(index<0 || index>=ll.size()){
//		x.clear();
//		y.clear();
//		return;
//	}
//
//	size_t starti=0;
//	size_t endi;
//	for(size_t i=0;i<index;i++){
//		starti+=ll[i];
//	}
//	endi=starti+ll[index];
//
//	x.assign(xll.begin()+starti, xll.begin()+endi);
//	y.assign(yll.begin()+starti, yll.begin()+endi);
//}


//PlotData PlotData::ExtractLastCycle(double xmax)
//{
//
//	PlotData p1;
//
//	p1.xlabel=xlabel;
//	p1.ylabel=ylabel;
//	p1.ps.assign(ps.begin(),ps.end());
//	p1.psp=psp;
//
//	std::vector<double> xseg(1,xmax);
//
//	for(size_t i=0;i<ll.size();i++){
//		std::vector<double> x;
//		std::vector<double> y;
//		GetDatai(i,x,y);
//
//		std::vector<double>::iterator itb;
//		itb = std::find_end (x.begin(), x.end(), xseg.begin(), xseg.end());
//
//		if(itb!=x.end()){
//
//			size_t ib=itb-x.begin();
//			size_t tailLen=x.end()-itb;
//
//			std::vector<double>::iterator ita;
//			ita = std::find_end (x.begin(), itb, xseg.begin(), xseg.end());
//			if(ita!=itb){
//				size_t ia=ita-x.begin();
//
//				size_t cycleLen=ib-ia;
//
//				if(tailLen>=cycleLen){
//					p1.xll.resize(p1.xll.size()+tailLen);
//					std::copy_backward(x.begin()+ib,x.end(),p1.xll.end());
//					p1.yll.resize(p1.yll.size()+tailLen);
//					std::copy_backward(y.begin()+ib,y.end(),p1.yll.end());
//					p1.ll.push_back(tailLen);
//				}
//				else{
//					p1.xll.resize(p1.xll.size()+cycleLen);
//					std::copy_backward(x.begin()+ia,x.begin()+ib,p1.xll.end());
//					p1.yll.resize(p1.yll.size()+cycleLen);
//					std::copy_backward(y.begin()+ia,y.begin()+ib,p1.yll.end());
//					p1.ll.push_back(cycleLen);
//				}
//			}
//			else{
//				p1.xll.resize(p1.xll.size()+tailLen);
//				std::copy_backward(x.begin()+ib,x.end(),p1.xll.end());
//				p1.yll.resize(p1.yll.size()+tailLen);
//				std::copy_backward(y.begin()+ib,y.end(),p1.yll.end());
//				p1.ll.push_back(tailLen);
//			}
//
//		}
//		else{
//			p1.xll.resize(p1.xll.size()+x.size());
//			std::copy_backward(x.begin(),x.end(),p1.xll.end());
//			p1.yll.resize(p1.yll.size()+y.size());
//			std::copy_backward(y.begin(),y.end(),p1.yll.end());
//			p1.ll.push_back(x.size());
//		}
//	}
//	return p1;
//}


//void PlotData::SetSpec(const CString & xla, const CString & yla, const PlotSpec & psp0)
//{
//	if(!xla.IsEmpty())
//		xlabel=xla;
//	if(!yla.IsEmpty())
//		ylabel=yla;
//
//	psp=psp0;
//}


bool PlotData::SaveImage(CString filepath, CSize sz, CDC* pDC, double pct)
{

	CDC dcMem;   //用于缓冲作图的内存DC
	dcMem.CreateCompatibleDC(pDC);               //依附窗口DC创建兼容内存DC		

	CBitmap bmp;           //内存中承载临时图象的位图
	bmp.CreateCompatibleBitmap(pDC,sz.cx,sz.cy);//创建兼容位图

	dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

	double xmin,xmax,ymin,ymax;
	UpdateRange(raw.xll,xmin,xmax,pct,true);
	UpdateRange(raw.yll,ymin,ymax,pct,true);

	ps.labelC=ps.gridC=ps.metricC=black;
	ps.bkgndC=ps.borderC=ps.winbkC=white;

	CRect plotrect(0,0,sz.cx,sz.cy);
	DrawData(plotrect,&dcMem,*this,xmin,xmax,ymin,ymax);
	dcMem.DeleteDC(); //删除DC

	CImage img;
	img.Attach(HBITMAP(bmp));
	HRESULT hResult = img.Save((LPCWSTR)filepath);


	bmp.DeleteObject(); //删除位图

	if (SUCCEEDED(hResult))
		return true;


	return false;

}
