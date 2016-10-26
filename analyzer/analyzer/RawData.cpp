// RawData.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "RawData.h"
#include "func.h"
#include "pcct.h"
#include "analyzerViewL.h"



// RawData

RawData::RawData()
{
}

RawData::~RawData()
{
}


void RawData::operator=(const RawData &src)
{
	ll.assign(src.ll.begin(),src.ll.end());
	xll.assign(src.xll.begin(),src.xll.end());
	yll.assign(src.yll.begin(),src.yll.end());
}


// RawData member functions


void RawData::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<ll.size();		
		size_t si=0;
		size_t ei=0;
		for(size_t i=0;i<ll.size();i++){
			ar<<ll[i];
			ei=si+ll[i];
			for(size_t j=si;j<ei;j++){
				ar<<xll[j]
				<<yll[j];
			}
			si=ei;
		}
	}
	else
	{	// loading code
		size_t l;

		ar>>l;
		ll.assign(l,0);
		size_t si=0;
		size_t ei=0;
		for(size_t i=0;i<ll.size();i++){
			ar>>ll[i];
			xll.resize(si+ll[i],0);
			yll.resize(si+ll[i],0);
			ei=si+ll[i];
			for(size_t j=si;j<ei;j++){
				ar>>xll[j]
				>>yll[j];
			}
			si=ei;
		}

	}
}


void RawData::GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y) const
{
	if(index>=ll.size()){
		x.clear();
		y.clear();
		return;
	}

	size_t starti=0;
	for(size_t i=0;i<index;i++){
		starti+=ll[i];
	}
	size_t endi=starti+ll[index];

	x.assign(xll.begin()+starti, xll.begin()+endi);
	y.assign(yll.begin()+starti, yll.begin()+endi);
}

void RawData::Clear(void)
{
	xll.clear();
	yll.clear();
	ll.clear();
}


int RawData::LoadFromFileList(CString fp)
{
	Clear();

	std::vector<CString> filelist;
	LoadFileList(fp,filelist);
	pcct data;

	while(!filelist.empty()){
		/////load data from file////////////
		data.clear();
		data.readFile(filelist.front());
		data.TomA();

		xll.resize(xll.size()+data.potential.size());
		std::copy_backward(data.potential.begin(),data.potential.end(),xll.end());

		yll.resize(yll.size()+data.current.size());
		std::copy_backward(data.current.begin(),data.current.end(),yll.end());

		ll.push_back(data.potential.size());

		filelist.erase(filelist.begin());

	}

	return 0;
}



int RawData::LoadFromFileList(CString fp, HWND olhw, size_t nd, DWORD sleepms)
{
	Clear();


	//CMainFrame *mf=(CMainFrame*)( ((CanalyzerViewL*)lv)->GetParentFrame() );
	//COutputList* ol=mf->GetOutputWnd()->GetListCtrl();

	std::vector<CString> filelist;
	LoadFileList(fp,filelist);
	pcct data;



	while(!filelist.empty()){
		/////load data from file////////////
		data.clear();
		data.readFile(filelist.front());
		data.TomA();
		
	//	/////////////////prompt add solution//////////////////////////////
	//::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(dataC.doa.addVolume),NULL);
	///////////////////wait response/////////////////////////
	//pst=pause;
	//WaitSecond(pst);
	/////////////////////refresh analysis class///////////////////////
	////dataC.clear();
	//////////////////////prompt running////////////////////////////////
	//::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

		std::vector<double> x;
		std::vector<double> y;
		size_t rnd;
		ll.push_back(0);

		do{
			rnd=data.popData(x,y,nd);
			xll.resize(xll.size()+x.size());
			std::copy_backward(x.begin(),x.end(),xll.end());

			yll.resize(yll.size()+y.size());
			std::copy_backward(y.begin(),y.end(),yll.end());

			ll.back()+=x.size();

			//::SendMessage(ol->GetSafeHwnd(),MESSAGE_UPDATE_DOL,(WPARAM)lv,NULL);
			::PostMessage(olhw,MESSAGE_UPDATE_DOL,NULL,NULL);

			Sleep(sleepms);

		}while(rnd>0);

		//if(rnd==0)
		//ll.push_back(data.potential.size());

		filelist.erase(filelist.begin());



	}

	return 0;
}