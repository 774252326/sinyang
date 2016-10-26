// SolutionAdditionParametersPageB.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "SolutionAdditionParametersPageB.h"
#include "afxdialogex.h"
#include "func.h"

// SolutionAdditionParametersPageB dialog

IMPLEMENT_DYNAMIC(SolutionAdditionParametersPageB, CPropertyPage)

SolutionAdditionParametersPageB::SolutionAdditionParametersPageB()
	: CPropertyPage(SolutionAdditionParametersPageB::IDD)
	, typeidx(1)
	, totaln(7)
{
	CString title;
	title.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);

	//typeidx=1;
	//totaln=7;
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



void SolutionAdditionParametersPageB::SetList(void)
{

	//UpdateData(FALSE);

	if(para.saplist.empty()){
		return;
	}



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

BOOL SolutionAdditionParametersPageB::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	GetList();

	return CPropertyPage::OnKillActive();
}


BOOL SolutionAdditionParametersPageB::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

		// ��ø����ڣ������Ա�CPropertySheet��   
	CPropertySheet* psheet = (CPropertySheet*) GetParent();   

	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

	//�������Ա�ֻ�С���ɡ���ť   
	//psheet->SetFinishText(_T("���"));  

	return CPropertyPage::OnSetActive();
}


int SolutionAdditionParametersPageB::OnCreate(LPCREATESTRUCT lpCreateStruct)
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



	//str.LoadStringW(IDS_STRING_VMS_VOLUME);
	//pStatic=new CStatic;
	//pStatic->Create(
	//	str,
	//	WS_CHILD
	//	|WS_VISIBLE, 
	//	CRect(pt,staticSize),
	//	this,
	//	IDS_STRING_VMS_VOLUME);

	//pt.x+=gap2.cx+staticSize.cx;

	//str.LoadStringW(IDS_EDIT_VMS_VOLUME);
	////str=L"0";
	//pEdit=new CEdit;
	//pEdit->CreateEx(
	//	WS_EX_CLIENTEDGE,
	//	L"Edit", 
	//	str,
	//	ES_LEFT
	//	|WS_CHILD
	//	|WS_VISIBLE,
	//	CRect(pt,CSize(winrect.Width()-gap2.cx-staticSize.cx,staticSize.cy)),
	//	this,
	//	IDS_EDIT_VMS_VOLUME);

	//pt.y+=staticSize.cy+gap2.cy;
	//pt.x-=gap2.cx+staticSize.cx;




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

	//BuildList(winrect.Width());

	SetList();

	return 0;
}
