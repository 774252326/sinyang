// MFCCaptionBarA.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "MFCCaptionBarA.h"
#include "colormapT.h"

// CMFCCaptionBarA

IMPLEMENT_DYNAMIC(CMFCCaptionBarA, CMFCCaptionBar)

	CMFCCaptionBarA::CMFCCaptionBarA()
	: timer(0)
	, x(0)
{

}

CMFCCaptionBarA::~CMFCCaptionBarA()
{
}


BEGIN_MESSAGE_MAP(CMFCCaptionBarA, CMFCCaptionBar)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CMFCCaptionBarA message handlers




int CMFCCaptionBarA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCCaptionBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	//CRect rect1;
	//this->GetWindowRect(&rect1);

	if( ec.Create( WS_CHILD | WS_TABSTOP | WS_BORDER /*|WS_VISIBLE*/, CRect(0, 9, 100, 32), this, IDS_EDIT_CAPTION_EDIT)==0 )
		return -1;
	if(st.Create( _T("Volume(ml):"), WS_CHILD|SS_CENTER/*|WS_VISIBLE*/, CRect(0, 9, 60, 32), this)==FALSE)
		return -1;

	

	//bn.Create(_T("My button"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(310,10,400,30), this, 1);

	return 0;
}


void CMFCCaptionBarA::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CMFCCaptionBar::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	//CRect rect1=this->GetButtonRect();

	//ec.SetRect(CRect(10, rect1.top, 100, rect1.bottom));

}


void CMFCCaptionBarA::OnSize(UINT nType, int cx, int cy)
{
	CMFCCaptionBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	SetEdit();
}


int CMFCCaptionBarA::SetEdit(void)
{
	CRect bRect=this->GetButtonRect();
	CPoint pt=bRect.TopLeft();
	pt.x-=this->GetMargin();

	CRect ecrect;
	ec.GetWindowRect(&ecrect);
	pt.x-=ecrect.Width();
	ecrect.MoveToXY(pt);
	ec.MoveWindow(&ecrect);
	//ec.ShowWindow(SW_SHOW);


	//pt.x-=this->GetMargin();
	CRect strect;
	st.GetWindowRect(&strect);
	pt.x-=strect.Width();
	strect.MoveToXY(pt);
	st.MoveWindow(&strect);

	return 0;
}


int CMFCCaptionBarA::HideEdit(void)
{
	//ec.ShowWindow(SW_HIDE);
	return 0;
}


int CMFCCaptionBarA::ShowButton(bool bShow)
{
	if(bShow){
		CString strTemp;
		//ASSERT
		(strTemp.LoadString(IDS_CAPTION_BUTTON));
		SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_RIGHT, FALSE);
		EnableButton();
	}
	else{

		SetButton(L" ", ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_RIGHT, FALSE);
		EnableButton(FALSE);
		Invalidate();
	}


	return 0;
}


int CMFCCaptionBarA::SetTextA(CString str, bool bFlash)
{
	//this->FlashWindow(bFlash);
	KillTimer(timer);
	m_clrBarText=black;
	SetText(str, CMFCCaptionBar::ALIGN_LEFT);

	if(bFlash){
		timer=SetTimer(1,500,NULL);
	}

	return 0;
}


void CMFCCaptionBarA::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default


	switch(nIDEvent){
	case 1:
		{
			bool aa=(m_clrBarText==black);
			m_clrBarText=aa ? red : black;
			Invalidate();
		}
		break;
	default:
		break;
	}

	CMFCCaptionBar::OnTimer(nIDEvent);
}


void CMFCCaptionBarA::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	DDX_Text(pDX, IDS_EDIT_CAPTION_EDIT, x);

	CMFCCaptionBar::DoDataExchange(pDX);


}
