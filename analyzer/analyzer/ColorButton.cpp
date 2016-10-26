// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "testmessage.h"
#include "ColorButton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CColorButton, CButton)

CColorButton::CColorButton()
{

}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(BN_CLICKED, &CColorButton::OnBnClicked)
END_MESSAGE_MAP()



// CColorButton message handlers




void CColorButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CButton::OnPaint() for painting messages

	CBrush brush(color);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	CRect rect;
	this->GetClientRect(&rect);
	dc.Rectangle(rect);
	dc.SelectObject(pOldBrush);
}


void CColorButton::OnBnClicked()
{
	// TODO: Add your control notification handler code here

	CColorDialog colorDlg(color);       
  
    if(IDOK == colorDlg.DoModal()){   
        color = colorDlg.GetColor(); 
		this->Invalidate();			
	}
}
