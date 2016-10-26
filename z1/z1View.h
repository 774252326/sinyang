
// z1View.h : interface of the Cz1View class
//

#pragma once


class Cz1View : public CView
{
protected: // create from serialization only
	Cz1View();
	DECLARE_DYNCREATE(Cz1View)

// Attributes
public:
	Cz1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cz1View();
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
	afx_msg void OnSetUp();
};

#ifndef _DEBUG  // debug version in z1View.cpp
inline Cz1Doc* Cz1View::GetDocument() const
   { return reinterpret_cast<Cz1Doc*>(m_pDocument); }
#endif

