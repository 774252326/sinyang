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

// o6View.h : interface of the Co6View class
//

#pragma once

//OpenGL Headers
#include <gl/gl.h>
#include <gl/glu.h>
#include "m\SurfaceMeshB.hpp"

class Co6View : public CView
{
protected: // create from serialization only
	Co6View();
	DECLARE_DYNCREATE(Co6View)

// Attributes
public:
	Co6Doc* GetDocument() const;

	// rendering context
	HGLRC m_hRC;
	// device context
	CDC *m_pDC;

	bool isOrtho;
	GLuint charlists;
	GLuint m_3DTextList;
	GLuint m_2DTextList;

	SurfaceMeshB sfmb;

// Operations
public:
	// render scene
	void RenderScene(void);
	
	// generate ASCII character display lists
	void GenCharList(void)
	{
		// 为每一个ASCII字符产生一个显示列表
		//isFirstCall = 0;
		const int MAX_CHAR=128;

		// 申请MAX_CHAR个连续的显示列表编号
		charlists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		//wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, charlists);
		wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, MAX_CHAR, charlists);
	};


	void DrawString(const char * str)
	{
		for(; *str!='\0'; ++str){
			glCallList(charlists + *str);
		}
	};


	void Create3DTextLists(void)
	{
		CFont m_font;
		GLYPHMETRICSFLOAT agmf[256]; 
		m_font.CreateFont(
			-12,                            // Height Of Font
			0,                                // Width Of Font
			0,                                // Angle Of Escapement
			0,                                // Orientation Angle
			FW_BOLD,                        // Font Weight
			FALSE,                            // Italic
			FALSE,                            // Underline
			FALSE,                            // Strikeout
			ANSI_CHARSET,                    // Character Set Identifier
			OUT_TT_PRECIS,                    // Output Precision
			CLIP_DEFAULT_PRECIS,            // Clipping Precision
			ANTIALIASED_QUALITY,            // Output Quality
			FF_DONTCARE|DEFAULT_PITCH,        // Family And Pitch
			"Algerian");
		CFont* m_pOldFont = m_pDC->SelectObject(&m_font);
		// create display lists for glyphs 0 through 255 with 0.1 extrusion 
		// and default deviation. The display list numbering starts at 1000 
		// (it could be any number) 
		m_3DTextList = glGenLists(256);
		wglUseFontOutlines(m_pDC->GetSafeHdc(), 0, 255, m_3DTextList, 0.0f, 0.5f,WGL_FONT_POLYGONS, agmf); 
		m_pDC->SelectObject(m_pOldFont);

	};


	void Create2DTextLists(void)
	{
		CFont m_font;
		m_font.CreateFont(
			-12,                            // Height Of Font
			0,                                // Width Of Font
			0,                                // Angle Of Escapement
			0,                                // Orientation Angle
			FW_NORMAL,                        // Font Weight
			FALSE,                            // Italic
			FALSE,                            // Underline
			FALSE,                            // Strikeout
			ANSI_CHARSET,                    // Character Set Identifier
			OUT_TT_PRECIS,                    // Output Precision
			CLIP_DEFAULT_PRECIS,            // Clipping Precision
			DEFAULT_QUALITY,                // Output Quality
			FF_DONTCARE|DEFAULT_PITCH,        // Family And Pitch
			"Arial");
		CFont* m_pOldFont = m_pDC->SelectObject(&m_font);
		// create display lists for glyphs 0 through 255 with 0.1 extrusion 
		// and default deviation. The display list numbering starts at 1000 
		// (it could be any number) 
		m_2DTextList = glGenLists(256);
		wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, 255, m_2DTextList); 
		m_pDC->SelectObject(m_pOldFont);

	};



		// Setup the Pixel Format
	bool SetupPixelFormat(void)
	{
		static PIXELFORMATDESCRIPTOR pfd = 
		{
			sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
			1,                              // version number
			PFD_DRAW_TO_WINDOW |            // support window
			PFD_SUPPORT_OPENGL |            // support OpenGL
			PFD_DOUBLEBUFFER,                // double buffered
			PFD_TYPE_RGBA,                  // RGBA type
			24,                             // 24-bit color depth
			0, 0, 0, 0, 0, 0,               // color bits ignored
			0,                              // no alpha buffer
			0,                              // shift bit ignored
			0,                              // no accumulation buffer
			0, 0, 0, 0,                     // accum bits ignored
			16,                             // 16-bit z-buffer
			0,                              // no stencil buffer
			0,                              // no auxiliary buffer
			PFD_MAIN_PLANE,                 // main layer
			0,                              // reserved
			0, 0, 0                         // layer masks ignored
		};
		int m_nPixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
		if ( m_nPixelFormat == 0 ){
			return false;
		}
		if ( SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE ){
			return false;
		}
		return true;
	};


	void InitGL(void)
	{
		//Specify Black as the clear color
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		//Specify the back of the buffer as clear depth
		glClearDepth(1.0f);
		//Enable Depth Testing
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_SMOOTH);
		//opengl rendering mode
		//glShadeModel(GL_FLAT);
		glShadeModel(GL_SMOOTH);

		GenCharList();
		wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, 1, charlists);
		Create2DTextLists();
		Create3DTextLists();

	};

	// initialize opengl
	bool Init(void)
	{

		//Get a DC for the Client Area
		m_pDC = new CClientDC(this);
		//Failure to Get DC
		if(m_pDC == NULL){
			AfxMessageBox("Error Obtaining DC");
			return false;
		}
		//Failure to set the pixel format
		if(!SetupPixelFormat()){
			return false;
		}

		//Create Rendering Context
		m_hRC = wglCreateContext (m_pDC->GetSafeHdc());
		//Failure to Create Rendering Context
		if(m_hRC == 0){
			AfxMessageBox("Error Creating RC");
			return false;
		}
		//Make the RC Current
		if(wglMakeCurrent (m_pDC->GetSafeHdc(), m_hRC)==FALSE){
			AfxMessageBox("Error making RC Current");
			return false;
		}

		return true;

	};


	void End()
	{		
		//Make the RC non-current
		if(wglMakeCurrent(0,0) == FALSE){
			AfxMessageBox("Could not make RC non-current");
		}

		//Delete the rendering context
		if(wglDeleteContext(m_hRC)==FALSE){
			AfxMessageBox("Could not delete RC");
		}
		//Delete the DC
		if(m_pDC){
			delete m_pDC;
		}
		//Set it to NULL
		m_pDC = NULL;
	};

	void EndGL()
	{
		glDeleteLists(charlists,1);
		glDeleteLists(m_3DTextList,1);
		glDeleteLists(m_2DTextList,1);
	};

		// reshape window
	void Reshape(int w, int h, 
		GLdouble fieldOfViewAngleY=60,
		GLdouble znear=0.1,
		GLdouble zfar=200,
		GLdouble xylen=3)
	{
		GLdouble aspect_ratio; // width/height ratio

		// select the full client area
		glViewport(0, 0, w, h);
		// compute the aspect ratio
		// this will keep all dimension scales equal
		aspect_ratio = (GLdouble)w/(GLdouble)h;
		// select the projection matrix and clear it
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// select the viewing volume

		if(isOrtho){
			if (aspect_ratio <= 1)
				glOrtho(-xylen, xylen, -xylen/aspect_ratio, xylen/aspect_ratio,-zfar,zfar);
			else
				glOrtho(-xylen*aspect_ratio, xylen*aspect_ratio, -xylen, xylen,-zfar,zfar);
		}
		else{
			gluPerspective(fieldOfViewAngleY, aspect_ratio, znear, zfar);
		}

		// switch back to the modelview matrix and clear it
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	};
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
	virtual ~Co6View();
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in o6View.cpp
inline Co6Doc* Co6View::GetDocument() const
   { return reinterpret_cast<Co6Doc*>(m_pDocument); }
#endif

