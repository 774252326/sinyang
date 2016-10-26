#include "StdAfx.h"
#include "PropertySheetA1ML.h"


PropertySheetA1ML::PropertySheetA1ML(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:PropertySheetA1(nIDCaption, pParentWnd, iSelectPage)
{

}

PropertySheetA1ML::PropertySheetA1ML(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:PropertySheetA1(pszCaption, pParentWnd, iSelectPage)
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

	//EnablePage (0, FALSE);
	//EnablePage (1, FALSE);

	return bResult;
}
BEGIN_MESSAGE_MAP(PropertySheetA1ML, PropertySheetA1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


void PropertySheetA1ML::OnClose()
{
	// TODO: Add your message handler code here and/or call default


	//::SendMessage(this->GetParent()->GetSafeHwnd(),MESSAGE_CLOSE_SAP_SHEET,NULL,NULL);

	PropertySheetA1::OnClose();
}


//BOOL PropertySheetA1ML::DestroyWindow()
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return PropertySheetA1::DestroyWindow();
//}


BOOL PropertySheetA1ML::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	NMHDR* pnmh = (NMHDR*)lParam;
       // tab is about to change
       if (TCN_SELCHANGING == pnmh->code)
           // save the current page index
           m_nLastActive = GetActiveIndex ();
       // tab has been changed
       else if (TCN_SELCHANGE == pnmh->code)
       {
           // get the current page index
           int nCurrentPage = GetActiveIndex ();
           // if current page is in our map of disabled pages
           if (m_DisabledPages.Lookup (nCurrentPage, nCurrentPage))
           // activate the previous page
           PostMessage (PSM_SETCURSEL, m_nLastActive);
       }



	return PropertySheetA1::OnNotify(wParam, lParam, pResult);
}


   void PropertySheetA1ML::EnablePage (int nPage, BOOL bEnable)
   {
       // if we want to enable the page
       if (bEnable)
       {
           // remove the index from the map
           m_DisabledPages.RemoveKey (nPage);
           // take out " - Disabled" from tab label
           CTabCtrl* pTab = GetTabControl();
           ASSERT (pTab);
           TC_ITEM ti;
           wchar_t szText[100];
           ti.mask = TCIF_TEXT;
           ti.pszText = szText;
           ti.cchTextMax = 100;
           VERIFY (pTab->GetItem (nPage, &ti));
           wchar_t * pFound = wcsstr (szText, L" - Disabled");
           if (pFound)
           {
               *pFound = '\0';
               VERIFY (pTab->SetItem (nPage, &ti));
           }
       }
       // if we want to disable it
       else
       {
           // add the index to the map
           m_DisabledPages.SetAt (nPage, nPage);
           // add " - Disabled" to tab label
           CTabCtrl* pTab = GetTabControl();
           ASSERT (pTab);
           TC_ITEM ti;
           wchar_t szText[100];
           ti.mask = TCIF_TEXT;
           ti.pszText = szText;
           ti.cchTextMax = 100;
           VERIFY (pTab->GetItem (nPage, &ti));
           wcscat (szText, L" - Disabled");
           VERIFY (pTab->SetItem (nPage, &ti));
       }
   }
