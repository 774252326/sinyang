#pragma once


// contourProperty dialog

class contourProperty : public CDialogEx
{
	DECLARE_DYNAMIC(contourProperty)

public:
	contourProperty(CWnd* pParent = NULL);   // standard constructor
	contourProperty(long n, CWnd* pParent = NULL);
	contourProperty(long n, double x1, double x2, bool showlabel=false, CWnd* pParent = NULL);
	virtual ~contourProperty();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_xlow;
	double m_xhigh;
	BOOL m_isnew;
	short m_contourNumber;
	BOOL m_showcv;
};
