// UserAccountPage.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "UserAccountPage.h"
#include "afxdialogex.h"
#include "func.h"

// UserAccountPage dialog

IMPLEMENT_DYNAMIC(UserAccountPage, CPropertyPage)

UserAccountPage::UserAccountPage()
	: CPropertyPage(UserAccountPage::IDD)
	, useIndex(-1)
{
	CString title;
	title.LoadStringW(IDS_STRING_USER_ACCOUNT);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}

UserAccountPage::~UserAccountPage()
{
	delete [] m_psp.pszTitle;
}

void UserAccountPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(UserAccountPage, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// UserAccountPage message handlers


int UserAccountPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	if(!userList.Create(dwStyle, CRect(pt,winrect.Size()), this, IDS_STRING_LIST_ACCOUNT) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

		SetList();

	return 0;
}


BOOL UserAccountPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	 CPropertySheet* psheet = (CPropertySheet*) GetParent();   
     psheet->SetWizardButtons(PSWIZB_FINISH);
     psheet->SetFinishText(_T("OK"));


	return CPropertyPage::OnSetActive();
}


void UserAccountPage::SetList(void)
{
	
	for(size_t i=0;i<ual.size();i++){

		//userList.InsertItem( i, ual[i].userName );
		//AdjustWidth(&userList,0,ual[i].userName);

		//userList.SetItemText(i,1,ual[i].passWord);
		//AdjustWidth(&userList,1,ual[i].passWord);

		//CString strTemp;
		//strTemp.LoadString(IDS_STRING_ADMIN+ual[i].au);
		//userList.SetItemText(i, 2, strTemp);
		//AdjustWidth(&userList,2,strTemp);

		//if(i==useIndex){
		//	strTemp.LoadString(IDS_STRING_YES);
		//	userList.bEditable=(ual[i].au==admin);	
		//}
		//else{
		//	strTemp.LoadString(IDS_STRING_NO);
		//}
		//userList.SetItemText(i,3,strTemp);
		//AdjustWidth(&userList,3,strTemp);

		//userList.SetItemText(i,4,ual[i].remark);
		//AdjustWidth(&userList,4,ual[i].remark);

		userList.InsertItemUA(i,ual[i],(i==useIndex));

	}


	
}


void UserAccountPage::GetList(void)
{
	int nItem=userList.GetItemCount();
	ual.resize(nItem);
	for(size_t i=0;i<ual.size();i++){
		userList.GetItemUA(i,ual[i]);
	}

}


BOOL UserAccountPage::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	GetList();

	return CPropertyPage::OnKillActive();
}
