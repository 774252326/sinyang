
#pragma once

#include "list\OutputListA.hpp"

#include "messagemaphpp.h"

class COutputWnd : public CDockablePane
{

protected:
	COutputListA m_listCtrlMonitor;

	// Construction
public:
	COutputWnd(){};
	virtual ~COutputWnd(){};


	// Implementation
public:
	void UpdateFonts()
	{
		m_listCtrlMonitor.SetFont(&afxGlobalData.fontRegular);
	};

	COutputListA* GetListCtrl(){return &m_listCtrlMonitor;}

protected:

	void AdjustHorzScroll(CListBox& wndListBox)
	{
		CClientDC dc(this);
		CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

		int cxExtentMax = 0;

		for (int i = 0; i < wndListBox.GetCount(); i ++)
		{
			CString strItem;
			wndListBox.GetText(i, strItem);

			cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
		}

		wndListBox.SetHorizontalExtent(cxExtentMax);
		dc.SelectObject(pOldFont);
	};


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		CRect rectDummy;
		rectDummy.SetRectEmpty();

		// Create output panes:
		const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

		if(!m_listCtrlMonitor.Create(dwStyle, rectDummy, this, 5) ){
			TRACE0("Failed to create output windows\n");
			return -1;      // fail to create
		}

		UpdateFonts();

		return 0;
	};


	afx_msg void OnSize(UINT nType, int cx, int cy)
	{
		CDockablePane::OnSize(nType, cx, cy);
		// Tab control should cover the whole client area:
		m_listCtrlMonitor.MoveWindow(0,0,cx,cy);

		//::SetWindowPos(m_listCtrlMonitor.GetSafeHwnd(),HWND_BOTTOM,0,0,cx,cy,SWP_SHOWWINDOW);

	};


	//DECLARE_MESSAGE_MAP()

	BEGIN_MESSAGE_MAP_HPP(COutputWnd, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
		END_MESSAGE_MAP_HPP()

};

