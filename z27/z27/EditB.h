#pragma once
#include "atltypes.h"
#include "VCEdit.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);
	double x;
	afx_msg void OnEnKillfocus();
	virtual void PreSubclassWindow();
};


