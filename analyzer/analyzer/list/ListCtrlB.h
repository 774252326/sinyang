#pragma once
#include "listctrla.hpp"
#include "EditB.h"

class ListCtrlB :
	public ListCtrlA
{
	DECLARE_DYNAMIC(ListCtrlB)

protected:
	EditB m_itemEdit;

public:
	ListCtrlB(void);
	virtual ~ListCtrlB(void);

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

	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()


};

