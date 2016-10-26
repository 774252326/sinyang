#pragma once
#include "propertysheeta.h"
   // We need to include this file so we can use CMap
   #include <afxtempl.h>

class PropertySheetA1ML :
	public PropertySheetA1
{
public:
	//PropertySheetA1ML(void);
	PropertySheetA1ML(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	PropertySheetA1ML(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~PropertySheetA1ML(void);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
//	virtual BOOL DestroyWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	   protected:
       // we save the current page in TCN_SELCHANGING
       int m_nLastActive;
       // list of indexes of disabled pages
       CMap <int, int&, int, int&> m_DisabledPages;
	   public:
       void EnablePage (int nPage, BOOL bEnable = TRUE);

};

