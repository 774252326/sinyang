#include "StdAfx.h"
#include "ListCtrlB.h"

IMPLEMENT_DYNAMIC(ListCtrlB, ListCtrlA)

	ListCtrlB::ListCtrlB(void)
{
}


ListCtrlB::~ListCtrlB(void)
{
}

BEGIN_MESSAGE_MAP(ListCtrlB, ListCtrlA)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ListCtrlB::OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ListCtrlB::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlB::OnNMDblclk)
END_MESSAGE_MAP()

void ListCtrlB::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	BeginEdit(m_itemEdit);

	*pResult = 0;
}


void ListCtrlB::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here

	EndEdit(m_itemEdit,pDispInfo->item.pszText);

	ModifyStyle(LVS_EDITLABELS,0);
	*pResult = 0;
}


void ListCtrlB::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	m_itemEdit.coord.SetPoint(pNMItemActivate->iItem,pNMItemActivate->iSubItem);
	ModifyStyle(0,LVS_EDITLABELS);
	EditLabel(pNMItemActivate->iItem);

	*pResult = 0;
}


BOOL ListCtrlB::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	HD_NOTIFY* pHDN = (HD_NOTIFY*)lParam;   
	switch(((NMHDR*)lParam)->code)  
	{  
	case HDN_BEGINTRACKW://�϶�  
	case HDN_BEGINTRACKA:  
	case HDN_DIVIDERDBLCLICK://˫��  
		if(pHDN->iItem  == 0)  // Col���趨Ϊ�Լ�����ı����ֵ������Col=0�����ǵ�1��  
		{  
			*pResult = TRUE;  
			return TRUE;  
		}  
	default:
		return ListCtrlA::OnNotify(wParam, lParam, pResult);
		//return CListCtrl::OnNotify(wParam, lParam, pResult);
	} 	
}

int ListCtrlB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	SetStyle();

	return 0;
}