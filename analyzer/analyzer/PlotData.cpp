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

	//LineSpecA aa;

	ps.push_back(plotsp);
	//ps.push_back(aa);

	xlabel=xla;
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
}


void PlotData::clear(void)
{
	xll.clear();
	yll.clear();
	ll.clear();
	ps.clear();
	xlabel.Empty();
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
}