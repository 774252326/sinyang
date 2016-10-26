// OpenImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "v2.h"
#include "OpenImageDlg.h"
#include "afxdialogex.h"


// OpenImageDlg dialog

IMPLEMENT_DYNAMIC(OpenImageDlg, CDialogEx)

OpenImageDlg::OpenImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(OpenImageDlg::IDD, pParent)
{

	m_fp1 = _T("");
	m_fp2 = _T("");
}

OpenImageDlg::~OpenImageDlg()
{
}

void OpenImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fp1);
	DDX_Text(pDX, IDC_EDIT2, m_fp2);
}


BEGIN_MESSAGE_MAP(OpenImageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &OpenImageDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &OpenImageDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// OpenImageDlg message handlers


void OpenImageDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

		     // szFilters is a text string that includes two file name filters:
  
     // "*.my" for "MyType Files" and "*.*' for "All Files."
  
     TCHAR szFilters[]= _T("All Files (*.*)|*.*||");
  
     // Create an Open dialog; the default file name extension is ".my".
  
     CFileDialog fileDlg(TRUE, _T("*"), _T("*.*"),
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
  
     // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
  
     // returns IDOK.
  
     if(fileDlg.DoModal() == IDOK)
     {
        m_fp1 = fileDlg.GetPathName();
  
        // Implement opening and reading file in here.
  
		//this->upd


        //Change the window's title to the opened file's title.
  
        //CString fileName = fileDlg.GetFileTitle();
  
        //SetWindowText(fileName);

		this->UpdateData(FALSE);
     }
}


void OpenImageDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here


	    TCHAR szFilters[]= _T("All Files (*.*)|*.*||");
  
     // Create an Open dialog; the default file name extension is ".my".
  
     CFileDialog fileDlg(TRUE, _T("*"), _T("*.*"),
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
  
     // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
  
     // returns IDOK.
  
     if(fileDlg.DoModal() == IDOK)
     {
        m_fp2 = fileDlg.GetPathName();
  
        // Implement opening and reading file in here.
  
		//this->upd


        //Change the window's title to the opened file's title.
  
        //CString fileName = fileDlg.GetFileTitle();
  
        //SetWindowText(fileName);

		this->UpdateData(FALSE);
     }
}
