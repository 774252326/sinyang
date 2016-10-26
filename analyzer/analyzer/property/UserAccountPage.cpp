// UserAccountPage.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "../Resource.h"
#include "UserAccountPage.h"
#include "afxdialogex.h"
//#include "func.h"

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
	//ListCtrlUA *pul=(ListCtrlUA*)GetDlgItem(IDS_STRING_LIST_ACCOUNT);
	//delete pul;
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

	//ListCtrlUA *pul=new ListCtrlUA(al.IsAdmin(useIndex));
	userList.SetEditable(al.IsAdmin(useIndex));

	//if(!pul->Create(dwStyle, CRect(pt,winrect.Size()), this, IDS_STRING_LIST_ACCOUNT) ){
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

	CString str;
	str.LoadStringW(IDS_STRING_OK);
	psheet->SetFinishText(str);


	return CPropertyPage::OnSetActive();
}


void UserAccountPage::SetList(void)
{	
	//ListCtrlUA *pul=(ListCtrlUA*)GetDlgItem(IDS_STRING_LIST_ACCOUNT);
	for(size_t i=0;i<al.ual.size();i++){
		//pul->InsertItemUA(i,al.ual[i],(i==useIndex));
		userList.InsertItemUA(i,al.ual[i],(i==useIndex));
	}
}


void UserAccountPage::GetList(void)
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

}


BOOL UserAccountPage::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class
	GetList();

	return CPropertyPage::OnKillActive();
}


BOOL UserAccountPage::OnWizardFinish()
{
	// TODO: Add your specialized code here and/or call the base class
	GetList();

	return CPropertyPage::OnWizardFinish();
}
