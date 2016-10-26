#include "StdAfx.h"
#include "OutputListA.h"
#include "../resource.h"
//#include "../func.h"
//#include "../OutputWnd.h"
//#include "../MainFrm.h"
#include "../analyzerDoc.h"




COutputListA::COutputListA(void)
{
}


COutputListA::~COutputListA(void)
{
}
BEGIN_MESSAGE_MAP(COutputListA, ListCtrlA)
	ON_WM_CREATE()
	ON_MESSAGE(MESSAGE_SHOW_DOL, &COutputListA::OnMessageShowDol)
	//ON_MESSAGE(MESSAGE_UPDATE_DOL, &COutputListA::OnMessageUpdateDol)
	//ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
END_MESSAGE_MAP()


int COutputListA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	SetStyle();

	BuildList();

	return 0;
}

size_t COutputListA::GetDOLRow(void)
{
	size_t s=0;
	for(size_t i=0;i<dol.size();i++){
		s+=dol[i].Ar.size();
	}
	return s;
}


void COutputListA::ShowDOL()
{


	size_t StepNo;
	size_t starti;
	StepNo=0;
	starti=0;

	CString strcycle;
	strcycle.LoadStringW(IDS_STRING_CYCLE);

	for(size_t i=starti;i<dol.size();i++){

		CString StepName=dol[i].GetStepName(i);

		for(size_t ari=0;ari<dol[i].Ar.size();ari++){

			CString str;
			str.Format(L"%d",StepNo);
			if(StepNo>=GetItemCount()){					
				InsertItem(StepNo,str);
			}
			else{
				SetItemText(StepNo,0,str);				
			}
			AdjustWidth(0,str);

			str.Format(L"%s(%s%d)",StepName,strcycle,ari+1);
			SetItemText(StepNo,1,str);
			AdjustWidth(1,str);

			if(ari==0){
				str.Format(L"%g",dol[i].addVolume);
				SetItemText(StepNo,2,str);
				AdjustWidth(2,str);
			}
			else{
				SetItemText(StepNo,2,L"");
			}

			str.Format(L"%g",dol[i].TotalVolume());
			SetItemText(StepNo,3,str);
			AdjustWidth(3,str);

			str.Format(L"%g",dol[i].Ar[ari]);
			SetItemText(StepNo,4,str);
			AdjustWidth(4,str);


			if(ari==dol[i].UseIndex){
				str.Format(L"%g",dol[i].Ar[ari]/dol[i].Ar0);
				SetItemText(StepNo,5,str);
				AdjustWidth(5,str);
				//ASSERT
				(str.LoadString(IDS_STRING_YES));
				//SetChoice(StepNo,6,0);
				SetItemText(StepNo,6,str);
			}
			else{
				SetItemText(StepNo,5,L"");
				//ASSERT				
				(str.LoadString(IDS_STRING_NO));
				//SetChoice(StepNo,6,1);
				SetItemText(StepNo,6,str);
			}
			AdjustWidth(6,str);


			StepNo++;
		}
	}

	EnsureVisible(StepNo-1, FALSE);

	while(GetItemCount()>StepNo){
		DeleteItem(StepNo);
	}

}

afx_msg LRESULT COutputListA::OnMessageShowDol(WPARAM wParam, LPARAM lParam)
{

	CanalyzerDoc *ow=(CanalyzerDoc*)(this->GetParentFrame()->GetActiveDocument());
	CSingleLock singleLock(&(ow->m_CritSection));
	if (singleLock.Lock())  // Resource has been locked
	{
		dol.assign(ow->da.dol.begin(),ow->da.dol.end());
		// Now that we are finished, 
		// unlock the resource for others.
		singleLock.Unlock();
	}
	ShowDOL();
	return 0;
}

void COutputListA::BuildList(void)
{	
	std::vector<CString> strl;
	CString strTemp;
	for(int i=IDS_STRING111;i<=IDS_STRING117;i++){		
		(strTemp.LoadString(i));
		strl.push_back(strTemp);
	}

	this->SetHeader(strl);

	//cbstr[6].clear();
	//(strTemp.LoadString(IDS_STRING_YES));
	//cbstr[6].push_back(strTemp);
	//(strTemp.LoadString(IDS_STRING_NO));
	//cbstr[6].push_back(strTemp);

	for(int i=0;i<strl.size();i++){		
		AdjustWidth(i,strl[i],30);
	}
}

void COutputListA::ResetHeader(void)
{	
	//std::vector<CString> strl;
	//CString strTemp;
	//for(int i=IDS_STRING111;i<=IDS_STRING117;i++){		
	//	(strTemp.LoadString(i));
	//	strl.push_back(strTemp);
	//}

	////cbstr[6].clear();
	////(strTemp.LoadString(IDS_STRING_YES));
	////cbstr[6].push_back(strTemp);
	////(strTemp.LoadString(IDS_STRING_NO));
	////cbstr[6].push_back(strTemp);


	//LVCOLUMN col;
	//col.mask = LVCF_TEXT;
	//col.cchTextMax=256;
	//col.pszText=new TCHAR[col.cchTextMax];

	//for(int i=0;i<strl.size();i++){
	//	GetColumn(i,&col);
	//	CString::CopyChars(col.pszText,strl[i].GetBuffer(),strl[i].GetLength()+1);
	//	SetColumn(i,&col);
	//	AdjustWidth(i,strl[i],30);
	//}
	//delete [] col.pszText;

	BuildList();

}