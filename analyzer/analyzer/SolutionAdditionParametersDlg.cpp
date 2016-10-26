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

	// ��ø����ڣ������Ա�CPropertySheet��   
    CPropertySheet* psheet = (CPropertySheet*) GetParent();   
    //�������Ա�ֻ�С���ɡ���ť   
    psheet->SetFinishText(_T("���"));  

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
	IStyle=GetWindowLong(m_SAPlist.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	IStyle&= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	IStyle|= LVS_REPORT; //set style
	SetWindowLong(m_SAPlist.m_hWnd, GWL_STYLE, IStyle);//����style
	DWORD dwStyle1;
	dwStyle1 = m_SAPlist.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle1 |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle1 |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_SAPlist.SetExtendedStyle(dwStyle1); //������չ���


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
