#pragma once
#include "afxwin.h"


#include "../messagemaphpp.h"
// StaticEdit

class StaticEdit : public CEdit
{
	//DECLARE_DYNAMIC(StaticEdit)
public:
	CFont editfont;
public:
	StaticEdit(){};
	virtual ~StaticEdit(){};
protected:
	afx_msg void OnEnSetfocus()
	{
		// TODO: Add your control notification handler code here
		this->HideCaret();
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CEdit::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		this->ModifyStyle(NULL,ES_READONLY);
		this->SetFont(&editfont,TRUE);

		return 0;
	};

//	DECLARE_MESSAGE_MAP()
//};
//
//BEGIN_MESSAGE_MAP(StaticEdit, CEdit)
//	ON_CONTROL_REFLECT(EN_SETFOCUS, &StaticEdit::OnEnSetfocus)
//	ON_WM_CREATE()
//END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP_HPP(StaticEdit, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &StaticEdit::OnEnSetfocus)
	ON_WM_CREATE()
END_MESSAGE_MAP_HPP()
};
