#include "StdAfx.h"
#include "ListCtrlSAP.h"
//#include "func.h"
#include "../resource.h"

ListCtrlSAP::ListCtrlSAP(void)
{
}


ListCtrlSAP::~ListCtrlSAP(void)
{
}
BEGIN_MESSAGE_MAP(ListCtrlSAP, ListCtrlAA)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, &ListCtrlSAP::OnLvnKeydown)
	ON_COMMAND(ID_POPUP_INSERT, &ListCtrlSAP::OnPopupInsert)
	ON_COMMAND(ID_POPUP_REMOVE, &ListCtrlSAP::OnPopupRemove)
END_MESSAGE_MAP()


int ListCtrlSAP::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CString strTemp;
	std::vector<CString> strl;
	for(int i=IDS_STRING520;i<=IDS_STRING526;i++){
		strTemp.LoadStringW(i);
		strl.push_back(strTemp);
	}
	this->SetHeader(strl);
	typelist[1]=eCombo;
	typelist[2]=typelist[3]=typelist[4]=typelist[5]=typelist[6]=eEditReal;
	maxd[2]=maxd[3]=maxd[4]=maxd[5]=maxd[6]=1000;

	// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_VOL_ONCE ; j <= IDS_STRING_VMS ; j++){
		strTemp.LoadStringW(j);
		cbstr[1].push_back(strTemp);
		AdjustWidth(1,strTemp);
	}

	return 0;
}


int ListCtrlSAP::Inserti(int iSelect)
{

	iSelect++;
	CString str;
	str.Format(L"%d",iSelect+1);
	iSelect=InsertItem( iSelect, str );
	EnsureVisible(iSelect, FALSE);
	for(iSelect++;iSelect<GetItemCount();iSelect++){
		str.Format(L"%d",iSelect+1);
		SetItemText(iSelect,0,str);
	}


	return 0;
}


int ListCtrlSAP::Deletei(int iSelect)
{

	if(iSelect!=-1) // valid item 	
	{
		DeleteItem( iSelect );
		EnsureVisible(iSelect, FALSE);
		for(;iSelect<GetItemCount();iSelect++){
			CString str;
			str.Format(L"%d",iSelect+1);
			SetItemText(iSelect,0,str);
		}
	}

	return 0;
}


void ListCtrlSAP::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here


	switch(pLVKeyDow->wVKey){
	case VK_DELETE:
		//Deletei(GetSelectionMark());
		OnPopupRemove();
		break;
	case VK_INSERT: 
		//Inserti(GetSelectionMark());
		OnPopupInsert();
		break;
	default:
		break;
	}


	*pResult = 0;
}


void ListCtrlSAP::OnPopupInsert()
{
	// TODO: Add your command handler code here

	ListCtrlAA::OnPopupInsert();

	int iSelect=GetSelectionMark();
	if(iSelect>0)
		iSelect--;
	for(;iSelect<GetItemCount();iSelect++){
		CString str;
		str.Format(L"%d",iSelect+1);
		SetItemText(iSelect,0,str);
	}

}


void ListCtrlSAP::OnPopupRemove()
{
	// TODO: Add your command handler code here

	ListCtrlAA::OnPopupRemove();

	int iSelect=GetSelectionMark();
	for(;iSelect<GetItemCount();iSelect++){
		CString str;
		str.Format(L"%d",iSelect+1);
		SetItemText(iSelect,0,str);
	}

}


int ListCtrlSAP::InsertItemSAP(int i, const sapitemA & si)
{
	CString strTemp;

	strTemp.Format(L"%d",i+1);
	int ri=InsertItem( i, strTemp );
	AdjustWidth(0,strTemp);

	//if(si.addType<0)
	//	strTemp.LoadStringW(IDS_STRING_VOL_ONCE);
	//else
	//	strTemp.LoadStringW(IDS_STRING_VOL_ONCE+si.addType);
	//SetItemText(i,1,strTemp);
	//AdjustWidth(this,1,strTemp);

	SetChoice(i,1,(si.addType<0?0:si.addType));

	strTemp.Format(L"%g",si.Sconc);
	SetItemText(i,2,strTemp);
	AdjustWidth(2,strTemp);

	strTemp.Format(L"%g",si.Aconc);
	SetItemText(i,3,strTemp);
	AdjustWidth(3,strTemp);

	strTemp.Format(L"%g",si.Lconc);
	SetItemText(i,4,strTemp);
	AdjustWidth(4,strTemp);

	strTemp.Format(L"%g",si.volconc);
	SetItemText(i,5,strTemp);
	AdjustWidth(5,strTemp);

	strTemp.Format(L"%g",si.endRatio);
	SetItemText(i,6,strTemp);
	AdjustWidth(6,strTemp);


	return ri;
}


bool ListCtrlSAP::GetItemSAP(int i, sapitemA & si)
{

	CString strTemp;

	//for(size_t i=0;i<nItem;i++){
	si.addType=GetChoice(i,1);

	strTemp=GetItemText(i,2);
	si.Sconc=_wtof(strTemp.GetBuffer());

	//strTemp.ReleaseBuffer(strTemp.GetAllocLength());

	strTemp=GetItemText(i,3);
	si.Aconc=_wtof(strTemp.GetBuffer());

	//strTemp.ReleaseBuffer(strTemp.GetAllocLength());

	strTemp=GetItemText(i,4);
	si.Lconc=_wtof(strTemp.GetBuffer());

	//strTemp.ReleaseBuffer(strTemp.GetAllocLength());

	strTemp=GetItemText(i,5);
	si.volconc=_wtof(strTemp.GetBuffer());

	//strTemp.ReleaseBuffer(strTemp.GetAllocLength());

	strTemp=GetItemText(i,6);
	si.endRatio=_wtof(strTemp.GetBuffer());

	//strTemp.ReleaseBuffer(strTemp.GetAllocLength());

	return true;
}
