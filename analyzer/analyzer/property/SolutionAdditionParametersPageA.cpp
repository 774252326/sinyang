// SolutionAdditionParametersPageA.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "../Resource.h"
#include "SolutionAdditionParametersPageA.h"
#include "afxdialogex.h"
//#include "func.h"

// SolutionAdditionParametersPageA dialog

IMPLEMENT_DYNAMIC(SolutionAdditionParametersPageA, CPropertyPage)

SolutionAdditionParametersPageA::SolutionAdditionParametersPageA()
	: CPropertyPage(SolutionAdditionParametersPageA::IDD)
	//, typeidx(1)
	//, totaln(7)
	//, strtmp(_T(""))
{
	CString title;
	title.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);

m_psp.dwFlags &=~PSP_HASHELP;
}

SolutionAdditionParametersPageA::~SolutionAdditionParametersPageA()
{
	delete [] m_psp.pszTitle;
}

void SolutionAdditionParametersPageA::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SolutionAdditionParametersPageA, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()


void SolutionAdditionParametersPageA::SetList(void)
{

	//UpdateData(FALSE);

	m_SAPlist.DeleteAllItems();

	for(size_t i=0;i<para.saplist.size();i++){
		m_SAPlist.InsertItemSAP(i,para.saplist[i]);
	}
}


void SolutionAdditionParametersPageA::GetList(void)
{
	//UpdateData(TRUE);

	int nItem=m_SAPlist.GetItemCount();

	para.saplist.resize(nItem);

	for(size_t i=0;i<nItem;i++){
		m_SAPlist.GetItemSAP(i,para.saplist[i]);
	}

}

BOOL SolutionAdditionParametersPageA::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	GetList();

	return CPropertyPage::OnKillActive();
}


BOOL SolutionAdditionParametersPageA::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

		// 获得父窗口，即属性表CPropertySheet类   
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

	//设置属性表只有“完成”按钮   
	//psheet->SetFinishText(_T("完成"));  

	return CPropertyPage::OnSetActive();
}


int SolutionAdditionParametersPageA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

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
