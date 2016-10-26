// ListCtrlLine.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ListCtrlLine.h"


// ListCtrlLine

IMPLEMENT_DYNAMIC(ListCtrlLine, CListCtrl)

	ListCtrlLine::ListCtrlLine()
{

}

ListCtrlLine::~ListCtrlLine()
{
}


BEGIN_MESSAGE_MAP(ListCtrlLine, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ListCtrlLine::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlLine::OnNMDblclk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()



// ListCtrlLine message handlers




int ListCtrlLine::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	//this->ModifyStyle(NULL,LVS_LIST);
	//this->ModifyStyleEx(NULL,LVS_EX_CHECKBOXES /*| LVS_EX_INFOTIP*/);  

	SetExtendedStyle(GetExtendedStyle() | LVS_EX_CHECKBOXES /*| LVS_EX_INFOTIP*/);
	
	return 0;
}


void ListCtrlLine::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here

	if(pDispInfo->item.pszText!=NULL){        
		SetItemText(pDispInfo->item.iItem, 0, pDispInfo->item.pszText);        
		rde.title[pDispInfo->item.iItem]=pDispInfo->item.pszText;
	}

	*pResult = 0;
}


void ListCtrlLine::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	CString str;
	str.Format(L"n=%d",pNMItemActivate->iItem);
	AfxMessageBox(str);

	*pResult = 0;
}


void ListCtrlLine::SetList(void)
{
	this->DeleteAllItems();
	for(size_t i=0;i<rde.ll.size();i++){
		this->InsertItem(i,rde.title[i]);
		this->SetCheck(i);
	}
}


void ListCtrlLine::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnClose();
}


BOOL ListCtrlLine::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CListCtrl::DestroyWindow();
}
