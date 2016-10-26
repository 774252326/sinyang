// EditB.cpp : implementation file
//

#include "stdafx.h"
//#include "edls.h"
#include "EditB.h"


// EditB

IMPLEMENT_DYNAMIC(EditB, CEdit)

EditB::EditB()
: x(0)
{

}

EditB::~EditB()
{
}


BEGIN_MESSAGE_MAP(EditB, CEdit)
	ON_WM_WINDOWPOSCHANGING()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &EditB::OnEnKillfocus)
END_MESSAGE_MAP()



// EditB message handlers




void EditB::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CEdit::OnWindowPosChanging(lpwndpos);

	// TODO: Add your message handler code here

	lpwndpos->cx=rect.Width();
	lpwndpos->cy=rect.Height();
	lpwndpos->x=rect.left;
	lpwndpos->y=rect.top;
}


void EditB::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class
	
	//DDX_Text(pDX, this->GetDlgCtrlID(), x);

	CEdit::DoDataExchange(pDX);
}


void EditB::OnEnKillfocus()
{
	// TODO: Add your control notification handler code here
	//UpdateData();
}


void EditB::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CEdit::PreSubclassWindow();
}
