#include "StdAfx.h"
#include "ListCtrlUA.h"
#include "func.h"
#include "resource.h"

ListCtrlUA::ListCtrlUA(void)
	: bEditable(false)
	//, useIndex(-1)
{
}


ListCtrlUA::ListCtrlUA(bool flg)
	//: bEditable(false)
{
	bEditable=flg;
}

ListCtrlUA::~ListCtrlUA(void)
{
}
BEGIN_MESSAGE_MAP(ListCtrlUA, ListCtrlAA)
	ON_WM_CREATE()
	ON_COMMAND(ID_POPUP_INSERT, &ListCtrlUA::OnPopupInsert)
	ON_COMMAND(ID_POPUP_REMOVE, &ListCtrlUA::OnPopupRemove)
	ON_COMMAND(ID_POPUP_REMOVEALL, &ListCtrlUA::OnPopupRemoveall)
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


int ListCtrlUA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ListCtrlAA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	CString strTemp;
	std::vector<CString> strl;
	for(int i=IDS_STRING_USER_NAME;i<=IDS_STRING_REMARK;i++){
		strTemp.LoadStringW(i);
		strl.push_back(strTemp);
	}

	this->SetHeader(strl);

	//if(bEditable){
	//	typelist[2]=eCombo;
	//	typelist[0]=typelist[1]=typelist[4]=eEdit;

		// insert string elements  for the ComboBox : 
		for ( int j=IDS_STRING_ADMIN ; j <= IDS_STRING_GUEST ; j++){
			strTemp.LoadStringW(j);
			cbstr[2].push_back(strTemp);
			AdjustWidth(this,2,strTemp);
		}

		for ( int j=IDS_STRING_YES ; j <= IDS_STRING_NO ; j++){
			strTemp.LoadStringW(j);
			cbstr[3].push_back(strTemp);
			AdjustWidth(this,3,strTemp);
		}
	//}

	return 0;
}


void ListCtrlUA::OnPopupInsert()
{
	// TODO: Add your command handler code here
	//ListCtrlAA::OnPopupInsert();

	int iSelect=GetSelectionMark();
	iSelect=InsertItemUA( iSelect<0?0:iSelect, UserAccount(), false );
	EnsureVisible(iSelect, FALSE);

}


void ListCtrlUA::OnPopupRemove()
{
	// TODO: Add your command handler code here
	int iSelect=GetSelectionMark();
	CString strTemp;
	strTemp.LoadString(IDS_STRING_NO);
	if( GetItemText(iSelect,3)==strTemp ){
		ListCtrlAA::OnPopupRemove();
	}
}


void ListCtrlUA::OnPopupRemoveall()
{
	// TODO: Add your command handler code here
}


void ListCtrlUA::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(bEditable){
		ListCtrlAA::OnRButtonUp(nFlags, point);
	}
}


void ListCtrlUA::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	if(bEditable){
		ListCtrlAA::OnContextMenu(pWnd, point);
	}

}


int ListCtrlUA::InsertItemUA(int i, const UserAccount & ua, bool bUse)
{

	int inserti=InsertItem( i, ua.userName );
	AdjustWidth(this,0,ua.userName);

	SetItemText(i,1,ua.passWord);
	AdjustWidth(this,1,ua.passWord);

	CString strTemp;
	strTemp.LoadString(IDS_STRING_ADMIN+ua.au);
	SetItemText(i, 2, strTemp);
	AdjustWidth(this,2,strTemp);

	if(bUse){
		strTemp.LoadString(IDS_STRING_YES);
		bEditable=(ua.au==admin);	

		if(bEditable){
		
		typelist[0]=typelist[1]=typelist[4]=eEdit;

		typelist[2]=eCombo;
		//// insert string elements  for the ComboBox : 
		//for ( int j=IDS_STRING_ADMIN ; j <= IDS_STRING_GUEST ; j++){
		//	strTemp.LoadStringW(j);
		//	cbstr[2].push_back(strTemp);
		//	AdjustWidth(this,2,strTemp);
		//}
	}

	}
	else{
		strTemp.LoadString(IDS_STRING_NO);
	}
	SetItemText(i,3,strTemp);
	AdjustWidth(this,3,strTemp);

	SetItemText(i,4,ua.remark);
	AdjustWidth(this,4,ua.remark);

	return inserti;
}


bool ListCtrlUA::GetItemUA(int i, UserAccount & ua, bool & bUse)
{
	bUse=false;

	ua.userName=this->GetItemText(i,0);
	ua.passWord=this->GetItemText(i,1);
	
	int ci=this->GetChoice(i,2);

	switch(ci){
	case 0:
		ua.au=admin;
		break;
	case 1:
		ua.au=user;
		break;
	case 2:
		ua.au=guest;
		break;
	default:
		return false;
	}

	bUse=(this->GetChoice(i,3)==0);

	ua.remark=GetItemText(i,4);
	
	return true;
}


//void ListCtrlUA::GetList(void)
//{
//
//}
