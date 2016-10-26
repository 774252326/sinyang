#include "StdAfx.h"
#include "ListCtrlSAPB.h"
#include "func.h"
#include "resource.h"

ListCtrlSAPB::ListCtrlSAPB(void)
{
}


ListCtrlSAPB::~ListCtrlSAPB(void)
{
}
BEGIN_MESSAGE_MAP(ListCtrlSAPB, ListCtrlAA)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, &ListCtrlSAPB::OnLvnKeydown)
	ON_COMMAND(ID_POPUP_INSERT, &ListCtrlSAPB::OnPopupInsert)
	ON_COMMAND(ID_POPUP_REMOVE, &ListCtrlSAPB::OnPopupRemove)
	//	ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlSAPB::OnNMDblclk)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


int ListCtrlSAPB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CString strTemp;
	std::vector<CString> strl;

	strTemp=L"file";
	strl.push_back(strTemp);

	for(int i=IDS_STRING524;i<=IDS_STRING525;i++){
		strTemp.LoadStringW(i);
		strl.push_back(strTemp);
	}

	this->SetHeader(strl);
	typelist[0]=eEdit;
	typelist[1]=eCombo;
	typelist[2]=typelist[3]=typelist[4]=typelist[5]=eEditReal;
	maxd[2]=maxd[3]=maxd[4]=maxd[5]=1000;

	// insert string elements  for the ComboBox : 
	//for ( int j=IDS_STRING_VOL_ONCE ; j <= IDS_STRING_VMS ; j++){
	strTemp.LoadStringW(IDS_STRING_VOL_ONCE);
	cbstr[1].push_back(strTemp);
	AdjustWidth(this,1,strTemp);
	//}

	strTemp.LoadStringW(IDS_STRING_VMS);
	cbstr[1].push_back(strTemp);
	AdjustWidth(this,1,strTemp);

	return 0;
}


int ListCtrlSAPB::Inserti(int iSelect)
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


int ListCtrlSAPB::Deletei(int iSelect)
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


void ListCtrlSAPB::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
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


void ListCtrlSAPB::OnPopupInsert()
{
	// TODO: Add your command handler code here



	TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");

	// Create an Open dialog; the default file name extension is ".my".

	CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 

	// returns IDOK.

	if(fileDlg.DoModal() == IDOK)
	{   
		int iSelect=GetSelectionMark();	
		if(iSelect<0)
			iSelect=this->GetItemCount(); 

		CString str=fileDlg.GetPathName();


		ListCtrlAA::OnPopupInsert();    

		SetItemText(iSelect,0,str);

		AdjustWidth(this,0,str);

		//POSITION pos=fileDlg.GetStartPosition();

		//while(pos!=NULL){
		// CString fp=fileDlg.GetNextPathName(pos);
		// //int nIndex=FindStringExact(0, fp);
		// //if(nIndex == LB_ERR)
		//	//this->AddString(fp);
		//}  
	}


}


void ListCtrlSAPB::OnPopupRemove()
{
	// TODO: Add your command handler code here

	ListCtrlAA::OnPopupRemove();

	//int iSelect=GetSelectionMark();
	//for(;iSelect<GetItemCount();iSelect++){
	//	CString str;
	//	str.Format(L"%d",iSelect+1);
	//	SetItemText(iSelect,0,str);
	//}

}


int ListCtrlSAPB::InsertItemSAPB(int i, const sapitemF & si)
{
	//	CString strTemp;

	////for(size_t i=0;i<para.saplist.size();i++){
	//	strTemp.Format(L"%d",i+1);
	//	int ri=InsertItem( i, strTemp );
	//	AdjustWidth(this,0,strTemp);

	//	if(si.addType<0)
	//		strTemp.LoadStringW(IDS_STRING_VOL_ONCE);
	//	else
	//		strTemp.LoadStringW(IDS_STRING_VOL_ONCE+si.addType);
	//	SetItemText(i,1,strTemp);
	//	AdjustWidth(this,1,strTemp);

	//	strTemp.Format(L"%g",si.Sconc);
	//	SetItemText(i,2,strTemp);
	//	AdjustWidth(this,2,strTemp);

	//	strTemp.Format(L"%g",si.Aconc);
	//	SetItemText(i,3,strTemp);
	//	AdjustWidth(this,3,strTemp);

	//	strTemp.Format(L"%g",si.Lconc);
	//	SetItemText(i,4,strTemp);
	//	AdjustWidth(this,4,strTemp);

	//	strTemp.Format(L"%g",si.volconc);
	//	SetItemText(i,5,strTemp);
	//	AdjustWidth(this,5,strTemp);

	//	strTemp.Format(L"%g",si.endRatio);
	//	SetItemText(i,6,strTemp);
	//	AdjustWidth(this,6,strTemp);


	//return ri;
	return 0;
}


bool ListCtrlSAPB::GetItemSAPB(int i, sapitemF & si)
{

	CString strTemp;

	si.file=GetItemText(i,0);

	//for(size_t i=0;i<nItem;i++){
	si.sia.addType=GetChoice(i,1);
	if(si.sia.addType!=0)
		si.sia.addType=4;

	strTemp=GetItemText(i,2);
	si.sia.Sconc=_wtof(strTemp.GetBuffer());

	strTemp=GetItemText(i,3);
	si.sia.Aconc=_wtof(strTemp.GetBuffer());

	strTemp=GetItemText(i,4);
	si.sia.Lconc=_wtof(strTemp.GetBuffer());

	strTemp=GetItemText(i,5);
	si.sia.volconc=_wtof(strTemp.GetBuffer());

	//strTemp=GetItemText(i,6);
	//si.endRatio=_wtof(strTemp.GetBuffer());

	return true;
}


//void ListCtrlSAPB::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//
//
//
//
//	*pResult = 0;
//}


void ListCtrlSAPB::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	//int iSelect=GetSelectionMark();	
	//if(iSelect>=0){

	//	TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");

	//	// Create an Open dialog; the default file name extension is ".my".

	//	CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
	//		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);

	//	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 

	//	// returns IDOK.

	//	if(fileDlg.DoModal() == IDOK)
	//	{   

	//		CString str=fileDlg.GetPathName();
	//		SetItemText(iSelect,0,str);

	//		AdjustWidth(this,0,str);
	//	}
	//}


	ListCtrlAA::OnLButtonDblClk(nFlags, point);
}
