#pragma once
#include "propertysheeta.hpp"
// We need to include this file so we can use CMap
#include <afxtempl.h>

class PropertySheetA1ML :
	public PropertySheetA1
{
protected:
	// we save the current page in TCN_SELCHANGING
	int m_nLastActive;
	// list of indexes of disabled pages
	CMap <int, int&, int, int&> m_DisabledPages;
public:
	void EnablePage (int nPage, BOOL bEnable = TRUE)
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
	};
public:
	//PropertySheetA1ML(void);
	PropertySheetA1ML(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0)
		:PropertySheetA1(nIDCaption, pParentWnd, iSelectPage)
	{};
	PropertySheetA1ML(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0)
		:PropertySheetA1(pszCaption, pParentWnd, iSelectPage)
	{};
	virtual ~PropertySheetA1ML(void){};
protected:
	virtual BOOL OnInitDialog()
	{
		// TODO:  Add your specialized code here
		m_bModeless = FALSE;   
		m_nFlags |= WF_CONTINUEMODAL;

		BOOL bResult = PropertySheetA1::OnInitDialog();

		m_bModeless = TRUE;
		m_nFlags &= ~WF_CONTINUEMODAL;

		return bResult;
	};
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
	};


	//DECLARE_MESSAGE_MAP()
	//afx_msg void OnClose();
	//	virtual BOOL DestroyWindow();



};

