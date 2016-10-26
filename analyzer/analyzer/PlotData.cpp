// PlotData.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotData.h"


// PlotData

PlotData::PlotData()
	//£ºxlabel(_T(""))
	//, ylabel(_T(""))
{
}

PlotData::~PlotData()
{
}


// PlotData member functions


void PlotData::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{	// storing code

		if(CheckData()){
			ar<<ll.size()
				<<xlabel
				<<ylabel;
			for(size_t i=0;i<ll.size();i++){
				//ps[i].Serialize(ar);
				ar<<ps[i].colour
					<<ps[i].name
					<<ps[i].dotSize
					<<ps[i].showLine
					<<ps[i].smoothLine
					<<ps[i].traceLast;

				ar<<ll[i];
			}
			for(size_t j=0;j<xll.size();j++){
				ar<<xll[j]
				<<yll[j];
			}
		}

		/////////////////////////////////////////////////

		//		ar<<ps.size()
		//	<<xlabel
		//	<<ylabel;
		//for(size_t i=0;i<ps.size();i++){
		//	//ps[i].Serialize(ar);
		//	ar<<ps[i].colour
		//		<<ps[i].name
		//		<<ps[i].dotSize
		//		<<ps[i].showLine
		//		<<ps[i].smoothLine
		//		<<ps[i].traceLast;

		//	ar<<xlist[i].size();
		//for(size_t j=0;j<xlist[i].size();j++){
		//	ar<<xlist[i][j]
		//	<<ylist[i][j];
		//}
		//}




	}
	else
	{	// loading code

		size_t nl;
		ar>>nl
			>>xlabel
			>>ylabel;
		//LineSpec ls;
		plotspec ls;
		ps.assign(nl,ls);
		ll.assign(nl,0);
		nl=0;
		for(size_t i=0;i<ll.size();i++){
			//ps[i].Serialize(ar);
			ar>>ps[i].colour
				>>ps[i].name
				>>ps[i].dotSize
				>>ps[i].showLine
				>>ps[i].smoothLine
				>>ps[i].traceLast;


			ar>>ll[i];
			nl+=ll[i];
		}

		xll.assign(nl,0);
		yll.assign(nl,0);

		for(size_t j=0;j<nl;j++){
			ar>>xll[j]
			>>yll[j];
		}

		///////////////////////////////////////////////////////////

		//size_t nl;
		//ar>>nl
		//	>>xlabel
		//	>>ylabel;
		////LineSpec ls;
		//plotspec ls;
		//ps.assign(nl,ls);
		////ll.assign(nl,0);
		//nl=0;
		//for(size_t i=0;i<ps.size();i++){
		//	//ps[i].Serialize(ar);
		//	ar>>ps[i].colour
		//		>>ps[i].name
		//		>>ps[i].dotSize
		//		>>ps[i].showLine
		//		>>ps[i].smoothLine
		//		>>ps[i].traceLast;


		//	//ar>>ll[i];
		//	//nl+=ll[i];
		//	ar>>nl;
		//	//xll.assign(nl,0);
		//	//yll.assign(nl,0);

		//	std::vector<double> x(nl);
		//	std::vector<double> y(nl);

		//	for(size_t j=0;j<nl;j++){
		//		ar>>x[j]
		//		>>y[j];
		//	}

		//	xlist.push_back(x);
		//	ylist.push_back(y);

		//}





	}

}


bool PlotData::CheckData(void)
{
	if(ps.size()==ll.size() && xll.size()==yll.size()){
		size_t k=xll.size();
		for(size_t i=0;i<ll.size();i++){
			k-=ll[i];
		}
		if(k==0){
			return true;
		}
	}

	return false;
}


void PlotData::AddNew(const std::vector<double> &x, const std::vector<double> &y, const plotspec &plotsp, const CString &xla, const CString &yla)
{
	if(x.size()!=y.size())
		return;

	xll.resize(xll.size()+x.size());
	std::copy_backward(x.begin(),x.end(),xll.end());
	yll.resize(yll.size()+y.size());
	std::copy_backward(y.begin(),y.end(),yll.end());
	ll.push_back(x.size());

	xlist.push_back(x);
	ylist.push_back(y);

	ps.push_back(plotsp);

	if(!xla.IsEmpty())
		xlabel=xla;
	if(!yla.IsEmpty())
		ylabel=yla;
}


void PlotData::AddFollow(const std::vector<double> &x, const std::vector<double> &y)
{
	if(x.size()!=y.size())
		return;

	xll.resize(xll.size()+x.size());
	std::copy_backward(x.begin(),x.end(),xll.end());
	yll.resize(yll.size()+y.size());
	std::copy_backward(y.begin(),y.end(),yll.end());
	ll.back()+=x.size();


	xlist.back().resize(xlist.back().size()+x.size());
	std::copy_backward(x.begin(),x.end(),xlist.back().end());
	ylist.back().resize(ylist.back().size()+y.size());
	std::copy_backward(y.begin(),y.end(),ylist.back().end());

}


void PlotData::clear(void)
{
	xll.clear();
	yll.clear();

	xlist.clear();
	ylist.clear();

	ll.clear();
	ps.clear();

	if(!xlabel.IsEmpty())
		xlabel.Empty();
	if(!ylabel.IsEmpty())
		ylabel.Empty();
}


void PlotData::SaveFile(CString fp)
{
	CFile theFile;
	theFile.Open(fp, CFile::modeCreate | CFile::modeWrite);
	CArchive archive(&theFile, CArchive::store);

	Serialize(archive);

	archive.Close();
	theFile.Close();
}


void PlotData::ReadFile(CString fp)
{
	CFile theFile;
	theFile.Open(fp, /*CFile::modeCreate |*/ CFile::modeRead);
	CArchive archive(&theFile, CArchive::load);
	Serialize(archive);
	archive.Close();
	theFile.Close();
}

void PlotData::AppendData(const PlotData & pda)
{
	xll.resize(xll.size()+pda.xll.size());
	std::copy_backward(pda.xll.begin(),pda.xll.end(),xll.end());
	yll.resize(yll.size()+pda.yll.size());
	std::copy_backward(pda.yll.begin(),pda.yll.end(),yll.end());
	ll.resize(ll.size()+pda.ll.size());
	std::copy_backward(pda.ll.begin(),pda.ll.end(),ll.end());
	ps.resize(ps.size()+pda.ps.size());
	std::copy_backward(pda.ps.begin(),pda.ps.end(),ps.end());
	xlabel=pda.xlabel;
	ylabel=pda.ylabel;

	//xlist.resize(xlist.size()+pda.xlist.size());
	//std::copy_backward(pda.xlist.begin(),pda.xlist.end(),xlist.end());
	//ylist.resize(ylist.size()+pda.ylist.size());
	//std::copy_backward(pda.ylist.begin(),pda.ylist.end(),ylist.end());


	for(size_t i=0;i<pda.xlist.size();i++){
		{
			std::vector<double> temp;
			temp.assign(pda.xlist[i].begin(),pda.xlist[i].end());
			xlist.push_back(temp);
		}
		{
			std::vector<double> temp;
			temp.assign(pda.ylist[i].begin(),pda.ylist[i].end());
			ylist.push_back(temp);
		}
	}

}

void PlotData::GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y)
{
	if(index<0 || index>=ll.size()){
		x.clear();
		y.clear();
		return;
	}

	size_t starti=0;
	size_t endi;
	for(size_t i=0;i<index;i++){
		starti+=ll[i];
	}
	endi=starti+ll[index];

	x.assign(xll.begin()+starti, xll.begin()+endi);
	y.assign(yll.begin()+starti, yll.begin()+endi);
}
