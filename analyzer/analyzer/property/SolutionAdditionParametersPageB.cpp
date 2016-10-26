// SolutionAdditionParametersPageB.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "../Resource.h"
#include "SolutionAdditionParametersPageB.h"
#include "afxdialogex.h"
//#include  "filefunc.h"

// SolutionAdditionParametersPageB dialog

IMPLEMENT_DYNAMIC(SolutionAdditionParametersPageB, CPropertyPage)

SolutionAdditionParametersPageB::SolutionAdditionParametersPageB()
	: CPropertyPage(SolutionAdditionParametersPageB::IDD)
	//, mf(NULL)
{
	CString title;
	title.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);

	m_psp.dwFlags &=~PSP_HASHELP;
}

SolutionAdditionParametersPageB::~SolutionAdditionParametersPageB()
{
	delete [] m_psp.pszTitle;
}

void SolutionAdditionParametersPageB::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SolutionAdditionParametersPageB, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// SolutionAdditionParametersPageB message handlers

void SolutionAdditionParametersPageB::SetList(void)
{

	m_SAPlist.DeleteAllItems();

	for(size_t i=0;i<para.saplist.size();i++){
		m_SAPlist.InsertItemSAP(i,para.saplist[i]);
	}


}


void SolutionAdditionParametersPageB::GetList(void)
{
	//UpdateData(TRUE);

	int nItem=m_SAPlist.GetItemCount();

	para.saplist.resize(nItem);


	for(size_t i=0;i<nItem;i++){
		m_SAPlist.GetItemSAP(i,para.saplist[i]);
	}

}

int SolutionAdditionParametersPageB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here




	//CSize gap1(20,20);
	//CSize gap2(20,20);
	////CSize staticSize(150,22);

	//CRect winrect;
	//this->GetWindowRect(&winrect);
	//winrect.DeflateRect(gap1);
	//CPoint pt(gap1);
	////CStatic *pStatic;
	////CEdit *pEdit;
	////CString str;

	//CSize listsz=winrect.Size();
	//listsz.cy-=gap2.cy;
	//listsz.cy/=2;


	////str.LoadStringW(IDS_STRING_SAP_TIPS);
	////stt.Create(
	////	str,
	////	WS_CHILD
	////	|WS_VISIBLE, 
	////	CRect(pt,CSize(winrect.Width(),staticSize.cy)),
	////	this,
	////	10000);

	////pt.y+=gap2.cy+staticSize.cy;


	//const DWORD dwStyle = WS_VISIBLE 
	//	| WS_CHILD 
	//	| WS_BORDER
	//	| WS_HSCROLL 
	//	| WS_VSCROLL 
	//	| LBS_NOINTEGRALHEIGHT;

	//if(!m_SAPlist0.Create(dwStyle, CRect(pt,listsz), this, IDS_LISTCTRL_SAP_0) ){
	//	TRACE0("Failed to create output windows\n");
	//	return -1;      // fail to create
	//}

	//pt.y+=gap2.cy+listsz.cy;

	//if(!m_SAPlist1.Create(dwStyle, CRect(pt,listsz), this, IDS_LISTCTRL_SAP_1) ){
	//	TRACE0("Failed to create output windows\n");
	//	return -1;      // fail to create
	//}


	//

	//for(size_t i=0;i<para0.saplist.size();i++){
	//	m_SAPlist0.InsertItemSAP(i,para0.saplist[i]);
	//}


	////size_t nc=m_SAPlist0.typelist.size();

	////m_SAPlist0.typelist.assign(nc,m_SAPlist0.eStatic);


	//SetList();
	///////////////////////////////////////////////////////////////////////////////////

		CSize gap1(20,20);
	CSize gap2(20,20);
	CSize staticSize(150,22);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);
	CStatic *pStatic;
	CEdit *pEdit;
	CString str;

	str.LoadStringW(IDS_STRING_SAP_TIPS);
	stt.Create(
		str,
		WS_CHILD
		|WS_VISIBLE, 
		CRect(pt,CSize(winrect.Width(),staticSize.cy)),
		this,
		10000);

	pt.y+=gap2.cy+staticSize.cy;


	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	if(!m_SAPlist.Create(dwStyle, CRect(pt,CPoint(winrect.Size()+gap1)), this, IDS_LISTCTRL_SAP) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	SetList();



	return 0;
}


BOOL SolutionAdditionParametersPageB::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class


	GetList();


	return CPropertyPage::OnKillActive();
}


BOOL SolutionAdditionParametersPageB::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

			// 获得父窗口，即属性表CPropertySheet类   
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

	return CPropertyPage::OnSetActive();
}


BOOL SolutionAdditionParametersPageB::OnApply()
{
	// TODO: Add your specialized code here and/or call the base class

	//pDoc->bChangeSAP=true;
	//pDoc->p3todo=para;
	pDoc->SetSAPtodo(para);

	//::PostMessage(mf->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);

	return CPropertyPage::OnApply();
}


void SolutionAdditionParametersPageB::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//::SendMessage(mf->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);

	CPropertyPage::OnCancel();
}
