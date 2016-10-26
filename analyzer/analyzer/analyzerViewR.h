#pragma once
#include "analyzerview.h"
//#include "analyzerDoc.h"
//#include "filefunc.h"

class CanalyzerViewR :
	public CanalyzerView
{
public:

//
//	static bool GetCalibrationCurve(
//	CString calibrationfilepath, 
//	std::vector<PlotDataEx> &pdl, 
//	COLORREF bkc=RGB(255,255,255))
//{
//	CanalyzerDoc tmp;
//	if(ReadFileCustom(&tmp,1,calibrationfilepath)){
//
//		//std::vector<DataOutA> dol;
//		//sapitemA sapitemdummy;
//		//size_t nextidx;
//		//BYTE bytedummy;
//		//double doubledummy;
//		//size_t nowidx;
//		//SAPara p3d;
//		//UINT flg=ComputeStateData(tmp.p1.analysistype,tmp.p2,tmp.p3,tmp.raw,dol,p3d,nowidx,nextidx,bytedummy,doubledummy);
//
//		UINT flg=tmp.ComputeStateData();
//
//		if(flg==0){
//			UINT ff=DataOutAList2PlotDataExList(tmp.dol,tmp.p1,bkc,pdl);
//
//			if(ff==0){
//
//				for(size_t i=0;i<pdl.size();i++){
//					for(size_t j=0;j<pdl[i].pd.ls.size();j++){
//						pdl[i].pd.ls[j].name.LoadStringW(IDS_STRING_CALIBRATION_CURVE);
//					}
//				}
//
//				Compute(tmp.dol,tmp.p1,pdl,true);
//
//				return true;
//			}
//		}
//	}
//
//
//	return false;
//
//
//
//
//};


	//static UINT DataOutAList2PlotDataExList(
	//	const std::vector<DataOutA> &dol, 
	//	const ANPara &p1, 
	//	COLORREF bkc, 
	//	std::vector<PlotDataEx> &pdl,
	//	bool bReadCb=true)
	//{
	//	std::vector<DWORD> sl;
	//	bool flg=CanalyzerDoc::GetStepList(sl,p1.analysistype);
	//	bool bShowCb=false;

	//	std::vector<PlotDataEx> cbpdl;
	//	std::vector<CString> cbnamelist;

	//	if(bReadCb){
	//		switch(p1.analysistype){
	//		case 2:
	//			{

	//				/////////////////////////plot standrad curve////////////////////////

	//				if(p1.calibrationfactortype!=0){
	//					if(	p1.calibrationfactortype!=1
	//						||!GetCalibrationCurve(p1.calibrationfilepath,cbpdl)){	
	//							return 1;
	//					}
	//					bShowCb=true;
	//				}
	//			}
	//			break;

	//		case 6:
	//		case 10:
	//		case 12:
	//			{
	//				if(!GetCalibrationCurve(p1.calibrationfilepath,cbpdl)){
	//					return 1;
	//				}
	//				bShowCb=true;

	//			}
	//			break;

	//		default:
	//			//return 1;
	//			break;
	//		}
	//	}

	//	std::vector<RawData> rdl;
	//	std::vector<CString> xlabellist;

	//	std::vector<CString> ylabellist;
	//	std::vector<size_t> dolastidx;


	//	UINT ff=CanalyzerDoc::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);

	//	PlotSpec ps;
	//	ps.SetPlotBKCr();
	//	ps.RefreshWinCr(bkc);

	//	CString str;
	//	str.LoadStringW(IDS_STRING_TEST_CURVE);



	//	//if(ff==1){
	//	for(size_t i=0;i<rdl.size();i++){
	//		if(i>=pdl.size()){
	//			pdl.push_back(PlotDataEx(ps));
	//		}

	//		pdl[i].pd.ps.xlabel=xlabellist[i];
	//		pdl[i].pd.ps.ylabel=ylabellist[i];


	//		std::vector<CString> namelist(rdl[i].ll.size(),str);

	//		if( bShowCb && !cbpdl.empty() ){
	//			pdl[i].pd.raw=cbpdl.back().pd.raw;
	//			pdl[i].pd.ls.assign(cbpdl.back().pd.ls.begin(),cbpdl.back().pd.ls.end());
	//			int insertN=pdl[i].pd.AddLineData(rdl[i],namelist);
	//			pdl[i].pd.SetLineColor(insertN,3,1);
	//		}			
	//		else{
	//			int insertN=pdl[i].pd.SetLineData(rdl[i],namelist);
	//			pdl[i].pd.SetLineColor(insertN,3,1);
	//		}

	//	}

	//	pdl.resize(rdl.size());

	//	return 0;
	//	//}



	//	return 3;


	//};

protected:
	DECLARE_DYNCREATE(CanalyzerViewR)
public:
	CanalyzerViewR(void);
	virtual ~CanalyzerViewR(void);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMessageUpdateTest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageComputeResult(WPARAM wParam, LPARAM lParam);
public:
	virtual void OnInitialUpdate();
};

