#pragma once


#define REPORT_ANP 0x01
#define REPORT_CVP 0x02
#define REPORT_SAP 0x04
#define REPORT_RES 0x08
#define REPORT_OUT 0x10
#define REPORT_TES 0x20
#define REPORT_VOL 0x40

// ReportDlg dialog

class ReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ReportDlg)

public:
	ReportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ReportDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BYTE GetSelection(void);

	CButton bn1;
};
