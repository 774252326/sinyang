// SolutionAdditionParametersPageB.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "SolutionAdditionParametersPageB.h"
#include "afxdialogex.h"
#include  "filefunc.h"

// SolutionAdditionParametersPageB dialog

IMPLEMENT_DYNAMIC(SolutionAdditionParametersPageB, CPropertyPage)

SolutionAdditionParametersPageB::SolutionAdditionParametersPageB()
	: CPropertyPage(SolutionAdditionParametersPageB::IDD)
{
	CString title;
	title.LoadStringW(IDS_STRING_ADDITION_SOLUTION_PARA);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
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

	//UpdateData(FALSE);

	//if(para1.saplist.empty()){
	//	return;
	//}

	m_SAPlist1.DeleteAllItems();

	for(size_t i=0;i<para1.saplist.size();i++){
		m_SAPlist1.InsertItemSAP(i,para1.saplist[i]);
	}


}


void SolutionAdditionParametersPageB::GetList(void)
{
	//UpdateData(TRUE);

	int nItem=m_SAPlist1.GetItemCount();

	para1.saplist.resize(nItem);


	for(size_t i=0;i<nItem;i++){
		m_SAPlist1.GetItemSAP(i,para1.saplist[i]);
	}

}

int SolutionAdditionParametersPageB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here




	CSize gap1(20,20);
	CSize gap2(20,20);
	//CSize staticSize(150,22);

	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);
	//CStatic *pStatic;
	//CEdit *pEdit;
	//CString str;

	CSize listsz=winrect.Size();
	listsz.cy-=gap2.cy;
	listsz.cy/=2;


	//str.LoadStringW(IDS_STRING_SAP_TIPS);
	//stt.Create(
	//	str,
	//	WS_CHILD
	//	|WS_VISIBLE, 
	//	CRect(pt,CSize(winrect.Width(),staticSize.cy)),
	//	this,
	//	10000);

	//pt.y+=gap2.cy+staticSize.cy;


	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	if(!m_SAPlist0.Create(dwStyle, CRect(pt,listsz), this, IDS_LISTCTRL_SAP_0) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	pt.y+=gap2.cy+listsz.cy;

	if(!m_SAPlist1.Create(dwStyle, CRect(pt,listsz), this, IDS_LISTCTRL_SAP_1) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}


	

	for(size_t i=0;i<para0.saplist.size();i++){
		m_SAPlist0.InsertItemSAP(i,para0.saplist[i]);
	}


	//size_t nc=m_SAPlist0.typelist.size();

	//m_SAPlist0.typelist.assign(nc,m_SAPlist0.eStatic);


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


	//pDoc->ChangeSAP(para1);

	pDoc->bChangeSAP=true;
	pDoc->p3todo=para1;

	//mypara1 * pa1=new mypara1;

	//pa1->adoc=pDoc;
	//pa1->pp3todo=&para1;

	//POSITION pos = pDoc->GetFirstViewPosition();
	//CMainFrame *mf=(CMainFrame*)(pDoc->GetNextView(pos)->GetParentFrame());

	//pa1->psta=&(mf->pst);

	////pa1->leftp=(CanalyzerViewL*)m_wndSplitter.GetPane(0,0);
	////pa1->rightp=(CanalyzerViewR*)m_wndSplitter.GetPane(0,1);
	////pa1->outw=this->GetOutputWnd();
	////pa1->cba=this->GetCaptionBar();
	////pa1->ol=this->GetOutputWnd()->GetListCtrl();
	////pa1->psta=&pst;
	////pa1->wd=wd;
	////pa1->mf=this;
	////

	////
	//	CWinThread *pWriteA;
	////
	////	//HANDLE hThread;
	////
	//pWriteA=AfxBeginThread(PROCESS1,
	//	(LPVOID)(pa1),
	//	THREAD_PRIORITY_NORMAL,
	//	0,
	//	CREATE_SUSPENDED);
	////
	////	//hThread=pWriteA->m_hThread;
	////
	////	//CloseHandle(hThread);
	//pWriteA->ResumeThread();


	return CPropertyPage::OnApply();
}
