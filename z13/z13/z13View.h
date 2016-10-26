
// z13View.h : interface of the Cz13View class
//

#pragma once

#include "resource.h"
#include "afxwin.h"
#include "tipdlg.h"

class Cz13View : public CFormView
{
protected: // create from serialization only
	Cz13View();
	DECLARE_DYNCREATE(Cz13View)

public:
	enum{ IDD = IDD_Z13_FORM };

// Attributes
public:
	Cz13Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~Cz13View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;
	afx_msg void OnBnClickedButton1();
	int i;
	tipdlg *ptipdlg;
	afx_msg void OnBnClickedButton2();
};

#ifndef _DEBUG  // debug version in z13View.cpp
inline Cz13Doc* Cz13View::GetDocument() const
   { return reinterpret_cast<Cz13Doc*>(m_pDocument); }
#endif

