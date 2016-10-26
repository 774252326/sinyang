// MFCCaptionBarA.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "MFCCaptionBarA.h"
#include "colormapT.h"
//#include "typedefine.h"

// CMFCCaptionBarA

IMPLEMENT_DYNAMIC(CMFCCaptionBarA, CMFCCaptionBar)

	CMFCCaptionBarA::CMFCCaptionBarA()
	: timer(0)
	, x(0)
	, ecWidth(50)
	, strblank(_T("                                        "))
	, fontheight(-14)
	, flashms(500)
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
	ON_MESSAGE(MESSAGE_BUSY, &CMFCCaptionBarA::OnMessageBusy)
	ON_MESSAGE(MESSAGE_WAIT_RESPONSE, &CMFCCaptionBarA::OnMessageWaitResponse)
	ON_MESSAGE(MESSAGE_OVER, &CMFCCaptionBarA::OnMessageOver)

	//ON_BN_CLICKED(IDS_BUTTON_CAPTION_CHECKBOX, &CMFCCaptionBarA::BtnClicked)
	ON_MESSAGE(MESSAGE_READY, &CMFCCaptionBarA::OnMessageReady)
	ON_MESSAGE(MESSAGE_OVER_H, &CMFCCaptionBarA::OnMessageOverH)
END_MESSAGE_MAP()



// CMFCCaptionBarA message handlers




int CMFCCaptionBarA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCCaptionBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	//CRect rect1=GetButtonRect();;
	//this->GetWindowRect(&rect1);


	if( ec.Create( WS_CHILD | WS_TABSTOP | WS_BORDER /*|WS_VISIBLE*/, CRect(), this, IDS_EDIT_CAPTION_EDIT)==0 )
		return -1;
	//if(st.Create( _T("Volume(ml):"), WS_CHILD|SS_CENTER/*|WS_VISIBLE*/, CRect(0, 9, 60, 32), this)==FALSE)
	//return -1;



	//if( bn.Create(_T(""), WS_CHILD|/*WS_VISIBLE|*/BS_AUTOCHECKBOX, CRect(0,0,12,12), this, IDS_BUTTON_CAPTION_CHECKBOX) ==FALSE)
		//return -1;

	return 0;
}


//void CMFCCaptionBarA::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	CMFCCaptionBar::OnShowWindow(bShow, nStatus);
//
//	// TODO: Add your message handler code here
//	//CRect rect1=this->GetButtonRect();
//
//	//ec.SetRect(CRect(10, rect1.top, 100, rect1.bottom));
//
//}


void CMFCCaptionBarA::OnSize(UINT nType, int cx, int cy)
{
	CMFCCaptionBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	SetEdit();
}


int CMFCCaptionBarA::SetEdit(void)
{

	//UpdateData(FALSE);

	CRect bRect=this->GetButtonRect();
	CPoint pt=bRect.TopLeft();
	pt.x-=this->GetMargin();

	//int ecw=50;
	pt.x-=ecWidth;
	CRect ecrect(0,0,ecWidth,bRect.Height());
	//ec.GetWindowRect(&ecrect);
	//ecrect.bottom=ecrect.top+bRect.Height();
	ecrect.MoveToXY(pt);
	ec.MoveWindow(&ecrect);
	//ec.ShowWindow(SW_SHOW);


	//pt.x-=this->GetMargin();
	//CRect strect;
	//st.GetWindowRect(&strect);
	//pt.x-=strect.Width();
	//strect.MoveToXY(pt);
	//st.MoveWindow(&strect);


	//CRect bnrect;
	//bn.GetWindowRect(&bnrect);
	//bnrect.MoveToXY(bRect.left,bRect.CenterPoint().y-bnrect.Height()/2);
	//bn.MoveWindow(&bnrect);


	return 0;
}


//int CMFCCaptionBarA::HideEdit(void)
//{
//	//ec.ShowWindow(SW_HIDE);
//	return 0;
//}


//int CMFCCaptionBarA::ShowButton(bool bShow)
//{
//	//if(bShow){
//	//	CString strTemp;
//	//	//ASSERT
//	//	(strTemp.LoadString(IDS_CAPTION_BUTTON));
//	//	//SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_RIGHT, FALSE);
//	//	SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
//	//	EnableButton();
//	//}
//	//else{
//
//	//	//SetButton(L" ", ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_RIGHT, FALSE);
//	//	SetButton(L" ", ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
//	//	EnableButton(FALSE);
//	//	Invalidate();
//	//}
//
//
//	return 0;
//}


int CMFCCaptionBarA::SetTextA(CString str, bool bFlash)
{
	//UpdateData(FALSE);
	//this->FlashWindow(bFlash);
	KillTimer(timer);
	m_clrBarText=black;
	//SetText(str, CMFCCaptionBar::ALIGN_RIGHT);
	SetText(str, CMFCCaptionBar::ALIGN_LEFT);
	if(bFlash){
		timer=SetTimer(1,flashms,NULL);

		//FlashWindowEx(FLASHW_ALL,1000,500);
	}
	Invalidate();
	return 0;
}


