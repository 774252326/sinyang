
// o3View.h : interface of the Co3View class
//

#pragma once
#include "atltypes.h"
#include "triangleMesh.h"

class Co3View : public CView
{
protected: // create from serialization only
	Co3View();
	DECLARE_DYNCREATE(Co3View)

// Attributes
public:
	Co3Doc* GetDocument() const;

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
	virtual ~Co3View();
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
	// initialize opengl
	bool Init(void);
	// Setup the Pixel Format
	bool setupPixelFormat(void);
	// render scene
	void renderScene(void);
	// reshape window
	void reshape(int h, int w);
	// rendering context
	HGLRC m_hRC;
	// device context
	CDC *m_pDC;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();

	

	afx_msg void OnViewRotate();
	void InitGL(void);
	afx_msg void OnUpdateViewRotate(CCmdUI *pCmdUI);
	int mouseMode;
	afx_msg void OnViewTranslate();
	afx_msg void OnUpdateViewTranslate(CCmdUI *pCmdUI);
	afx_msg void OnViewZoom();
	afx_msg void OnUpdateViewZoom(CCmdUI *pCmdUI);

	double xRot;
	double yRot;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CPoint mouseDownPoint;
	double rotateStep;
	double translateStep;
	double zoomStep;
	double xPos;
	double yPos;
	afx_msg void OnViewReset();
	double zoomFactor;
	afx_msg void OnFileOpen();
	CString m_filePath;
	GLuint m_DPList;
	void createSceneList(float ***dt, float *colorv, long r, long c, long h);
	bool isOrtho;
	afx_msg void OnViewOrtho();
	afx_msg void OnUpdateViewOrtho(CCmdUI *pCmdUI);
	void setupLighting(void);


	GLuint m_surfaceList;
	GLuint m_meshList;
	afx_msg void OnViewMesh();
	bool checkMesh;
	afx_msg void OnUpdateViewMesh(CCmdUI *pCmdUI);
	afx_msg void OnViewSurface();
	bool checkSurface;
	afx_msg void OnUpdateViewSurface(CCmdUI *pCmdUI);
	void createMeshList(long ** face, long nf, float ** point, long np);

	void createSurfaceList(long ** face, float *facecolor, long nf, float ** point, long np);
	GLuint m_surfaceList2;
	void createSurfaceList2(long ** face, float *facecolor, long nf, float ** point, long np);
	bool checkSmoothSurface;
	afx_msg void OnViewSmoothsurface();
	afx_msg void OnUpdateViewSmoothsurface(CCmdUI *pCmdUI);
	GLuint m_unityCube;
	void createUnityCubeList(void);
	long ** genFrontFaceList(long ** face, long nf, float ** point, long np, long * nff);
	afx_msg void OnViewContour();
	bool checkContour;
	afx_msg void OnUpdateViewContour(CCmdUI *pCmdUI);

	void createMeshList(const triangleMesh& trm);
	void createSurfaceList(const triangleMesh& trm);
	void createSurfaceList2(const triangleMesh& trm);
	//triangleMesh tm;
	void createContourList(const triangleMesh& trm);
	GLuint m_contourList;

	void createSurfaceList3(const triangleMesh& trm);
	

	GLuint m_surfaceList3;
	int isplane;
	afx_msg void OnViewPlane();
	afx_msg void OnUpdateViewPlane(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in o3View.cpp
inline Co3Doc* Co3View::GetDocument() const
   { return reinterpret_cast<Co3Doc*>(m_pDocument); }
#endif

