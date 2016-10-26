#pragma once
#include "atltypes.h"


// EditB

class EditB : public CEdit
{
	DECLARE_DYNAMIC(EditB)
public:
	CRect rect;
	CPoint coord;

public:
	EditB(){};
	virtual ~EditB(){};

protected:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos)
	{
	CEdit::OnWindowPosChanging(lpwndpos);

	// TODO: Add your message handler code here

	lpwndpos->cx=rect.Width();
	lpwndpos->cy=rect.Height();
	lpwndpos->x=rect.left;
	lpwndpos->y=rect.top;
	};



	DECLARE_MESSAGE_MAP()



};


	IMPLEMENT_DYNAMIC(EditB, CEdit)

	BEGIN_MESSAGE_MAP(EditB, CEdit)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()