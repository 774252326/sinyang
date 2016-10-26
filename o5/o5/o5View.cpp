
// o5View.cpp : implementation of the Co5View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "o5.h"
#endif

#include "o5Doc.h"
#include "o5View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Co5View

IMPLEMENT_DYNCREATE(Co5View, CView)

	BEGIN_MESSAGE_MAP(Co5View, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Co5View::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEWHEEL()
		ON_WM_CHAR()
		ON_WM_MOUSEMOVE()
	END_MESSAGE_MAP()

	// Co5View construction/destruction

	Co5View::Co5View()
		: m_pDC(NULL)
		, m_3DTextList(0)
		, m_2DTextList(0)
		, m_aspectRatio(0)
		, m_xAngle(0)
		, m_yAngle(0)
		, m_xPos(0)
		, m_yPos(0)
		, m_zPos(-1)
		, m_MouseDownPoint(0)
	{
		// TODO: add construction code here

	}

	Co5View::~Co5View()
	{
	}

	BOOL Co5View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CView::PreCreateWindow(cs);
	}

	// Co5View drawing

	void Co5View::OnDraw(CDC* /*pDC*/)
	{
		Co5Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here


		// Clear out the color & depth buffers
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		RenderScene();
		// Tell OpenGL to flush its pipeline
		::glFinish();
		// Now Swap the buffers
		::SwapBuffers( m_pDC->GetSafeHdc() );

	}


	// Co5View printing


	void Co5View::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL Co5View::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void Co5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void Co5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void Co5View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void Co5View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Co5View diagnostics

