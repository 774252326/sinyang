#pragma once
#include "listctrla.hpp"
#include "EditB.hpp"

#include "../messagemaphpp.h"

class ListCtrlB :
	public ListCtrlA
{
	//DECLARE_DYNAMIC(ListCtrlB)

protected:
	EditB m_itemEdit;

public:
	ListCtrlB(void){};
	virtual ~ListCtrlB(void){};

	BOOL SetHeader(const std::vector<CString> & headerstrl)
	{
		if(headerstrl.size()<2){
			return FALSE;
		}
		BOOL flg=ListCtrlA::SetHeader(headerstrl);
		SetColumnWidth(0,0);
		return flg;
	};
	BOOL AdjustWidth(int nCol, CString str, int gap=15)
	{
		if(nCol>0)
			return ListCtrlA::AdjustWidth(nCol,str,gap);
		return FALSE;
	};
	BOOL AdjustWidth(int nCol, int nRow, int gap=15)
	{
		if(nCol>0)
			return ListCtrlA::AdjustWidth(nCol,nRow,gap);
		return FALSE;
	};
	BOOL DeleteColumn(int nCol)
	{
		if(nCol>0)
			return ListCtrlA::DeleteColumn(nCol);
		return FALSE;
	};
	int InsertColumn(int nCol,const LVCOLUMN* pColumn)
	{
		if(nCol>0)
			return ListCtrlA::InsertColumn(nCol,pColumn);
		return -1;
	};
	int InsertColumn(int nCol,LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT,int nWidth = -1,int nSubItem = -1)
	{
		if(nCol>0)
			return ListCtrlA::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
		return -1;
	};
protected:

	void EndEdit(EditB &me,LPTSTR pszText)
	{
		if(me.coord.x >= 0 && me.coord.y >= 0)
		{  
			if(pszText!=NULL){  
				SetItemText(me.coord.x, me.coord.y, pszText);        
			}  
			VERIFY(me.UnsubclassWindow()!=NULL);   
		}
	};

	void BeginEdit(EditB &me)
	{
		if(me.coord.x >= 0 && me.coord.y >= 0)
		{
			//get edit control and subclass
			HWND hWnd= (HWND)SendMessage(LVM_GETEDITCONTROL);
			ASSERT(hWnd != NULL);
			VERIFY(me.SubclassWindow(hWnd));

			GetSubItemRect( me.coord.x, me.coord.y, LVIR_LABEL, me.rect);
			CString str=GetItemText(me.coord.x, me.coord.y);
			me.SetWindowText(str);	
		}
	};

	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
		{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	BeginEdit(m_itemEdit);

	*pResult = 0;
};

	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
		{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here

	EndEdit(m_itemEdit,pDispInfo->item.pszText);

	ModifyStyle(LVS_EDITLABELS,0);
	*pResult = 0;
};

	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
		{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	m_itemEdit.coord.SetPoint(pNMItemActivate->iItem,pNMItemActivate->iSubItem);
	ModifyStyle(0,LVS_EDITLABELS);
	EditLabel(pNMItemActivate->iItem);

	*pResult = 0;
};

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
		{
	// TODO: Add your specialized code here and/or call the base class

	HD_NOTIFY* pHDN = (HD_NOTIFY*)lParam;   
	switch(((NMHDR*)lParam)->code)  
	{  
	case HDN_BEGINTRACKW://拖动  
	case HDN_BEGINTRACKA:  
	case HDN_DIVIDERDBLCLICK://双击  
		if(pHDN->iItem  == 0)  // Col—设定为自己不想改变的列值，比如Col=0，就是第1列  
		{  
			*pResult = TRUE;  
			return TRUE;  
		}  
	default:
		return ListCtrlA::OnNotify(wParam, lParam, pResult);
		//return CListCtrl::OnNotify(wParam, lParam, pResult);
	} 	
};


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
	if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	SetStyle();

	return 0;
};

	BEGIN_MESSAGE_MAP_HPP(ListCtrlB, ListCtrlA)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ListCtrlB::OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ListCtrlB::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlB::OnNMDblclk)
END_MESSAGE_MAP_HPP()


};

