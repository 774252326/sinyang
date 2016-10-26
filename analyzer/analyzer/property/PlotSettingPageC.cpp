// PlotSettingPageC.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "../Resource.h"
#include "PlotSettingPageC.h"
#include "afxdialogex.h"
//#include "ColorButton.h"
#include "../analyzerViewL.h"

// PlotSettingPageC dialog

IMPLEMENT_DYNAMIC(PlotSettingPageC, CPropertyPage)

	PlotSettingPageC::PlotSettingPageC()
	: CPropertyPage(PlotSettingPageC::IDD)
	, newC(0)
	, oldC(0)
	, bTwo(FALSE)
{
	CString title;
	title.LoadStringW(IDS_STRING_CURVE);
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}


PlotSettingPageC::PlotSettingPageC(const CString & title
	, const std::vector<LineSpec> &pspec
	)
	: CPropertyPage(PlotSettingPageC::IDD)
	, newC(0)
	, oldC(0)
	, bTwo(FALSE)
{
	ps.assign(pspec.begin(),pspec.end());
	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}

PlotSettingPageC::~PlotSettingPageC()
{
	delete [] m_psp.pszTitle;
}

void PlotSettingPageC::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PlotSettingPageC, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// PlotSettingPageC message handlers


BOOL PlotSettingPageC::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(FALSE);

	SetModified();    // Enable Apply Now button.

	SetList();

	return CPropertyPage::OnSetActive();
}


BOOL PlotSettingPageC::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	GetList();

	return CPropertyPage::OnKillActive();
}


int PlotSettingPageC::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CSize gap1(20,20);
	CSize gap2(20,20);

	CRect winrect;
	this->GetClientRect(&winrect);
	winrect.DeflateRect(gap1);
	CPoint pt(gap1);

	CSize buttonSize((winrect.Width()-gap2.cx*3)/4,22);

	CString str;

	if(bTwo!=FALSE){

		str.LoadStringW(IDS_STRING_OLD);
		//pStatic=new CStatic;
		StaticIDS_STRING_OLD.Create(str,
			WS_CHILD
			|WS_VISIBLE, 
			CRect(pt,buttonSize),
			this,
			IDS_STRING_OLD);


		pt.x+=gap2.cx+buttonSize.cx;

		//CMFCColorButton *pmfccb;

		//pmfccb=new CMFCColorButton();

		BtnIDS_STRING_OLD_COLOR.Create(L"",
			WS_VISIBLE
			|WS_CHILD, 
			CRect(pt,buttonSize),
			this,
			IDS_STRING_OLD_COLOR
			);


		pt.x+=gap2.cx+buttonSize.cx;

		str.LoadStringW(IDS_STRING_NEW);
		//pStatic=new CStatic;
		StaticIDS_STRING_NEW.Create(str,
			WS_CHILD
			|WS_VISIBLE, 
			CRect(pt,buttonSize),
			this,
			IDS_STRING_NEW);

		pt.x+=gap2.cx+buttonSize.cx;

		//pmfccb=new CMFCColorButton();

		BtnIDS_STRING_NEW_COLOR.Create(L"",
			WS_VISIBLE
			|WS_CHILD, 
			CRect(pt,buttonSize),
			this,
			IDS_STRING_NEW_COLOR);


		pt.x=gap1.cx;
		pt.y+=gap2.cy+buttonSize.cy;

	}
	winrect.top=pt.y;

	const DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	if(!pslist.Create(dwStyle, winrect, this, IDS_LISTCTRL_SAP) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	return 0;
}





void PlotSettingPageC::SetList(void)
{

	if(bTwo!=FALSE){
		((CMFCColorButton*)GetDlgItem(IDS_STRING_NEW_COLOR))->SetColor(newC);		
		((CMFCColorButton*)GetDlgItem(IDS_STRING_OLD_COLOR))->SetColor(oldC);
	}


	pslist.DeleteAllItems();

	for(size_t i=0;i<ps.size();i++){
		pslist.InsertItemLS(i,ps[i]);
	}

}


void PlotSettingPageC::GetList(void)
{

	if(bTwo!=FALSE){
		newC=((CMFCColorButton*)GetDlgItem(IDS_STRING_NEW_COLOR))->GetColor();		
		oldC=((CMFCColorButton*)GetDlgItem(IDS_STRING_OLD_COLOR))->GetColor();
	}

	int nItem=pslist.GetItemCount();

	for(size_t i=0;i<nItem;i++){
		pslist.GetItemLS(i,ps[i]);
		if(bTwo!=FALSE){
			ps[i].colour=oldC;
		}
	}
	if(bTwo!=FALSE){
		ps.back().colour=newC;
	}
}




void PlotSettingPageC::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
	//CMDIChildWnd* pchild = pframe->MDIGetActive();
	//CPSheetDoc* doc = (CPSheetDoc*) pchild->GetActiveDocument();
	//doc->m_Color = m_Color;
	// Tell the view to paint with the new selected color.
	CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());
	if(bTwo!=FALSE){
		((CanalyzerViewL*)view)->oldCr=oldC;
		((CanalyzerViewL*)view)->newCr=newC;
	}

	if(!ps.empty()){
		view->pw.pdex->pd.ls.assign(ps.begin(),ps.end());
		view->pw.SetLegend();
	}

	view->Invalidate();

	CPropertyPage::OnOK();
}
