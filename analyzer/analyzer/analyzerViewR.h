#pragma once
#include "analyzerview.h"
//#include "analyzerDoc.h"
//#include "filefunc.h"

class CanalyzerViewR :
	public CanalyzerView
{
public:
	
	static UINT DataOutAList2PlotDataExList(
		const std::vector<DataOutA> &dol, 
		const ANParaEx &p1, 
		COLORREF bkc, 
		std::vector<PlotDataEx> &pdl,
		bool bShowRs=false,
		bool bReadCb=true)
	{
		//std::vector<DWORD> sl;
		//bool flg=CanalyzerDoc::GetStepList(sl,p1.analysistype);
		bool bShowCb=false;

		//std::vector<PlotDataEx> cbpdl;
		//std::vector<CString> cbnamelist;

		std::vector<RawData> cbrdl;
		std::vector<CString> cbxlabellist;

		std::vector<CString> cbylabellist;
		//std::vector<size_t> cbdolastidx;

		std::vector< std::vector<CString> > cbnlist;


		if(bReadCb){
			switch(p1.analysistype){
			case 2:
				{
					switch(p1.calibrationfactortype){
					case 1:
						{
							DocDataEx ddex0;
							if(ddex0.Read(p1.calibrationfilepath)){
								UINT ff1=ddex0.ComputeStateData();
								if(ff1==0||ff1==7)
									bShowCb=DocDataEx::GetResultData(ddex0.dol,ddex0.p1,cbrdl,cbxlabellist,cbylabellist,cbnlist,true);
							}
						}
						break;
					default:
						break;
					}

				}
				break;

			case 6:
			case 10:
			case 12:
				{
					DocDataEx ddex0;
					if(ddex0.Read(p1.calibrationfilepath)){
						UINT ff1=ddex0.ComputeStateData();
						if(ff1==0||ff1==7)
							bShowCb=DocDataEx::GetResultData(ddex0.dol,ddex0.p1,cbrdl,cbxlabellist,cbylabellist,cbnlist,true);
							//bShowCb=false;
					}
				}
				break;
			default:
				break;
			}
		}

		std::vector<RawData> rdl;
		std::vector<CString> xlabellist;

		std::vector<CString> ylabellist;
	

		std::vector< std::vector<CString> > nlist;
		UINT ff;
		if(bShowRs){
			ff=DocDataEx::GetResultData(dol,p1,rdl,xlabellist,ylabellist,nlist);
		}
		else{
			std::vector<size_t> dolastidx;
			ff=DocDataEx::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);
			CString str;
			str.LoadStringW(IDS_STRING_TEST_CURVE);
			nlist.resize(rdl.size());
			for(size_t i=0;i<nlist.size();i++){
				nlist[i].assign(rdl[i].ll.size(),str);
			}
		}

		PlotSpec ps;
		ps.SetPlotBKCr();
		ps.RefreshWinCr(bkc);





		//if(ff==1)
		{
			for(size_t i=0;i<rdl.size();i++){
				if(i>=pdl.size()){
					pdl.push_back(PlotDataEx(ps));
				}

				pdl[i].pd.ps.xlabel=xlabellist[i];
				pdl[i].pd.ps.ylabel=ylabellist[i];


				//std::vector<CString> namelist(rdl[i].ll.size(),str);

				if( bShowCb && i<cbrdl.size() ){
					//pdl[i].pd.raw=cbrdl[i];
					//pdl[i].pd.ls.assign(cbpdl.back().pd.ls.begin(),cbpdl.back().pd.ls.end());
					int insertN0=pdl[i].pd.SetLineData(cbrdl[i],cbnlist[i]);
					pdl[i].pd.SetLineColor(insertN0,3,1);
					int insertN=pdl[i].pd.AddLineData(rdl[i],nlist[i]);
					pdl[i].pd.SetLineColor(insertN,3,1);

					pdl[i].pd.ResetLineType();
				}			
				else{
					//int insertN=pdl[i].pd.SetLineData(rdl[i],namelist);
					int insertN=pdl[i].pd.SetLineData(rdl[i],nlist[i]);
					pdl[i].pd.SetLineColor(insertN,3,1);

					if(bShowRs){
						pdl[i].pd.ResetLineType();
					}

				}

			}

			pdl.resize(rdl.size());



			return 1;
		}

		return 0;



	};

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

