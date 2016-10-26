#pragma once


// ParaDlg dialog

class ParaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ParaDlg)

public:
	ParaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ParaDlg();
	CWnd* pParent0;
// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double raw;
	double correction;
	
	double m_correction;
	double m_raw;
	double m_final;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedOk();
};
