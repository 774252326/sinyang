#pragma once
#include "atltypes.h"

#include "../messagemaphpp.h"

// EditB

class EditB : public CEdit
{

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

	BEGIN_MESSAGE_MAP_HPP(EditB, CEdit)
		ON_WM_WINDOWPOSCHANGING()
		END_MESSAGE_MAP_HPP()

};



