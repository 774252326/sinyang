// ImageListBox.cpp : implementation file
//

#include "stdafx.h"
#include "v3.h"
#include "ImageListBox.h"
#include <vector>
#include <algorithm>
// ImageListBox

IMPLEMENT_DYNAMIC(ImageListBox, CListBox)

ImageListBox::ImageListBox()
{

}

ImageListBox::~ImageListBox()
{
}


BEGIN_MESSAGE_MAP(ImageListBox, CListBox)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_POPUP_ADD, &ImageListBox::OnPopupAdd)
	ON_COMMAND(ID_POPUP_REMOVE, &ImageListBox::OnPopupRemove)
	ON_CONTROL_REFLECT(LBN_DBLCLK, &ImageListBox::OnLbnDblclk)
END_MESSAGE_MAP()



// ImageListBox message handlers




void ImageListBox::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_MENU);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	//if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
	{
		//CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		//if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			//return;


		pSumMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

		//pSumMenu->DestroyMenu();


		//((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		//this->GetParent()->ons
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}


void ImageListBox::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	ClientToScreen(&point);
	OnContextMenu(this, point);

	//CListBox::OnRButtonUp(nFlags, point);
}


void ImageListBox::OnPopupAdd()
{
	// TODO: Add your command handler code here


	    TCHAR szFilters[]= _T("JPEG Files (*.jpg)|*.jpg|BMP Files (*.bmp)|*.bmp|PNG Files (*.png)|*.png|All Files (*.*)|*.*||");
  
     // Create an Open dialog; the default file name extension is ".my".
  
     CFileDialog fileDlg(TRUE, _T("jpg"), _T("*.jpg"),
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT , szFilters);
  
     // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
  
     // returns IDOK.
  
     if(fileDlg.DoModal() == IDOK)
     {       
		 POSITION pos=fileDlg.GetStartPosition();

		 while(pos!=NULL){
			 CString fp=fileDlg.GetNextPathName(pos);
			 int nIndex=FindStringExact(0, fp);
			 if(nIndex == LB_ERR)
				this->AddString(fp);
		 }  
     }


}


void ImageListBox::OnPopupRemove()
{
	// TODO: Add your command handler code here
	
	std::vector<int> sis(this->GetSelCount());
	GetSelItems(sis.size(), sis.data()); 
	std::sort(sis.begin(),sis.end());

	while(!sis.empty()){
		this->DeleteString(sis.back());
		sis.pop_back();
	}


}


void ImageListBox::OnLbnDblclk()
{
	// TODO: Add your control notification handler code here

	//AfxMessageBox(L"dsaf");

	int ci=this->GetCurSel();

	if(ci!=LB_ERR){
		CString fp;
		this->GetText(ci,fp);
		ShellExecute(NULL, L"open", fp, NULL, NULL, SW_SHOW);	
	}

}
