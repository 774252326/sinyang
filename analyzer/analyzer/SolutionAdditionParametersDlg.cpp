// SolutionAdditionParametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "SolutionAdditionParametersDlg.h"
#include "afxdialogex.h"


// SolutionAdditionParametersDlg dialog

IMPLEMENT_DYNAMIC(SolutionAdditionParametersDlg, CPropertyPage)

SolutionAdditionParametersDlg::SolutionAdditionParametersDlg()
	: CPropertyPage(SolutionAdditionParametersDlg::IDD)
{

}

SolutionAdditionParametersDlg::~SolutionAdditionParametersDlg()
{
}

void SolutionAdditionParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SolutionAdditionParametersDlg, CPropertyPage)
END_MESSAGE_MAP()


// SolutionAdditionParametersDlg message handlers


BOOL SolutionAdditionParametersDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class

	// 获得父窗口，即属性表CPropertySheet类   
    CPropertySheet* psheet = (CPropertySheet*) GetParent();   
    //设置属性表只有“完成”按钮   
    psheet->SetFinishText(_T("完成"));  

	return CPropertyPage::OnSetActive();
}


BOOL SolutionAdditionParametersDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here


	CSize gap1(20,20);
	CRect winrect;
	this->GetWindowRect(&winrect);
	winrect.DeflateRect(gap1);


	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT 
		| WS_CHILD 
		| WS_VISIBLE 
		| WS_HSCROLL 
		| WS_VSCROLL 
		| WS_BORDER ;

	if(!m_SAPlist.Create(dwStyle, CRect(CPoint(gap1),winrect.Size()), this, IDS_LISTCTRL_SAP) ){
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	m_SAPlist.ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);
	LONG IStyle;
	IStyle=GetWindowLong(m_SAPlist.m_hWnd, GWL_STYLE);//获取当前窗口style
	IStyle&= ~LVS_TYPEMASK; //清除显示方式位
	IStyle|= LVS_REPORT; //set style
	SetWindowLong(m_SAPlist.m_hWnd, GWL_STYLE, IStyle);//设置style
	DWORD dwStyle1;
	dwStyle1 = m_SAPlist.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle1 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_SAPlist.SetExtendedStyle(dwStyle1); //设置扩展风格


	CString strTemp;
	for(size_t i=0;i<4;i++){
		strTemp.LoadStringW(IDS_STRING510+i);
		m_SAPlist.InsertColumn(IDS_STRING510+i, strTemp, LVCFMT_LEFT,winrect.Width()/4);
	}

	if ( !m_SAPlist.GetSafeHwnd() )
	{
		return 0;
	}

	//m_SAPlist.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
