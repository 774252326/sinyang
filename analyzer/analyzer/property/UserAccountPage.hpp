#pragma once

//#include "UserAccount.h"
//#include "ListCtrlUA.h"

//#include <vector>
//#include "AccountList.h"

#include "../user/AccountList.hpp"
#include "../list\ListCtrlUA.hpp"
#include "../Resource.h"
#include "afxdialogex.h"

#include "../messagemaphpp.h"

// UserAccountPage dialog

class UserAccountPage : public CPropertyPage
{
	//DECLARE_DYNAMIC(UserAccountPage)
public:

	ListCtrlUA userList;
	AccountList al;
	int useIndex;

	// Dialog Data
	enum { IDD = IDD_DIALOG_UAP };
public:
	UserAccountPage()
		: CPropertyPage(UserAccountPage::IDD)
		, useIndex(-1)
	{
		CString title;
		title.LoadStringW(IDS_STRING_USER_ACCOUNT);
		m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
		m_psp.pszTitle = new TCHAR[title.GetLength()+1];
		_tcscpy((wchar_t*)m_psp.pszTitle, title);
	};
	virtual ~UserAccountPage()
	{
		delete [] m_psp.pszTitle;
	};


	protected:
	void SetList(void)
	{	
		//ListCtrlUA *pul=(ListCtrlUA*)GetDlgItem(IDS_STRING_LIST_ACCOUNT);
		for(size_t i=0;i<al.ual.size();i++){
			//pul->InsertItemUA(i,al.ual[i],(i==useIndex));
			userList.InsertItemUA(i,al.ual[i],(i==useIndex));
		}
	};

	void GetList(void)
	{
		if(al.IsAdmin(useIndex)){

			int nItem=userList.GetItemCount();
			al.ual.resize(nItem);
			//ListCtrlUA *pul=(ListCtrlUA*)GetDlgItem(IDS_STRING_LIST_ACCOUNT);
			useIndex=-2;

			for(size_t i=0;i<al.ual.size();i++){
				bool buse=false;
				userList.GetItemUA(i,al.ual[i],buse);
				//pul->GetItemUA(i,al.ual[i],buse);
				if(buse){
					useIndex=i;
				}
			}
		}

	};

	virtual BOOL OnSetActive()
	{
		// TODO: Add your specialized code here and/or call the base class

		return CPropertyPage::OnSetActive();
	};

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		CSize gap1(20,20);
		//CSize gap2(20,20);
		//CSize staticSize(150,22);

		CRect winrect;
		this->GetWindowRect(&winrect);
		winrect.DeflateRect(gap1);
		CPoint pt(gap1);

		const DWORD dwStyle = WS_VISIBLE 
			| WS_CHILD 
			| WS_BORDER
			| WS_HSCROLL 
			| WS_VSCROLL 
			| LBS_NOINTEGRALHEIGHT;

		//ListCtrlUA *pul=new ListCtrlUA(al.IsAdmin(useIndex));
		userList.SetEditable(al.IsAdmin(useIndex));

		//if(!pul->Create(dwStyle, CRect(pt,winrect.Size()), this, IDS_STRING_LIST_ACCOUNT) ){
		if(!userList.Create(dwStyle, CRect(pt,winrect.Size()), this, IDS_STRING_LIST_ACCOUNT) ){
			TRACE0("Failed to create output windows\n");
			return -1;      // fail to create
		}
		
		SetList();

		return 0;
	};

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CPropertyPage::DoDataExchange(pDX);
	};    // DDX/DDV support

	virtual BOOL OnKillActive()
	{
		// TODO: Add your specialized code here and/or call the base class
		GetList();

		return CPropertyPage::OnKillActive();
	};

	virtual BOOL OnWizardFinish()
	{
		// TODO: Add your specialized code here and/or call the base class
		GetList();

		return CPropertyPage::OnWizardFinish();
	};

//	DECLARE_MESSAGE_MAP()
//
//};
//
//BEGIN_MESSAGE_MAP(UserAccountPage, CPropertyPage)
//	ON_WM_CREATE()
//END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP_HPP(UserAccountPage, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP_HPP()

};