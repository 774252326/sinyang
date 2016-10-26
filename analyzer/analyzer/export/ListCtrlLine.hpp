#pragma once
//#include "rawdataex.h"
#include "struct1\RawDataEx.hpp"

// ListCtrlLine

class ListCtrlLine : public CListCtrl
{
	//DECLARE_DYNAMIC(ListCtrlLine)

public:
	RawDataEx rde;
	ListCtrlLine(){};
	virtual ~ListCtrlLine(){};
	void SetList(void)
	{
		this->DeleteAllItems();
		for(size_t i=0;i<rde.ll.size();i++){
			this->InsertItem(i,rde.title[i]);
			this->SetCheck(i);
		}
	};
protected:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CListCtrl::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		//this->ModifyStyle(NULL,LVS_LIST);
		//this->ModifyStyleEx(NULL,LVS_EX_CHECKBOXES /*| LVS_EX_INFOTIP*/);  

		SetExtendedStyle(GetExtendedStyle() | LVS_EX_CHECKBOXES /*| LVS_EX_INFOTIP*/);

		return 0;
	};

	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
	{
		NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
		// TODO: Add your control notification handler code here

		if(pDispInfo->item.pszText!=NULL){        
			SetItemText(pDispInfo->item.iItem, 0, pDispInfo->item.pszText);        
			rde.title[pDispInfo->item.iItem]=pDispInfo->item.pszText;
		}

		*pResult = 0;
	};

	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		// TODO: Add your control notification handler code here

		//CString str;
		//str.Format(L"n=%d",pNMItemActivate->iItem);
		//AfxMessageBox(str);

		this->EditLabel(pNMItemActivate->iItem);

		*pResult = 0;
	};

	DECLARE_MESSAGE_MAP()



};

BEGIN_MESSAGE_MAP(ListCtrlLine, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ListCtrlLine::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlLine::OnNMDblclk)
	//ON_WM_CLOSE()
END_MESSAGE_MAP()

