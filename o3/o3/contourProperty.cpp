// contourProperty.cpp : implementation file
//

#include "stdafx.h"
#include "o3.h"
#include "contourProperty.h"
#include "afxdialogex.h"


// contourProperty dialog

IMPLEMENT_DYNAMIC(contourProperty, CDialogEx)

contourProperty::contourProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(contourProperty::IDD, pParent)
{

	m_xlow = 0.0;
	m_xhigh = 0.0;
	m_contourNumber = 0;
}


contourProperty::contourProperty(long n, CWnd* pParent /*=NULL*/)
	: CDialogEx(contourProperty::IDD, pParent)
{

	m_xlow = 0.0;
	m_xhigh = 0.0;
	m_contourNumber = (short)n;
}


contourProperty::contourProperty(long n, double x1, double x2, bool showlabel, CWnd* pParent /*=NULL*/)
	: CDialogEx(contourProperty::IDD, pParent)
{

	m_xlow = x1;
	m_xhigh = x2;
	m_contourNumber = (short)n;
	m_showcv=showlabel;
}


contourProperty::~contourProperty()
{
}

void contourProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XLOW, m_xlow);
	DDX_Text(pDX, IDC_XHIGH, m_xhigh);
	DDX_Check(pDX, IDC_CHECK1, m_isnew);
	DDX_Text(pDX, IDC_CONTUORNUMBER, m_contourNumber);
	DDV_MinMaxShort(pDX, m_contourNumber, 1, 100);
	DDX_Check(pDX, IDC_CHECK2, m_showcv);
}


BEGIN_MESSAGE_MAP(contourProperty, CDialogEx)
END_MESSAGE_MAP()


// contourProperty message handlers
