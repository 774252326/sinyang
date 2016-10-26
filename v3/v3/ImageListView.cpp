// ImageListView.cpp : implementation file
//

#include "stdafx.h"
#include "v3.h"
#include "ImageListView.h"


// ImageListView

IMPLEMENT_DYNCREATE(ImageListView, CListView)

ImageListView::ImageListView()
{

}

ImageListView::~ImageListView()
{
}

BEGIN_MESSAGE_MAP(ImageListView, CListView)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// ImageListView diagnostics

#ifdef _DEBUG
void ImageListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void ImageListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// ImageListView message handlers


void ImageListView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here


	CMenu menu;
	menu.LoadMenu(IDR_POPUP_MENU);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();

}
