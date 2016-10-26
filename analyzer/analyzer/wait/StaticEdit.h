#pragma once
#include "afxwin.h"


// StaticEdit

class StaticEdit : public CEdit
{
	DECLARE_DYNAMIC(StaticEdit)

public:
	StaticEdit();
	virtual ~StaticEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocus();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CFont editfont;
};


