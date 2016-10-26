#pragma once
#include "ListCtrlSAPB.h"
#include "ListCtrlDOA.h"
#include "afxwin.h"
// testdlg dialog

class testdlg : public CDialogEx
{
	DECLARE_DYNAMIC(testdlg)

public:
	testdlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~testdlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	ListCtrlSAPB lin;
	ListCtrlDOA lout;
	CButton btn;
	CButton btn2;
	int GetSIFList(std::vector<sapitemF> & sfl);
	int ComputeDOA(const std::vector<sapitemF> & sfl, std::vector<DataOutA> & dol);
	afx_msg void Onbtn1(void);
	void OnCopy(void);
};