void CMFCCaptionBarA::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default


	switch(nIDEvent){
	case 1:
		{
			m_clrBarText=(m_clrBarText==black)? red : black;
			//UpdateData(FALSE);
			Invalidate();
			//this->FlashWindow(TRUE);
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


void CMFCCaptionBarA::ShowMessageRunning(void)
{
	CString strTemp;
	(strTemp.LoadString(IDS_STRING_RUNNING));
	ShowMessage(strTemp);
}


void CMFCCaptionBarA::ShowMessage(CString str)
{

	EnableButton(FALSE);
	SetEdit();
	ec.ShowWindow(SW_HIDE);
	//bn.ShowWindow(SW_HIDE);
	SetTextA(str);
	this->ShowWindow(SW_SHOW);
	this->GetParentFrame()->RecalcLayout(FALSE);
}


void CMFCCaptionBarA::ShowMessageWithButton(CString str, double xv, bool bFlash)
{
	str+=strblank;
	SetTextA(str,bFlash);

	x=xv;
	UpdateData(FALSE);
	
	EnableButton();
	SetEdit();
	ec.ShowWindow(SW_SHOW);
	this->ShowWindow(SW_SHOW);
	this->GetParentFrame()->RecalcLayout(FALSE);
}



void CMFCCaptionBarA::OnDrawButton(
	CDC* pDC,
	CRect rect,
	const CString& strButton,
	BOOL bEnabled 
	)
{
	if(bEnabled){
		//rect.left+=ecWidth+this->GetMargin();
		//rect.right+=ecWidth+this->GetMargin();
		CMFCCaptionBar::OnDrawButton(pDC, rect, strButton, bEnabled);
	}
	else{
		//pDC->SetBkMode(TRANSPARENT);
		CMFCCaptionBar::OnDrawButton(pDC, rect, L" ", bEnabled);
	}
}


void CMFCCaptionBarA::OnDrawText(
	CDC* pDC,
	CRect rect,
	const CString& strText 
	)
{
	CFont font;

	CFont *pfnt=pDC->GetCurrentFont();

	LOGFONT lgft;
	pfnt->GetLogFont(&lgft);

	lgft.lfHeight=fontheight;

	font.CreateFontIndirectW(&lgft);
	pDC->SelectObject(&font);

	CSize sz=pDC->GetTextExtent(strText);

	rect.right=rect.left+sz.cx;
	rect.top=rect.CenterPoint().y-sz.cy/2;
	rect.bottom=rect.top+sz.cy;

	if(ec.IsWindowVisible()){
		rect.right-=ecWidth;
		rect.right-=this->GetMargin();
		CString strcopy(strText);
		strcopy.Replace(strblank,NULL);
		CMFCCaptionBar::OnDrawText(pDC, rect, strcopy);
	}
	else{
		CMFCCaptionBar::OnDrawText(pDC, rect, strText);
	}

	font.DeleteObject();
}

afx_msg LRESULT CMFCCaptionBarA::OnMessageBusy(WPARAM wParam, LPARAM lParam)
{
	ShowMessageRunning();
	return 0;
}


afx_msg LRESULT CMFCCaptionBarA::OnMessageWaitResponse(WPARAM wParam, LPARAM lParam)
{

	CString strTemp;

	(strTemp.LoadString(IDS_STRING_WAIT_RESPONSE));

	if(wParam!=NULL){
		double *px=(double*)wParam;
		ShowMessageWithButton(strTemp,*px,true);
	}
	else{
		ShowMessageWithButton(strTemp,x,true);
	}

	return 0;
}


afx_msg LRESULT CMFCCaptionBarA::OnMessageOver(WPARAM wParam, LPARAM lParam)
{

	CString strTemp;
	(strTemp.LoadString(IDS_STRING_OVER));

	CString str((wchar_t*)wParam);

	strTemp+=str;

	ShowMessage(strTemp);

	return 0;
}


//void CMFCCaptionBarA::BtnClicked(void)
//{
//	//bool bCheck=bn.GetCheck();
//
//	CString str;
//
//	//if(bCheck){
//		//str=rightPlotFile1;
//	//}
//	//else{
//		//str=rightPlotFile0;
//	//}
//
//	::SendMessageW(this->GetParentFrame()->GetSafeHwnd(), MESSAGE_SWITCH_FIGURE, (WPARAM)str.GetBuffer(), NULL);
//
//}


afx_msg LRESULT CMFCCaptionBarA::OnMessageReady(WPARAM wParam, LPARAM lParam)
{
	CString strTemp;
	(strTemp.LoadString(IDS_STRING_READY));
	ShowMessage(strTemp);
	return 0;
}


afx_msg LRESULT CMFCCaptionBarA::OnMessageOverH(WPARAM wParam, LPARAM lParam)
{

	CString strTemp;
	(strTemp.LoadString(IDS_STRING_OVER));

	CString str((wchar_t*)wParam);
	strTemp+=str;

	str.LoadStringW(IDS_STRING_OVER_SWITCH);

	strTemp+=str;

	ShowMessage(strTemp);
	SetEdit();
	//bn.ShowWindow(SW_SHOW);
	return 0;
}
