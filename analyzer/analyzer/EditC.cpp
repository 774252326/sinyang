// EditC.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "EditC.h"


// EditC

IMPLEMENT_DYNAMIC(EditC, CEdit)

EditC::EditC()
{

}

EditC::~EditC()
{
}


BEGIN_MESSAGE_MAP(EditC, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &EditC::OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &EditC::OnEnKillfocus)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// EditC message handlers




void EditC::OnEnSetfocus()
{
	// TODO: Add your control notification handler code here

	//this->SetSel(0,-1);
	//DWORD dwSel = this->GetSel();
  //SetSel(HIWORD(dwSel), -1);

	//int si,ei;
	//this->GetSel(si,ei);

	//this->SetHighlight(si,ei);

}


void EditC::OnEnKillfocus()
{
	// TODO: Add your control notification handler code here

	CString  strEditText;  
     this->GetWindowTextW(strEditText);  

       if (strEditText.IsEmpty())  
      {  
        //m_bedtPhoneNumberInitState=TRUE;  
    this->SetWindowTextW(L"username");  
}  


}


BOOL EditC::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	//this->SetWindowTextW(L"username");

	return CEdit::PreCreateWindow(cs);
}


int EditC::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	this->SetWindowTextW(L"username");

	return 0;
}
