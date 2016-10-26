// dlg2.cpp : implementation file
//

#include "stdafx.h"
#include "z18.h"
#include "dlg2.h"
#include <vector>

// dlg2

IMPLEMENT_DYNCREATE(dlg2, CFormView)

	dlg2::dlg2()
	: CFormView(dlg2::IDD)
{

}

dlg2::~dlg2()
{
}

void dlg2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(dlg2, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// dlg2 diagnostics

#ifdef _DEBUG
void dlg2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void dlg2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// dlg2 message handlers


void dlg2::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	int gap=10;
	CRect dlgRect(gap,gap,cx-gap,cy-gap);

	if( GetDlgItem(IDC_LIST1)->GetSafeHwnd()){
		GetDlgItem(IDC_LIST1)->SetWindowPos(NULL, dlgRect.left, dlgRect.top, dlgRect.Width(), dlgRect.Height(), SWP_SHOWWINDOW);
	}
}


void dlg2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	CRect rect;
	// 获取编程语言列表视图控件的位置和大小   
	GetDlgItem(IDC_LIST1)->GetClientRect(&rect); 

	std::vector<CString> strl(4);
	strl[0]=_T("No.");
	strl[1]=_T("Step name");
	strl[2]=_T("Ar value(mc)");
	strl[3]=_T("Use");

	// 为列表视图控件添加三列
	for(size_t i=0;i<strl.size();i++){
		( (CListCtrl*)GetDlgItem(IDC_LIST1) )->InsertColumn(i, strl[i], LVCFMT_CENTER, rect.Width()/strl.size(), i);  
	}

}
