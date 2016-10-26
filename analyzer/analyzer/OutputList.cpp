
#include "stdafx.h"

#include "OutputList.h"
#include "Resource.h"
//#include "MainFrm.h"
#include "func.h"



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
