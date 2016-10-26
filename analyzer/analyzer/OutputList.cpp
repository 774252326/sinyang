
#include "stdafx.h"

#include "OutputList.h"
#include "Resource.h"
//#include "MainFrm.h"
#include "func.h"

//#include "analyzerDoc.h"
#include "analyzerViewL.h"
#include "analyzerViewR.h"

#include <time.h>
/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
	//: gap(12)
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE(MESSAGE_UPDATE_DOL, &COutputList::OnMessageUpdateDol)
	ON_MESSAGE(MESSAGE_SHOW_DOL, &COutputList::OnMessageShowDol)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("Copy output"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("Clear output"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}



BOOL COutputList::InsertListCtrl(int StepNo, CString StepName, double addVol, double totalVol, double Q, double nQ, bool Use, int CycleNo)
{

	//insert the data from the list control head(��¼�ĳɴ����������)
	CString str;
	CString strTemp;

	str.Format(L"%d",StepNo);
	InsertItem(StepNo,str);

	SetItemText(StepNo,1,StepName);

	if(addVol!=0){
		str.Format(L"%g",addVol);
		SetItemText(StepNo,2,str);

		str.Format(L"%g",totalVol);
		SetItemText(StepNo,3,str);
	}

	str.Format(L"%g",Q);
	SetItemText(StepNo,4,str);

	if(Use){
		if(nQ!=0){
			str.Format(L"%g",nQ);
			SetItemText(StepNo,5,str);
		}
		//ASSERT
		(str.LoadString(IDS_STRING_YES));
	}
	else{
		//ASSERT
		(str.LoadString(IDS_STRING_NO));
	}
	SetItemText(StepNo,6,str);

	for(int i=0;i<7;i++){
		AdjustWidth(this,i,StepNo);
	}
	EnsureVisible(StepNo, FALSE);

	return TRUE;
}



BOOL COutputList::InsertListCtrl(int StepNo, DataOutA &dout, int count, int Ari, bool bUse)
{
	CString str,strt;
	strt.LoadStringW(IDS_STRING_CYCLE);
	str.Format(L"%s(%s%d)",dout.GetStepName(count),strt,Ari+1);

	return InsertListCtrl(
		StepNo,
		str,
		(Ari>0?0:dout.addVolume),
		dout.TotalVolume(),
		dout.Ar[Ari],
		dout.Ar[Ari]/dout.Ar0,
		(Ari==dout.UseIndex&bUse)
		);
}

BOOL COutputList::SetLastUse(const DataOutA & doa)
{
	int arsz=doa.Ar.size();

	if(doa.UseIndex>0
		&& doa.UseIndex<arsz){


			int nCount = GetItemCount();

			int StepNo=nCount-arsz+doa.UseIndex;

			CString str;
			str.Format(L"%g",doa.Ar[doa.UseIndex]/doa.Ar0);
			SetItemText(StepNo,5,str);

			//ASSERT
			(str.LoadString(IDS_STRING_YES));
			SetItemText(StepNo,6,str);



	}

	return TRUE;
}


afx_msg LRESULT COutputList::OnMessageUpdateDol(WPARAM wParam, LPARAM lParam)
{
	//CanalyzerViewL *pavl=(CanalyzerViewL*)wParam;
	//CanalyzerViewR *pavr=(CanalyzerViewR*)lParam;

	CMainFrame *mf=(CMainFrame*)(GetParentFrame());
	CanalyzerViewL *pavl=((CanalyzerViewL*)(mf->m_wndSplitter.GetPane(0,0)));
	CanalyzerViewR* pavr=((CanalyzerViewR*)(mf->m_wndSplitter.GetPane(0,1)));
	CanalyzerDoc *pad=pavl->GetDocument();	 

	//HWND hwl=mf->m_wndSplitter.GetPane(0,0)->GetSafeHwnd();
	//HWND hwr=mf->m_wndSplitter.GetPane(0,1)->GetSafeHwnd();


	dol.clear();

	UINT flg=ComputeStateData(pad->p1.analysistype,pad->p2,pad->p3,pad->raw,dol);	

	//if(flg==0){
	//this->ShowDOL(true);



	//::SendMessage(pavl->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
	//::SendMessage(pavr->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);

	//::SendMessage(hwl,MESSAGE_UPDATE_RAW,NULL,NULL);
	//::SendMessage(hwr,MESSAGE_UPDATE_TEST,NULL,NULL);

	::PostMessage(pavr->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
	::PostMessage(pavl->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
	//pavr->SendMessage(MESSAGE_UPDATE_TEST,NULL,NULL);
	//}
	::PostMessage(this->GetSafeHwnd(),MESSAGE_SHOW_DOL,wParam,NULL);

	return 0;
}


void COutputList::ShowDOL(bool bRefreshAll)
{
	clock_t t=clock();
	if(bRefreshAll){
		this->DeleteAllItems();
		size_t ci=0;
		for(size_t i=0;i<dol.size();i++){
			for(size_t ari=0;ari<dol[i].Ar.size();ari++){
				this->InsertListCtrl(ci,dol[i],i,ari);
				ci++;
			}
		}
	}
	else{
		//int ti=this->GetItemCount();
		//size_t nr=GetDOLRow();
		//if(ti<=0){
		//	ShowDOL(true);
		//}
		//else{
		//if(nr==ti+1){
		//	this->DeleteItem(ti-1);
		//	if(dol.back().Ar.empty())
		//	{

		//if(ti>dol.size()){
		//	ShowDOL(true);
		//}
		//else{
		//if(ti>0){
		//	this->DeleteItem(ti-1);
		//	this->InsertListCtrl(ti-1,
		//}

		//for(int i=ti-1;


	}

	TRACE(L"%d ms\n",clock()-t);


}


size_t COutputList::GetDOLRow(void)
{
	size_t s=0;
	for(size_t i=0;i<dol.size();i++){
		s+=dol[i].Ar.size();
	}
	return s;
}


afx_msg LRESULT COutputList::OnMessageShowDol(WPARAM wParam, LPARAM lParam)
{


	int ti=this->GetItemCount();
	size_t nr=GetDOLRow();
	if(ti==nr || nr==0){
		return 1;
	}

	size_t StepNo;
	CString strcycle;
	strcycle.LoadStringW(IDS_STRING_CYCLE);
	size_t starti;
	if(true)
	{

		this->DeleteAllItems();
		StepNo=0;
		starti=0;



	for(size_t i=starti;i<dol.size();i++){

		CString StepName=dol[i].GetStepName(i);

		for(size_t ari=0;ari<dol[i].Ar.size();ari++){

			CString str;

			str.Format(L"%d",StepNo);
			InsertItem(StepNo,str);
			AdjustWidth(this,0,str);

			str.Format(L"%s(%s%d)",StepName,strcycle,ari+1);
			SetItemText(StepNo,1,str);
			AdjustWidth(this,1,str);

			if(ari==0){
				str.Format(L"%g",dol[i].addVolume);
				SetItemText(StepNo,2,str);
				AdjustWidth(this,2,str);
			}

			str.Format(L"%g",dol[i].TotalVolume());
			SetItemText(StepNo,3,str);
			AdjustWidth(this,3,str);

			str.Format(L"%g",dol[i].Ar[ari]);
			SetItemText(StepNo,4,str);
			AdjustWidth(this,4,str);

			if(ari==dol[i].UseIndex){
				str.Format(L"%g",dol[i].Ar[ari]/dol[i].Ar0);
				SetItemText(StepNo,5,str);
				AdjustWidth(this,5,str);
				//ASSERT
				(str.LoadString(IDS_STRING_YES));
			}
			else{
				//ASSERT
				(str.LoadString(IDS_STRING_NO));
			}
			SetItemText(StepNo,6,str);
			AdjustWidth(this,6,str);

			EnsureVisible(StepNo, FALSE);
			StepNo++;
		}
	}

	}
	else{
		if(dol.back().Ar.size()>1){
			SetItemText(ti-1,5,L"");
			CString str;
			(str.LoadString(IDS_STRING_NO));
			SetItemText(ti-1,6,str);
			AdjustWidth(this,6,str);
		}

		CString StepName=dol.back().GetStepName(dol.size()-1);

		CString str;

			str.Format(L"%d",StepNo);
			InsertItem(StepNo,str);
			AdjustWidth(this,0,str);


			size_t ari=dol.back().Ar.size();
			str.Format(L"%s(%s%d)",StepName,strcycle,ari);
			SetItemText(StepNo,1,str);
			AdjustWidth(this,1,str);

			ari--;

			if(ari==0){
				str.Format(L"%g",dol.back().addVolume);
				SetItemText(StepNo,2,str);
				AdjustWidth(this,2,str);
			}

			str.Format(L"%g",dol.back().TotalVolume());
			SetItemText(StepNo,3,str);
			AdjustWidth(this,3,str);

			str.Format(L"%g",dol.back().Ar[ari]);
			SetItemText(StepNo,4,str);
			AdjustWidth(this,4,str);

			if(ari==dol.back().UseIndex){
				str.Format(L"%g",dol.back().Ar[ari]/dol.back().Ar0);
				SetItemText(StepNo,5,str);
				AdjustWidth(this,5,str);
				//ASSERT
				(str.LoadString(IDS_STRING_YES));
			}
			else{
				//ASSERT
				(str.LoadString(IDS_STRING_NO));
			}
			SetItemText(StepNo,6,str);
			AdjustWidth(this,6,str);

			EnsureVisible(StepNo, FALSE);

	}
		//starti=dol.size()-1;

		//for(int j=StepNo;j<ti;j++){
			//this->DeleteItem(j);
		//}

	//}




	return 0;
}
