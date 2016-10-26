#include "stdafx.h"
#include "func.h"


void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap)
{
	int widthc,widtht;
	widtht=ls->GetStringWidth(str)+gap;
	widthc=ls->GetColumnWidth(nCol);
	if(widthc<widtht)
		ls->SetColumnWidth(nCol,widtht);
}

void AdjustWidth(CListCtrl *ls, int nCol, int nRow, int gap)
{
	CString	str=ls->GetItemText(nRow,nCol);
	AdjustWidth(ls, nCol, str, gap);
}


DWORD stp(BYTE step,BYTE stepControl,BYTE plotFilter)
{
	return step|(stepControl<<8)|(plotFilter<<16);
}

BYTE nby(DWORD w, unsigned int i)
{
	i=i%4;
	i*=8;
	return (BYTE)(w>>i);
}

void nbyl(const std::vector<DWORD> &sl, std::vector<BYTE> &stepl, unsigned int i)
{
	stepl.assign(sl.size(),0);
	for(size_t j=0;j<sl.size();j++){
		stepl[j]=nby(sl[j],i);
	}
}


bool GetStepList(std::vector<DWORD> &sl, int atype)
{

	switch(atype){
	case 1:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_S),
				stp(DOA_S,0,PF_Q_NORMALIZED|PF_S)
			};
			sl.assign(stepl,stepl+2);
		}
		return true;
	case 2:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_SAMPLE),
				stp(DOA_SAMPLE,0,PF_Q_NORMALIZED|PF_SAMPLE)
			};
			sl.assign(stepl,stepl+2);
		}
		return true;
	case 3:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_S),
				stp(DOA_S,0,PF_CONCERTRATION|PF_S)
			};
			sl.assign(stepl,stepl+2);

		}
		return true;

	case 4:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_A),
				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,SC_PLOT_LAST,PF_CONCERTRATION|PF_A),
				stp(DOA_A|DOA_MORE,0,PF_CONCERTRATION|PF_A)
			};
			sl.assign(stepl,stepl+4);

		}
		return true;

	case 5:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S|DOA_A|DOA_MORE,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_CONCERTRATION|PF_L),
				stp(DOA_L,0,PF_CONCERTRATION|PF_L)
			};
			sl.assign(stepl,stepl+3);
		}
		return true;

	case 6:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S|DOA_A,SC_NO_PLOT,0),
				stp(DOA_SAMPLE,SC_NO_PLOT,0)
			};
			sl.assign(stepl,stepl+3);
		}
		return true;

	case 7:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_S),
				stp(DOA_S|DOA_A,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_S)
			};

			sl.assign(stepl,stepl+2);

		}
		return true;

	case 8:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_SAMPLE),
				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,0,PF_Q_NORMALIZED|PF_SAMPLE),
				stp(DOA_A|DOA_MORE,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_NEW_ONCE,PF_Q_NORMALIZED|PF_A|PF_CONCERTRATION)
			};


			sl.assign(stepl,stepl+3);
		}
		return true;

	case 9:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
				stp(DOA_L,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
			};
			sl.assign(stepl,stepl+2);
		}
		return true;
	case 10:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_L,SC_NO_PLOT,0),
				stp(DOA_SAMPLE,SC_NO_PLOT,0)
			};
			sl.assign(stepl,stepl+3);
		}
		return true;

	case 11:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S|DOA_A,SC_NEW_RIGHT_PLOT|SC_NEW_LINE|SC_PLOT_LAST|SC_NEW_ONCE,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
				stp(DOA_L,0,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
			};
			sl.assign(stepl,stepl+3);

		}
		return true;

	case 12:
		{
			DWORD stepl[]={
				stp(DOA_VMS,SC_NO_PLOT,0),
				stp(DOA_S|DOA_A,SC_NO_PLOT,0),
				stp(DOA_SAMPLE|DOA_RESET_SOLUTION_AT_END,/*SC_NEW_RIGHT_PLOT|*/SC_NO_PLOT/*|SC_NEW_LINE|SC_PLOT_LAST*/,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L),
				stp(DOA_L,SC_NEW_LINE,PF_Q_NORMALIZED|PF_CONCERTRATION|PF_L)
			};
			sl.assign(stepl,stepl+4);
		}

		return true;
	default:
		sl.clear();
		return false;
	}


}




