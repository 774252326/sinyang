#pragma once
#include "listctrlc.hpp"
//#include "UserAccount.h"
//#include "../struct/UserAccount.hpp"
#include "../user/UserAccount.hpp"
#include "../resource.h"

#include "../messagemaphpp.h"

class ListCtrlUA :
	public ListCtrlC
{

protected:
	bool bEditable;
public:
	void SetEditable(bool b){bEditable=b;};

	ListCtrlUA(void)
		: bEditable(false)
	{};
	ListCtrlUA(bool flg)
	{
		bEditable=flg;
	};
	virtual ~ListCtrlUA(void){};
		int InsertItemUA(int i, const UserAccount & ua, bool bUse)
	{

		int inserti=InsertItem( i, L"" );

		SetItemText(i,1,ua.userName);
		AdjustWidth(1,ua.userName);

		if(bEditable){
			SetItemText(i,2,ua.passWord);
			AdjustWidth(2,ua.passWord);
		}
		else{
			SetItemText(i,2,L"*");
		}

		SetChoice(i,3,ua.au);

		SetChoice(i,4,bUse?0:1);
		AdjustWidth(4,i);

		SetItemText(i,5,ua.remark);
		AdjustWidth(5,ua.remark);

		return inserti;
	};
	bool GetItemUA(int i, UserAccount & ua, bool & bUse)
	{
		bUse=false;

		ua.userName=this->GetItemText(i,1);
		ua.passWord=this->GetItemText(i,2);

		int ci=this->GetChoice(i,3);

		switch(ci){
		case 0:
			ua.au=UserAccount::authority::admin;
			break;
		case 1:
			ua.au=UserAccount::authority::user;
			break;
		case 2:
			ua.au=UserAccount::authority::guest;
			break;
		default:
			return false;
		}

		bUse=(this->GetChoice(i,4)==0);

		ua.remark=GetItemText(i,5);

		return true;
	};
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (ListCtrlC::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		CString strTemp;
		std::vector<CString> strl(1,CString());
		for(int i=IDS_STRING_USER_NAME;i<=IDS_STRING_REMARK;i++){
			strTemp.LoadStringW(i);
			strl.push_back(strTemp);
		}

		this->SetHeader(strl);

		if(bEditable){		
			typelist[1]=typelist[2]=typelist[5]=eEdit;
			typelist[3]=eCombo;
		}

		// insert string elements  for the ComboBox : 
		for ( int j=IDS_STRING_ADMIN ; j <= IDS_STRING_GUEST ; j++){
			strTemp.LoadStringW(j);
			cbstr[3].push_back(strTemp);
			AdjustWidth(3,strTemp);
		}

		for ( int j=IDS_STRING_YES ; j <= IDS_STRING_NO ; j++){
			strTemp.LoadStringW(j);
			cbstr[4].push_back(strTemp);
			AdjustWidth(4,strTemp);
		}

		return 0;
	};

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		if(bEditable){
			ClientToScreen(&point);
			OnContextMenu(this, point);
			ListCtrlC::OnRButtonUp(nFlags, point);
		}
	};
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point)
	{
		// TODO: Add your message handler code here
		if(bEditable){
			CMenu menu;
			menu.LoadMenu(IDR_LISTUA_POPUP);
			CMenu* pSumMenu = menu.GetSubMenu(0);
			pSumMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			UpdateDialogControls(this, FALSE);
			SetFocus();
		}
	};


	afx_msg void OnPopupDeleteuser()
	{
		// TODO: Add your command handler code here

		int iSelect=GetSelectionMark();
		if(iSelect!=-1) // valid item 	
		{
			int ci=GetChoice(iSelect,4);
			if(ci==1){
				DeleteItem( iSelect );
				EnsureVisible(iSelect, FALSE);
			}
		}

	};
	afx_msg void OnPopupAdduser()
	{
		// TODO: Add your command handler code here

		int iSelect=GetSelectionMark();
		iSelect=InsertItemUA( iSelect<0?0:iSelect, UserAccount(), false );
		EnsureVisible(iSelect, FALSE);

	};

//	DECLARE_MESSAGE_MAP()
//};
//
//BEGIN_MESSAGE_MAP(ListCtrlUA, ListCtrlC)
//	ON_WM_CREATE()
//	ON_WM_RBUTTONUP()
//	ON_WM_CONTEXTMENU()
//	ON_COMMAND(32810, &ListCtrlUA::OnPopupDeleteuser)
//	ON_COMMAND(32809, &ListCtrlUA::OnPopupAdduser)
//END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP_HPP(ListCtrlUA, ListCtrlC)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(32810, &ListCtrlUA::OnPopupDeleteuser)
	ON_COMMAND(32809, &ListCtrlUA::OnPopupAdduser)
END_MESSAGE_MAP_HPP()
};
