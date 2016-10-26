
// v2View.h : interface of the Cv2View class
//

#pragma once

#include "v2Doc.h"
#include "atltypes.h"

class Cv2View : public CView
{
protected: // create from serialization only
	Cv2View();
	DECLARE_DYNCREATE(Cv2View)

// Attributes
public:
	Cv2Doc* GetDocument() const;

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
	virtual ~Cv2View();
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
	CImage img;
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CPoint m_mouseDownPoint;
	CRect imgrect;
	CSize imgsz;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnInitialUpdate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CPoint ptc;
};

#ifndef _DEBUG  // debug version in v2View.cpp
inline Cv2Doc* Cv2View::GetDocument() const
   { return reinterpret_cast<Cv2Doc*>(m_pDocument); }
#endif

