#pragma once
#include "atltypes.h"


// EditB

class EditB : public CEdit
{
	DECLARE_DYNAMIC(EditB)

public:
	EditB();
	virtual ~EditB();
public:
	CRect rect;
	CPoint coord;
protected:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	DECLARE_MESSAGE_MAP()

};


