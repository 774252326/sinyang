#if !defined(AFX_COMBOITEM_H__834127E7_9297_11D5_8AFA_DAC6E5A1C54B__INCLUDED_)
#define AFX_COMBOITEM_H__834127E7_9297_11D5_8AFA_DAC6E5A1C54B__INCLUDED_

#include <afxcoll.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboItem.h : header file
//

#include "../messagemaphpp.h"

/////////////////////////////////////////////////////////////////////////////
// ComboBoxA window
///
/// \brief The ComboBoxA class
///可编辑表格中的下拉选择框

class ComboBoxA : public CComboBox
{


	// Attributes
protected:
	CStringList m_sList;
	CPoint coord;
	BOOL m_bVK_ESCAPE;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ComboBoxA)
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		// TODO: Add your specialized code here and/or call the base class
		if( pMsg->message == WM_KEYDOWN )	
		{		
			if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)	
			{
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);			
				return 1;
			}	
		}	

		return CComboBox::PreTranslateMessage(pMsg);
	};
	//}}AFX_VIRTUAL

	// Implementation
public:
	// Construction
	ComboBoxA( CPoint pt, CStringList *psList = NULL)
	{
		coord=pt;

		if ( psList )
			m_sList.AddTail( psList );

		m_bVK_ESCAPE = FALSE;

	};

	virtual ~ComboBoxA(){};

	// Generated message map functions

protected:
	//{{AFX_MSG(ComboBoxA)
	afx_msg void OnNcDestroy()
	{

		CComboBox::OnNcDestroy();

		// TODO: Add your message handler code here
		delete this;	
	};

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: Add your message handler code here and/or call default
		if(nChar == VK_ESCAPE || nChar == VK_RETURN)	
		{		
			if( nChar == VK_ESCAPE)
				m_bVK_ESCAPE = 1;
			GetParent()->SetFocus();		
			return;	
		}	

		CComboBox::OnChar(nChar, nRepCnt, nFlags);
	};

	////afx_msg void OnKillFocus(CWnd* pNewWnd)
	////{
	//	//int nIndex = GetCurSel();
	//
	//	CComboBox::OnKillFocus(pNewWnd);
	//
	//	//CString str;	
	//	//GetWindowText(str);
	//	//// Send Notification to parent of ListView ctrl	
	//	//LV_DISPINFO lvDispinfo;
	//	//lvDispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	//	//lvDispinfo.hdr.idFrom = GetDlgCtrlID();//that's us
	//	//lvDispinfo.hdr.code = LVN_ENDLABELEDIT;
	//	//lvDispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;	
	//	//lvDispinfo.item.iItem = m_nItem;
	//	//lvDispinfo.item.iSubItem = m_nSubItem;
	//	//lvDispinfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	//	//lvDispinfo.item.cchTextMax = str.GetLength();
	//	//lvDispinfo.item.lParam = GetItemData(GetCurSel());
	//	//if(nIndex!=CB_ERR)
	//	//	GetParent()->GetParent()->SendMessage(
	//	//		WM_NOTIFY,
	//	//		GetParent()->GetDlgCtrlID(),
	//	//		(LPARAM)&lvDispinfo);
	//	//PostMessage(WM_CLOSE);
	//};
	afx_msg void OnCloseup()
	{
		int nIndex = GetCurSel();

		CString str;	
		GetWindowText(str);
		// Send Notification to parent of ListView ctrl	
		LV_DISPINFO lvDispinfo;
		lvDispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		lvDispinfo.hdr.idFrom = GetDlgCtrlID();//that's us
		lvDispinfo.hdr.code = LVN_ENDLABELEDIT;
		lvDispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;	
		lvDispinfo.item.iItem = coord.x;
		lvDispinfo.item.iSubItem = coord.y;
		lvDispinfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
		lvDispinfo.item.cchTextMax = str.GetLength();
		lvDispinfo.item.lParam = GetItemData(GetCurSel());
		if(nIndex!=CB_ERR)
			GetParent()->GetParent()->SendMessage(
			WM_NOTIFY,
			GetParent()->GetDlgCtrlID(),
			(LPARAM)&lvDispinfo);


		GetParent()->SetFocus();	

		PostMessage(WM_CLOSE);
	};

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CComboBox::OnCreate(lpCreateStruct) == -1)
			return -1;

		CFont* font = GetParent()->GetFont();	
		SetFont(font);
		//add the items from CStringlist
		POSITION pos = m_sList.GetHeadPosition();
		while(pos != NULL)
			AddString((LPCTSTR)(m_sList.GetNext(pos)));	
		SetFocus();	

		return 0;
	};


	//}}AFX_MSG

//	DECLARE_MESSAGE_MAP()
//
//};
//
///////////////////////////////////////////////////////////////////////////////
//
//BEGIN_MESSAGE_MAP(ComboBoxA, CComboBox)
//	//{{AFX_MSG_MAP(ComboBoxA)
//	ON_WM_NCDESTROY()
//	ON_WM_CHAR()
//	//ON_WM_KILLFOCUS()
//	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
//	ON_WM_CREATE()
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP_HPP(ComboBoxA, CComboBox)
	//{{AFX_MSG_MAP(ComboBoxA)
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	//ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP_HPP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOITEM_H__834127E7_9297_11D5_8AFA_DAC6E5A1C54B__INCLUDED_)
