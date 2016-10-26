
// o5View.h : interface of the Co5View class
//

#pragma once


class Co5View : public CView
{
protected: // create from serialization only
	Co5View();
	DECLARE_DYNCREATE(Co5View)

// Attributes
public:
	Co5Doc* GetDocument() const;

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
	virtual ~Co5View();
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);


		//rendering context
	HGLRC m_hRC;
	// device context
	CDC *m_pDC;
	// initialize opengl
	BOOL InitializeOpenGL(void);
	// setup pixel format
	BOOL SetupPixelFormat(void);
	// render scene
	void RenderScene(void);
	// correspond to init() using glut
	void init(void);
	// correspond to reshape() using glut
	void reshape(int cx, int cy);
	void Create3DTextLists(void);
	GLuint m_3DTextList;
	void Create2DTextLists(void);
	GLuint m_2DTextList;
	void SetupLighting(void);

	// width/height ratio
	GLdouble m_aspectRatio;

	GLfloat m_xAngle;
	GLfloat m_yAngle;
	GLfloat m_xPos;
	GLfloat m_yPos;
	GLfloat m_zPos;
	CPoint m_MouseDownPoint;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in o5View.cpp
inline Co5Doc* Co5View::GetDocument() const
   { return reinterpret_cast<Co5Doc*>(m_pDocument); }
#endif

