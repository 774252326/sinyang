#pragma once


// OpenImageDlg dialog

class OpenImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OpenImageDlg)

public:
	OpenImageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~OpenImageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_fp1;
	CString m_fp2;
	afx_msg void OnBnClickedButton2();
};