#ifdef _DEBUG
	void Co5View::AssertValid() const
	{
		CView::AssertValid();
	}

	void Co5View::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	Co5Doc* Co5View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Co5Doc)));
		return (Co5Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Co5View message handlers


	int Co5View::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		//Initialize OpenGL Here
		InitializeOpenGL();


		return 0;
	}


	void Co5View::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: Add your message handler code here


		//Make the RC non-current
		if(::wglMakeCurrent (0,0) == FALSE)
		{
			MessageBox(L"Could not make RC non-current");
		}

		//Delete the rendering context
		if(::wglDeleteContext (m_hRC)==FALSE)
		{
			MessageBox(L"Could not delete RC");
		}
		//Delete the DC
		if(m_pDC)
		{
			delete m_pDC;
		}
		//Set it to NULL
		m_pDC = NULL;


	}


	void Co5View::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		if ( cx>0 && cy>0 ){
			reshape(cx,cy);
		}


	}


	BOOL Co5View::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: Add your message handler code here and/or call default

		return TRUE;

		return CView::OnEraseBkgnd(pDC);
	}


	// initialize opengl
	BOOL Co5View::InitializeOpenGL(void)
	{
		//Get a DC for the Client Area
		m_pDC = new CClientDC(this);
		//Failure to Get DC
		if(m_pDC == NULL)
		{
			MessageBox(L"Error Obtaining DC");
			return FALSE;
		}
		//Failure to set the pixel format
		if(!SetupPixelFormat())
		{
			return FALSE;
		}
		//Create Rendering Context
		m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());
		//Failure to Create Rendering Context
		if(m_hRC == 0)
		{
			MessageBox(L"Error Creating RC");
			return FALSE;
		}
		//Make the RC Current
		if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
		{
			MessageBox(L"Error making RC Current");
			return FALSE;
		}

		init();

		return TRUE;

	}


	// setup pixel format
	BOOL Co5View::SetupPixelFormat(void)
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
		int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
		if ( m_nPixelFormat == 0 )
		{
			return FALSE;
		}
		if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
		{
			return FALSE;
		}
		return TRUE;

	}


	// correspond to reshape() using glut
	void Co5View::reshape(int cx, int cy)
	{
		// select the full client area
		::glViewport(0, 0, cx, cy);
		// compute the aspect ratio
		// this will keep all dimension scales equal
		m_aspectRatio = (GLdouble)cx/(GLdouble)cy;
		// select the projection matrix and clear it
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();
		// select the viewing volume
		::gluPerspective(45.0f, m_aspectRatio, .01f, 200.0f);
		// switch back to the modelview matrix and clear it
		::glMatrixMode(GL_MODELVIEW);
		::glLoadIdentity();
	}


	void Co5View::SetupLighting(void)
	{
		//Material Properties
		GLfloat matSpecular[] = { 1.0f, 0.0f, 0.0f, 1.0f};
		GLfloat matShininess[] = { 50.0f};
		GLfloat matAmbient[] = { 0.25f, 0.25f, 0.25f, 1.0f};
		GLfloat matDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f};
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
		//Lighting Parameters
		//Enable Lighting
		glEnable(GL_LIGHTING);
		//Specify a single directional light
		GLfloat ambient1[] = { 0.5f,0.5f,0.5f};
		GLfloat diffuse1[] = { 0.5f,0.5f,0.5f};
		GLfloat specular1[] = { 1.0f,0.0f,0.0f};
		GLfloat position1[] = { 0.0f,0.0f,5.0f,0.0};
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);
		glLightfv(GL_LIGHT0, GL_POSITION, position1);    
		glEnable(GL_LIGHT0);
		//Specify a single positional spotlight
		GLfloat ambient2[] = { 1.0f,1.0f,0.0f};
		GLfloat diffuse2[] = { 1.0f,0.0f,0.0f};
		GLfloat position2[] = { 1.0f,0.0f,5.0f,1.0};
		GLfloat direction2[] = {0.0f,0.0f,-5.0f};
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
		glLightfv(GL_LIGHT1, GL_POSITION, position2);    
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction2);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0f);
		glEnable(GL_LIGHT1);

	}


	// correspond to init() using glut
	void Co5View::init(void)
	{
		//Specify Black as the clear color
		::glClearColor(0.0f,0.0f,0.0f,0.0f);
		//Specify the back of the buffer as clear depth
		::glClearDepth(1.0f);
		//Enable Depth Testing
		::glEnable(GL_DEPTH_TEST);

		glShadeModel(GL_FLAT);
		SetupLighting();
		//Create Font Display Lists
		Create2DTextLists();
		Create3DTextLists();

	}


	// render scene
	void Co5View::RenderScene(void)
	{
		glLoadIdentity();
		glTranslatef(m_xPos, m_yPos, m_zPos);
		glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
		glRotatef(m_yAngle, 0.0f,1.0f,0.0f);

		glDisable(GL_LIGHTING);

		glColor3d(1,1,1);
		glutWireCube(1);
	}


	void Co5View::Create3DTextLists(void)
	{
		CFont m_font;
		GLYPHMETRICSFLOAT agmf[256]; 
		m_font.CreateFont(    -12,                            // Height Of Font
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
			L"Arial");
		CFont* m_pOldFont = m_pDC->SelectObject(&m_font);
		// create display lists for glyphs 0 through 255 with 0.1 extrusion 
		// and default deviation. The display list numbering starts at 1000 
		// (it could be any number) 
		m_3DTextList = glGenLists(256);
		wglUseFontOutlines(m_pDC->GetSafeHdc(), 0, 255, m_3DTextList, 0.0f, 0.5f,WGL_FONT_POLYGONS, agmf); 
		m_pDC->SelectObject(m_pOldFont);

	}


	void Co5View::Create2DTextLists(void)
	{
		CFont m_font;
		m_font.CreateFont(    -20,                            // Height Of Font
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
			L"Arial");
		CFont* m_pOldFont = m_pDC->SelectObject(&m_font);
		// create display lists for glyphs 0 through 255 with 0.1 extrusion 
		// and default deviation. The display list numbering starts at 1000 
		// (it could be any number) 
		m_2DTextList = glGenLists(256);
		wglUseFontBitmaps(m_pDC->GetSafeHdc(), 0, 255, m_2DTextList); 
		m_pDC->SelectObject(m_pOldFont);

	}

	void Co5View::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default


		m_MouseDownPoint=point;
		SetCapture();


		CView::OnLButtonDown(nFlags, point);
	}


	void Co5View::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		m_MouseDownPoint=CPoint(0,0);
		ReleaseCapture();

		CView::OnLButtonUp(nFlags, point);
	}


	BOOL Co5View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default

		m_zPos *=(zDelta>0) ? 0.8:1.25;

		TRACE("%f,",m_zPos);

		InvalidateRect(NULL,FALSE);


		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void Co5View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: Add your message handler code here and/or call default


		switch(nChar){
		//case 'f':
		//	//raidus*=0.8;
		//	InvalidateRect(NULL,FALSE);
		//	break;
		//case 'd':
		//	//raidus/=0.8;
		//	InvalidateRect(NULL,FALSE);
		//	break;
		case 'z':
			m_xPos=m_yPos=0;m_zPos=-1;
			m_xAngle=m_yAngle=0;
			InvalidateRect(NULL,FALSE);
			break;
		default:
			break;
		}


		CView::OnChar(nChar, nRepCnt, nFlags);
	}


	void Co5View::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default


		// Check if we have captured the mouse
		if (GetCapture()==this)
		{
			//Increment the object rotation angles
			m_xAngle+=(point.y-m_MouseDownPoint.y)/3.6;
			m_yAngle+=(point.x-m_MouseDownPoint.x)/3.6;
			//Redraw the view
			InvalidateRect(NULL,FALSE);
			//Set the mouse point
			m_MouseDownPoint=point;
		}


		CView::OnMouseMove(nFlags, point);
	}
