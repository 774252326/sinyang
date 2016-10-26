// EditB.cpp : implementation file
//

#include "stdafx.h"
#include "EditB.h"


// EditB

IMPLEMENT_DYNAMIC(EditB, CEdit)

EditB::EditB()
{

}

EditB::~EditB()
{
}


BEGIN_MESSAGE_MAP(EditB, CEdit)
	ON_WM_WINDOWPOSCHANGING()
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
