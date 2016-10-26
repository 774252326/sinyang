#pragma once
#include "atltypes.h"


// EditB

class EditB : public CEdit
{
	DECLARE_DYNAMIC(EditB)

public:
	EditB();
	virtual ~EditB();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	CRect rect;
};


