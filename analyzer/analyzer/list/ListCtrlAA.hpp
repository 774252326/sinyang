#pragma once
#include "listctrlc.hpp"

#include "../resource.h"

#include "../messagemaphpp.h"
///
/// \brief The ListCtrlAA class
///增加右键弹出菜单的可编辑表格
///
class ListCtrlAA :
	public ListCtrlC
{
public:
	ListCtrlAA(void){};
	virtual ~ListCtrlAA(void){};

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		ClientToScreen(&point);
		OnContextMenu(this, point);


		ListCtrlA::OnRButtonUp(nFlags, point);
	};


	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point)
	{
		// TODO: Add your message handler code here

		CMenu menu;
		menu.LoadMenu(IDR_LIST_POPUP);

		CMenu* pSumMenu = menu.GetSubMenu(0);

		{
			pSumMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

			UpdateDialogControls(this, FALSE);
		}

		SetFocus();
	};


	afx_msg void OnPopupInsert()
	{
		// TODO: Add your command handler code here

		int iSelect=GetSelectionMark();
		CString str=L"";
		//str.Format(L"%d",iSelect+1);
		iSelect=InsertItem( iSelect<0?GetItemCount():iSelect, str );
		EnsureVisible(iSelect, FALSE);
	};


	afx_msg void OnPopupRemove()
	{
		// TODO: Add your command handler code here

		int iSelect=GetSelectionMark();
		if(iSelect!=-1) // valid item 	
		{
			DeleteItem( iSelect );
			EnsureVisible(iSelect, FALSE);
		}
	};


	afx_msg void OnPopupRemoveall()
	{
		// TODO: Add your command handler code here

		this->DeleteAllItems();
	};

	//	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);	
	//DECLARE_MESSAGE_MAP()

	BEGIN_MESSAGE_MAP_HPP(ListCtrlAA, ListCtrlC)
		ON_WM_RBUTTONUP()
		ON_WM_CONTEXTMENU()
		ON_COMMAND(32806, &ListCtrlAA::OnPopupInsert)
		ON_COMMAND(32807, &ListCtrlAA::OnPopupRemove)
		ON_COMMAND(32808, &ListCtrlAA::OnPopupRemoveall)
		//	ON_NOTIFY_REFLECT(LVN_KEYDOWN, &ListCtrlAA::OnLvnKeydown)
		END_MESSAGE_MAP_HPP()

};

