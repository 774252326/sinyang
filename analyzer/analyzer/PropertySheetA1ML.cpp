#include "StdAfx.h"
#include "PropertySheetA1ML.h"


PropertySheetA1ML::PropertySheetA1ML(void)
	:PropertySheetA1(L"")
{
}


PropertySheetA1ML::~PropertySheetA1ML(void)
{
}


BOOL PropertySheetA1ML::OnInitDialog()
{


	// TODO:  Add your specialized code here
m_bModeless = FALSE;   
   m_nFlags |= WF_CONTINUEMODAL;

	BOOL bResult = PropertySheetA1::OnInitDialog();

   m_bModeless = TRUE;
   m_nFlags &= ~WF_CONTINUEMODAL;

	return bResult;
}