UINT ComputeStateData(RawData raw,
	CVPara &p2,
	SAPara &p3,
	int ANPType,
	std::vector<DataOutA> &dol
){



	////////////////////////////////////////////////////////////////////////////////////////

	while( !p3.saplist.empty() && !sl.empty() ){

		BYTE step=nby(sl.front(),0);
		BYTE stepControl=nby(sl.front(),1);
		BYTE plotFilter=nby(sl.front(),2);

		if( dataB.ReadTask(p3.saplist.front(),step) ){

			if( step&DOA_VMS ){				
				CString postr;
				postr.LoadStringW(IDS_STRING_POTENTIAL);
				CString custr;
				custr.LoadStringW(IDS_STRING_CURRENT);
				leftp->AddPlot(PlotData(postr,custr,leftp->psview));

				//if(leftp->GetPD()!=NULL){
				//	//pDoc->lp.back().SetSpec(postr,custr,leftp->GetPD()->psp);
				//	leftp->AddPlot(PlotData(postr,custr,leftp->psview));
				//}
				//else{
				//	//pDoc->lp.back().SetSpec(postr,custr,PlotSpec(0,leftp->bkcr));
				//	leftp->AddPlot(PlotData(postr,custr,PlotSpec(0,leftp->bkcr)));
				//}
			}

			if(stepControl&SC_NEW_RIGHT_PLOT
				&&( !(stepControl&SC_STEP_COMPLETE) || !(stepControl&SC_NEW_ONCE) )){
					//rightp->AddPlot(PlotData());
					CString xla;
					CString yla;
					GetXYLabel(xla,yla,plotFilter);
					rightp->AddPlot(PlotData(xla,yla,rightp->psview));
				//if(rightp->GetPD()!=NULL){
				//	//pDoc->rp.back().SetSpec(xla,yla,rightp->GetPD()->psp);
				//	rightp->AddPlot(PlotData(xla,yla,rightp->GetPD()->psp));
				//}
				//else{
				//	//pDoc->rp.back().SetSpec(xla,yla,PlotSpec(0,rightp->bkcr));
				//	rightp->AddPlot(PlotData(xla,yla,PlotSpec(0,rightp->bkcr)));
				//}
			}

			OneStep(outw,leftp,cba,pst,dataB,filelist,p3,!(step&DOA_MORE)/*,!(stepControl&SC_NO_PLOT)*/);

			if(!(stepControl&SC_NO_PLOT)){
				if(!(stepControl&SC_PLOT_LAST)){
					//SetData(x[0],y[0],plotFilter,dataB.doa);
					//if( (stepControl&SC_NEW_LINE)
					//	&&!(stepControl&SC_STEP_COMPLETE)){
					//		LineSpec ps1=lsp;
					////		ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp.back().ps.size() ) ) ;
					//		pDoc->rp.back().AddNew(x,y,ps1);
					//}
					//else{
					//	pDoc->rp.back().AddFollow(x,y);
					//}

					OnePlot(pDoc->rp.back(),
						outw,
						//dataB.doa,
						pDoc->dol.back(),
						plotFilter,
						stepControl,
						((stepControl&SC_NEW_LINE)&&!(stepControl&SC_STEP_COMPLETE)),
						lsp);
					if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
						rightp->Invalidate(FALSE);
				}
			}

			stepControl|=SC_STEP_COMPLETE;
			sl.front()=stp(step,stepControl,plotFilter);

		}
		else{
			if( stepControl&SC_STEP_COMPLETE ){

				if(step&DOA_RESET_SOLUTION_AT_END){		
					dataB.doa.ResetCompound();
					pDoc->dol.back().ResetCompound();
				}

				if(!(stepControl&SC_NO_PLOT)){
					if( stepControl&SC_PLOT_LAST ){
						//SetData(x[0],y[0],plotFilter,dataB.doa);
						//if(stepControl&SC_NEW_LINE){
						//	LineSpec ps1=lsp;
						//	ps1.colour=genColor( genColorvFromIndex<float>( pDoc->rp.back().ps.size() ) ) ;
						//	pDoc->rp.back().AddNew(x,y,ps1);
						//}
						//else{
						//	pDoc->rp.back().AddFollow(x,y);
						//}


						OnePlot(pDoc->rp.back(),
							outw,
							//dataB.doa,
							pDoc->dol.back(),
							plotFilter,
							stepControl,
							((stepControl&SC_NEW_LINE)),
							lsp);

						if(rightp->updatePlotRange((int)(pDoc->rp.size())-1))
							rightp->Invalidate(FALSE);
					}	
				}
				sl.erase(sl.begin());
			}
			else{				
				AfxMessageBox(IDS_STRING_STEP_ERROR);
				CString strerr;
				strerr.LoadStringW(IDS_STRING_STEP_ERROR);
				::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
				pst=stop;
				return 1;
				p3.saplist.erase(p3.saplist.begin());
			}
		}
	}


	return 0;

}
