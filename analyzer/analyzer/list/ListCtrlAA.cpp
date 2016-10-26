#include "StdAfx.h"
#include "ListCtrlAA.h"
#include "../resource.h"


//add popup menu support for ListCtrlA


ListCtrlAA::ListCtrlAA(void)
{
}


ListCtrlAA::~ListCtrlAA(void)
{
}
BEGIN_MESSAGE_MAP(ListCtrlAA, ListCtrlA)
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_INSERT, &ListCtrlAA::OnPopupInsert)
	ON_COMMAND(ID_POPUP_REMOVE, &ListCtrlAA::OnPopupRemove)
	ON_COMMAND(ID_POPUP_REMOVEALL, &ListCtrlAA::OnPopupRemoveall)
	//	ON_NOTIFY_REFLECT(LVN_KEYDOWN, &ListCtrlAA::OnLvnKeydown)
END_MESSAGE_MAP()


void ListCtrlAA::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	ClientToScreen(&point);
	OnContextMenu(this, point);


	ListCtrlA::OnRButtonUp(nFlags, point);
}


void ListCtrlAA::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here

	CMenu menu;
	menu.LoadMenu(IDR_LIST_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	//if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
	{
		//CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		//if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
		//return;


		pSumMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

		//pSumMenu->DestroyMenu();


		//((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);

		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}


void ListCtrlAA::OnPopupInsert()
{
	// TODO: Add your command handler code here

	int iSelect=GetSelectionMark();
	CString str=L"";
	//str.Format(L"%d",iSelect+1);
	iSelect=InsertItem( iSelect<0?GetItemCount():iSelect, str );
	EnsureVisible(iSelect, FALSE);
}


void ListCtrlAA::OnPopupRemove()
{
	// TODO: Add your command handler code here

	int iSelect=GetSelectionMark();
	if(iSelect!=-1) // valid item 	
	{
		DeleteItem( iSelect );
		EnsureVisible(iSelect, FALSE);
		//for(;iSelect<GetItemCount();iSelect++){
		//CString str;
		//str.Format(L"%d",iSelect+1);
		//SetItemText(iSelect,0,str);
		//}
	}
}


void ListCtrlAA::OnPopupRemoveall()
{
	// TODO: Add your command handler code here

	this->DeleteAllItems();
}


//void ListCtrlAA::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//
//
//
//
//
//
//	*pResult = 0;
//}
