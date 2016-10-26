// PlotSettingPageB.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "PlotSettingPageB.h"
#include "afxdialogex.h"
#include "func.h"

// PlotSettingPage dialog

IMPLEMENT_DYNAMIC(PlotSettingPage, CPropertyPage)

PlotSettingPage::PlotSettingPage()
	: CPropertyPage(PlotSettingPage::IDD)
{

}


PlotSettingPage::PlotSettingPage(const CString & title
	, const PlotSpec &fspec
	, const std::vector<LineSpec> &pspec)
	: CPropertyPage(PlotSettingPage::IDD)
{

	fs=fspec;
	ps.assign(pspec.begin(),pspec.end());

	m_psp.dwFlags = m_psp.dwFlags | PSP_USETITLE ; 	
	m_psp.pszTitle = new TCHAR[title.GetLength()+1];
	_tcscpy((wchar_t*)m_psp.pszTitle, title);
}




PlotSettingPage::~PlotSettingPage()
{
}

void PlotSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PlotSettingPage, CPropertyPage)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// PlotSettingPage message handlers


int PlotSettingPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	CSize buttonSize(winrect.Width(),80);


	DWORD dwStyle = WS_VISIBLE 
		| WS_CHILD 
		//| WS_BORDER
		| WS_HSCROLL 
		| WS_VSCROLL 
		| LBS_NOINTEGRALHEIGHT;

	if(!pslist.Create(dwStyle, CRect(pt,buttonSize), this, IDS_LISTCTRL_PSP) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	pt.y+=buttonSize.cy+gap2.cy;

	winrect.top=pt.y;

	dwStyle|=WS_BORDER;

	//if(!lslist.Create(dwStyle, winrect, this, IDS_LISTCTRL_LSP) ){
	//	TRACE0("Failed to create output windows\n");
	//	return -1;      // fail to create
	//}

	pslist.SetD(fs);
	//SetList();

	return 0;
}


void PlotSettingPage::SetList(void)
{

	//fs=pslist.GetD();


	//CString strTemp;

	//strTemp.LoadStringW(IDS_STRING_COLOR_TYPE_0-1);
	//pslist.InsertItem( 0, strTemp );

	//strTemp.LoadStringW(IDS_STRING_GRID_LINE_SOLID+fs.gridType);
	//pslist.SetItemText(0,1,strTemp);

	//strTemp.Format(L"%d",fs.labelSize);
	//pslist.SetItemText(0, 2, strTemp);

	//strTemp.Format(L"%d",fs.metricSize);
	//pslist.SetItemText(0, 3, strTemp);

	//strTemp.Format(L"%d",fs.bkgndC);
	//pslist.SetItemText(0, 4, strTemp);

	//strTemp.Format(L"%d",fs.metricC);
	//pslist.SetItemText(0, 5, strTemp);

	//strTemp.Format(L"%d",fs.labelC);
	//pslist.SetItemText(0, 6, strTemp);

	//for(int j=0;j<7;j++){
	//	AdjustWidth(&pslist,j,0);
	//}





	//if(ps.empty()){
	//	return;
	//}



	//for(size_t i=0;i<ps.size();i++){
	//	strTemp.Format(L"%d",i+1);
	//	lslist.InsertItem( i, strTemp );
	//
	//	lslist.SetItemText(i,1,ps[i].name);

	//	strTemp.Format(L"%d",ps[i].colour);
	//	lslist.SetItemText(i, 2, strTemp);

	//	strTemp.Format(L"%d",ps[i].dotSize);
	//	lslist.SetItemText(i,3,strTemp);


	//	strTemp.LoadStringW(IDS_STRING_GRID_LINE_SOLID+ps[i].lineType);
	//	lslist.SetItemText(i,4,strTemp);
	//	

	//	strTemp.LoadStringW(IDS_STRING_NO_SMOOTH+ps[i].smoothLine);
	//	lslist.SetItemText(i,5,strTemp);
	//	

	//	strTemp.LoadStringW( (ps[i].traceLast?IDS_STRING_YES:IDS_STRING_NO) );
	//	lslist.SetItemText(i,6,strTemp);
	//	
	//	for(int j=0;j<7;j++){
	//		AdjustWidth(&lslist,j,i);
	//	}
	//}

}


void PlotSettingPage::GetList(void)
{


	//if(ps.empty()){
	//	return;
	//}
	//
	//int nItem=lslist.GetItemCount();

	//CString strTemp;

	//for(size_t i=0;i<nItem;i++){
	//	ps[i].name=lslist.GetItemText(i,1);

	//	strTemp=lslist.GetItemText(i,2);
	//	ps[i].colour=_wtoi(strTemp.GetBuffer());

	//	strTemp=lslist.GetItemText(i,3);
	//	ps[i].dotSize=_wtoi(strTemp.GetBuffer());
	//	ps[i].lineType=lslist.GetChoice(i,4);
	//	ps[i].smoothLine=lslist.GetChoice(i,5);
	//	ps[i].traceLast=(lslist.GetChoice(i,6)==0);

	//}

}


BOOL PlotSettingPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::OnSetActive();
}


BOOL PlotSettingPage::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	//GetList();
	fs=pslist.GetD();

	return CPropertyPage::OnKillActive();
}
