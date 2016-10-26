#pragma once


// SolutionAdditionParametersDlg dialog

class SolutionAdditionParametersDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(SolutionAdditionParametersDlg)

public:
	SolutionAdditionParametersDlg();
	virtual ~SolutionAdditionParametersDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	CListCtrl m_SAPlist;
	virtual BOOL OnInitDialog();
};
