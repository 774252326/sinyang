// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// o6View.cpp : implementation of the Co6View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "o6.h"
#endif

#include "o6Doc.h"
#include "o6View.h"

#include "funT1\colormapT.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Co6View

IMPLEMENT_DYNCREATE(Co6View, CView)

	BEGIN_MESSAGE_MAP(Co6View, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Co6View::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_ERASEBKGND()
		ON_WM_SIZE()
	END_MESSAGE_MAP()

	// Co6View construction/destruction

	Co6View::Co6View()
	{
		// TODO: add construction code here

	}

	Co6View::~Co6View()
	{
	}

	BOOL Co6View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CView::PreCreateWindow(cs);
	}

	// Co6View drawing

	void Co6View::OnDraw(CDC* /*pDC*/)
	{
		Co6Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here


		RenderScene();

		// Now Swap the buffers
		SwapBuffers( m_pDC->GetSafeHdc() );
	}


	// Co6View printing


	void Co6View::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL Co6View::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void Co6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void Co6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void Co6View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void Co6View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Co6View diagnostics

#ifdef _DEBUG
	void Co6View::AssertValid() const
	{
		CView::AssertValid();
	}

	void Co6View::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	Co6Doc* Co6View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Co6Doc)));
		return (Co6Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Co6View message handlers

	// render scene
	void Co6View::RenderScene(void)
	{
		// Clear out the color & depth buffers
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode(GL_PROJECTION);


		auto res=std::minmax_element(sfmb.triangleValue.begin(),sfmb.triangleValue.end());

		//glColor3f(1.0f, 0.0f, 1.0f); 
		//for(size_t i=0;i<sfmb.triangle.size();i++){	
		//	glBegin (GL_LINE_LOOP);
		//	for(size_t j=0;j<sfmb.triangle[i].size();j++){
		//		glVertex3d(sfmb.point[sfmb.triangle[i][j]].x[0],sfmb.point[sfmb.triangle[i][j]].x[1],0);
		//	}
		//	glEnd();
		//}


		//glColor3f(1.0f, 1.0f, 0.0f); 
		for(size_t i=0;i<sfmb.contourp.size();i++){
			float rgba[4];
			glBegin (GL_LINE_STRIP);

			genColor(rgba,(sfmb.contourValue[i]-*res.first)/(*res.second-*res.first)*0.9);
			glColor4fv(rgba);

			for(size_t j=0;j<sfmb.contourp[i].size();j++){			
				glVertex3d(sfmb.contourp[i][j].x[0],sfmb.contourp[i][j].x[1],0);
			}
			glEnd();
		}


		//for(size_t i=0;i<sfmb.sfma.triangle.size();i++){	
		//	float rgba[4];
		//	glBegin (GL_LINE_LOOP);
		//	//glBegin(GL_TRIANGLES);
		//	for(size_t j=0;j<sfmb.sfma.triangle[i].size();j++){
		//		genColor(rgba,(sfmb.sfma.pointValue[sfmb.sfma.triangle[i][j]]-*res.first)/(*res.second-*res.first)*0.9);
		//		glColor4fv(rgba);
		//		glVertex3d(sfmb.sfma.point[sfmb.sfma.triangle[i][j]].x[0],sfmb.sfma.point[sfmb.sfma.triangle[i][j]].x[1],0);
		//	}
		//	glEnd();
		//}


		// Tell OpenGL to flush its pipeline
		//glFinish();
		glFlush();
	}

	int Co6View::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		Init();
		InitGL();

		return 0;
	}


	void Co6View::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: Add your message handler code here

		End();
		EndGL();
	}


	BOOL Co6View::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: Add your message handler code here and/or call default
		return TRUE;
		return CView::OnEraseBkgnd(pDC);
	}


	void Co6View::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		if(cx>0 && cy>0){
			Reshape(cx,cy);
		}

	}


	void Co6View::OnInitialUpdate()
	{
		CView::OnInitialUpdate();

		// TODO: Add your specialized code here and/or call the base class

		Co6Doc *pDoc=this->GetDocument();
		CString fp=pDoc->GetPathName();
		if(!fp.IsEmpty()){
			std::vector<std::vector<double>> mx;
			if(pDoc->readf2(fp.GetBuffer(),mx,10)){

				sfmb.Clear();
				sfmb.ClearContour();

				for(size_t i=0;i<mx.size();i++){

					std::vector<size_t> t1(3);
					for(size_t j=0;j<t1.size();j++){
						Point2D pt;
						pt.x[0]=mx[i][j];
						pt.x[1]=mx[i][j+3];
						std::vector<Point2D>::iterator it;
						it=std::find(sfmb.point.begin(),sfmb.point.end(),pt);
						if(it!=sfmb.point.end()){
							t1[j]=it-sfmb.point.begin();
						}
						else{			
							t1[j]=sfmb.point.size();
							sfmb.point.push_back(pt);
						}
					}

					sfmb.triangle.push_back(t1);
					sfmb.triangleValue.push_back(mx[i][9]);

				}
				{
					clock_t t = clock();

					sfmb.InitB();
					sfmb.GenContourMap(30);
					CString str;
					str.Format("Elapsed time %g s.\n", float(clock() - t)/CLOCKS_PER_SEC );
					//str.Format("Elapsed time %d ms.\n", clock() - t );
					MessageBox(str);
				}
				{
					//clock_t t = clock();
					
					//CString str;
					//str.Format("Elapsed time %d ms.\n", clock() - t );
					//MessageBox(str);
				}
				this->Invalidate();

			}
		}
	}
