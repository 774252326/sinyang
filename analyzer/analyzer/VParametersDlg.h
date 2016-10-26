#pragma once


// CVParametersDlg dialog

class CVParametersDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CVParametersDlg)

public:
	CVParametersDlg();
	virtual ~CVParametersDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
};
