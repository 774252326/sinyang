// StaticEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "StaticEdit.h"


// StaticEdit

IMPLEMENT_DYNAMIC(StaticEdit, CEdit)

StaticEdit::StaticEdit()
{

}

StaticEdit::~StaticEdit()
{
}


BEGIN_MESSAGE_MAP(StaticEdit, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &StaticEdit::OnEnSetfocus)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// StaticEdit message handlers




void StaticEdit::OnEnSetfocus()
{
	// TODO: Add your control notification handler code here
	this->HideCaret();
}


int StaticEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	this->ModifyStyle(NULL,ES_READONLY);
	this->SetFont(&editfont,TRUE);

	return 0;
}
