#pragma once


// AnalysisParametersDlg dialog

class AnalysisParametersDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(AnalysisParametersDlg)

public:
	AnalysisParametersDlg();
	virtual ~AnalysisParametersDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg void ComboSelectChange(void);
	//afx_msg void OnBnClickedButton1();
};
