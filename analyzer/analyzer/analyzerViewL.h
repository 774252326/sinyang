#pragma once
#include "analyzerview.h"

class CanalyzerViewL :	public CanalyzerView
{
public:
	static bool IsVMSStep(DataOutA d) {
		return d.stepFilter&DOA_VMS;
	};

	static UINT RawData2PlotDataList(const RawData &raw, 
		const std::vector<DataOutA> dol, 
		COLORREF bkc,
		COLORREF newClr,
		COLORREF oldClr,
		std::vector<PlotDataEx> &pdl)
	{
		if(dol.empty()){
			pdl.clear();
			return 1;
		}

		std::vector<DataOutA> doltmp;
		doltmp.assign(dol.begin(),dol.end());

		std::vector<DataOutA>::iterator it0 = doltmp.begin();
		std::vector<DataOutA>::iterator it1 = (it0+1);

		size_t pi0=0;
		size_t pn0=0;

		while(it0!=doltmp.end()){

			if(pi0>=pdl.size()){
				PlotSpec ps;
				//ps.winbkC=bkc;
				//ps.SetCr();
				ps.SetPlotBKCr();
				ps.RefreshWinCr(bkc);

				ps.xlabel.LoadStringW(IDS_STRING_POTENTIAL);
				ps.ylabel.LoadStringW(IDS_STRING_CURRENT);

				pdl.push_back(PlotDataEx(ps));
			}
			else{
				pdl[pi0].pd.ps.xlabel.LoadStringW(IDS_STRING_POTENTIAL);
				pdl[pi0].pd.ps.ylabel.LoadStringW(IDS_STRING_CURRENT);
			}
			///////////////////////////////////////////////
			size_t n0=it0 - doltmp.begin();

			it1=std::find_if (it0+1, doltmp.end(), IsVMSStep);

			size_t n1=it1 - doltmp.begin();
			///////////////////////////////////////////////////////////////
			RawData rawtmp;
			rawtmp.ll.assign(raw.ll.begin()+n0,raw.ll.begin()+n1);
			size_t pn=rawtmp.ValidPointNumber();

			rawtmp.xll.assign(raw.xll.begin()+pn0,raw.xll.begin()+pn0+pn);
			rawtmp.yll.assign(raw.yll.begin()+pn0,raw.yll.begin()+pn0+pn);
			///////////////////////////////////////////////////////////////////
			std::vector<CString> namelist;
			for(size_t i=n0;i<n1;i++){
				namelist.push_back(doltmp[i].GetStepName(i));
			}
			////////////////////////////////////////////////////////////////////////
			int insertN=pdl[pi0].pd.SetLineData(rawtmp,namelist);

			insertN=pdl[pi0].pd.ls.size();
			pdl[pi0].pd.SetLineColor(insertN,newClr,oldClr);

			/////////////////////////////////////////////////////////////
			pi0++;
			pn0+=pn;

			it0=it1;
		}


		pdl.resize(pi0);

		if(!pdl.empty()){
			pdl.back().pd.ShowLastLast();
		}

		return 0;


	};


protected:
	CanalyzerViewL(void);
	virtual ~CanalyzerViewL(void);
	DECLARE_DYNCREATE(CanalyzerViewL)
public:	

	virtual void OnInitialUpdate();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg LRESULT OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam);
public:
	COLORREF newCr;
	COLORREF oldCr;
	afx_msg void OnOptionsPlotsettings();
};

