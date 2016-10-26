
// o4View.cpp : implementation of the Co4View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "o4.h"
#endif

#include "o4Doc.h"
#include "o4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <string.h>
#include "../../o3/o3/ro.h"


// Co4View

IMPLEMENT_DYNCREATE(Co4View, CView)

	BEGIN_MESSAGE_MAP(Co4View, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Co4View::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_WM_CHAR()
		ON_COMMAND(ID_FILE_OPEN, &Co4View::OnFileOpen)
	END_MESSAGE_MAP()

	// Co4View construction/destruction

	Co4View::Co4View()
		: m_pDC(NULL)
		, m_3DTextList(0)
		, m_2DTextList(0)
		, m_borderList(0)
		, m_meshList(0)
		, m_aspectRatio(0)
		, m_surfaceList(0)
		, m_xAngle(0)
		, m_yAngle(0)
		, m_xPos(0)
		, m_yPos(0)
		, m_zPos(-1)
		, m_MouseDownPoint(0)
		, raidus(0.01)
	{
		// TODO: add construction code here

	}

	Co4View::~Co4View()
	{
	}

	BOOL Co4View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs


		//An OpenGL Window must be created with the following flags
		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;


		return CView::PreCreateWindow(cs);
	}

	// Co4View drawing

	void Co4View::OnDraw(CDC* /*pDC*/)
	{
		Co4Doc* pDoc = GetDocument();
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


	// Co4View printing


	void Co4View::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL Co4View::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void Co4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void Co4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void Co4View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void Co4View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Co4View diagnostics

#ifdef _DEBUG
	void Co4View::AssertValid() const
	{
		CView::AssertValid();
	}

	void Co4View::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	Co4Doc* Co4View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Co4Doc)));
		return (Co4Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Co4View message handlers


	int Co4View::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		//Initialize OpenGL Here
		InitializeOpenGL();

		return 0;
	}


	void Co4View::OnDestroy()
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


	void Co4View::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here



		if ( 0 >= cx || 0 >= cy )
		{
			return;
		}

		reshape(cx,cy);
	}


	BOOL Co4View::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: Add your message handler code here and/or call default

		return TRUE;

		return CView::OnEraseBkgnd(pDC);
	}


	// initialize opengl
	BOOL Co4View::InitializeOpenGL(void)
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
	BOOL Co4View::SetupPixelFormat(void)
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


	// render scene
	void Co4View::RenderScene(void)
	{
		//glTranslatef(0,0,-3);
		//glRotatef(100,1.0f,0.0f,0.0f);
		//glRotatef(100,0.0f,1.0f,0.0f);
		//glRotatef(100,0.0f,0.0f,1.0f);

		//glutSolidCube(1.0f);


		glLoadIdentity();
		glTranslatef(m_xPos, m_yPos, m_zPos);
		glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
		glRotatef(m_yAngle, 0.0f,1.0f,0.0f);
		//glutWireCube(1.0f);

		//glTranslatef(-2.0f,0.0f,-5.0f);
		//glRotatef(-10.0,1.0f,0.0f,0.0f);    
		//glRotatef(-10.0,0.0f,1.0f,0.0f);    

		glDisable(GL_LIGHTING);
		//glCallList(m_meshList);
		//glCallList(m_borderList);
		//glCallList(m_surfaceList);
		//char str[]="1234567890";

		////2D文本
		//// Position The Text On The Screen
		//glDisable(GL_LIGHTING);
		//glColor3f(1.0f,1.0f,0.0f);
		//glRasterPos2f(0,0);
		//glListBase(m_2DTextList);
		////glCallLists(6, GL_UNSIGNED_BYTE ,"OpenGL");
		//glCallLists(strlen(str), GL_UNSIGNED_BYTE ,str);

		//glEnable(GL_LIGHTING);
		//TRACE("%d,",strlen(str));

		//3D文本
		//glListBase(m_3DTextList);
		//glCallLists(6, GL_UNSIGNED_BYTE ,"OpenGL");

		//////////////////////////////////////////////////////////////////

		size_t i;

		for(i=0;i<pc.plist.size();i++){
			glPushMatrix();
			glColor3dv(pc.clist[i].pt3);
			glTranslated(pc.plist[i].pt3[2],pc.plist[i].pt3[1],pc.plist[i].pt3[0]);
			//glutSolidSphere(raidus,30,30);
			glutSolidCube(raidus);
			glPopMatrix();
		}

		glColor3d(1,1,1);
		glutWireCube(pc.range);
		//glutWireCube(pc.range/2);
		//glutWireCube(pc.range/4);
		//glutWireCube(pc.range/8);
		//glutWireCube(pc.range/16);
		//glutWireCube(pc.range/20);
	}


	// correspond to init() using glut
	void Co4View::init(void)
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

		////wchar_t fp[]=L"C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\octahedron.txt";
		//wchar_t fp[]=L"C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\grid05022013_2.txt";
		//wchar_t fp[]=L"C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\gridSurface05022013_2.txt";
		//long nd;
		//double **dt=readf2<double>(fp,&nd,10);

		//surfaceMesh sfm;
		//sfm.loadKWaferLevelContourNodeVector(dt,nd,0);
		//createMeshList(sfm);
		//createBorderList(sfm);
		//createSurfaceList(sfm);

		//wchar_t fp[]=L"C:\\Users\\r8anw2x\\Desktop\\test_01.txt";
		wchar_t fp[]=L"C:\\Users\\r8anw2x\\Documents\\MATLAB\\byo3d\\mlShadowScan\\models\\man_v1-lr.wrl";
		//wchar_t fp[]=L"C:\\Users\\r8anw2x\\Documents\\MATLAB\\byo3d\\mlShadowScan\\models\\man_v1.wrl";
		//wchar_t fp[]=L"C:\\Users\\r8anw2x\\Dropbox\\W\\byo3d\\patate.wrl";
		pc.load(fp);
		pc.unity(1);
		//pc.warp(0.025,0.025,1);
		//pc.unity(1);

	}


	// correspond to reshape() using glut
	void Co4View::reshape(int cx, int cy)
	{
		//GLdouble aspect_ratio; // width/height ratio
		//// select the full client area
		//::glViewport(0, 0, cx, cy);
		//// compute the aspect ratio
		//// this will keep all dimension scales equal
		//aspect_ratio = (GLdouble)cx/(GLdouble)cy;
		//// select the projection matrix and clear it
		//::glMatrixMode(GL_PROJECTION);
		//::glLoadIdentity();
		//// select the viewing volume
		//::gluPerspective(45.0f, aspect_ratio, .01f, 200.0f);

		//// switch back to the modelview matrix and clear it
		//::glMatrixMode(GL_MODELVIEW);
		//::glLoadIdentity();

		/////////////////////////////////////////////////////////

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
		//::gluOrtho2D(-10.0f, 10.0f, -10.0f, 10.0f);  

		//	double kk=0.2,dd=10;
		//		if (cx <= cy)
		//glOrtho (-kk, kk, -kk*(GLfloat) cy/(GLfloat) cx, kk*(GLfloat) cy/(GLfloat) cx,-dd,dd);
		//else
		//glOrtho (-kk*(GLfloat)cx/(GLfloat) cy, kk*(GLfloat) cx/(GLfloat) cy, -kk, kk,-dd,dd);

		// switch back to the modelview matrix and clear it
		::glMatrixMode(GL_MODELVIEW);
		::glLoadIdentity();


	}


	void Co4View::Create3DTextLists(void)
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


	void Co4View::Create2DTextLists(void)
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


	void Co4View::SetupLighting(void)
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


	void Co4View::createMeshList(const surfaceMesh & sfm)
	{
		GLfloat linewidth=2;
		size_t i,j;
		m_meshList = glGenLists(1);
		glNewList(m_meshList, GL_COMPILE);

		glLineWidth(linewidth);
		glColor3f(1.0f, 1.0f, 1.0f); 

		for(i=1;i<sfm.triangle.size();i++){			
			glBegin (GL_LINE_LOOP);
			for(j=0;j<sfm.triangle[i].size();j++){
				glVertex3dv(sfm.point[sfm.triangle[i][j]].data());
			}
			glEnd();
			//glLineWidth(1);
		}
		glEndList();
	}
	void Co4View::createSurfaceList(const surfaceMesh & sfm)
	{
		m_surfaceList = glGenLists(1);
		glNewList(m_surfaceList, GL_COMPILE);

		size_t i,j;

		float v1[3];
		float v2[3];
		float nor[3];
		float innerp[3]={0,0,0};

		float rgba[4];

		float mxcv=sfm.maxTriangleValue;
		float mncv=sfm.minTriangleValue;

		for(i=1;i<sfm.triangle.size();i++){
			glEnable(GL_NORMALIZE);

			genColor(rgba,(sfm.triangleValue[i]-mncv)/(mxcv-mncv)*0.8);
			glColor4fv(rgba);

			glBegin(GL_TRIANGLES);

			for(j=0;j<sfm.triangle[i].size();j++){
				glVertex3dv(sfm.point[sfm.triangle[i][j]].data());
			}
			glEnd();
			glDisable(GL_NORMALIZE);
		}
		glEndList();

	}
	void Co4View::createBorderList(const surfaceMesh & sfm)
	{
		GLfloat linewidth=2;
		size_t i,j;
		m_borderList = glGenLists(1);
		glNewList(m_borderList, GL_COMPILE);

		glLineWidth(linewidth);
		glColor3f(1.0f, 1.0f, 1.0f); 

		for(i=0;i<sfm.border.size();i++){			
			glBegin (GL_LINE_STRIP);
			for(j=0;j<sfm.border[i].size();j++){
				glVertex3dv(sfm.point[sfm.border[i][j]].data());
			}
			glEnd();
			//glLineWidth(1);
		}
		glEndList();
	}


	void Co4View::ProcessSelection(CPoint point)
	{
		int xPos = point.x ;
		int yPos = point.y ;
		GLuint selectBuff[64];
		GLint hits, viewport[4];
		glSelectBuffer(64, selectBuff);
		glGetIntegerv(GL_VIEWPORT, viewport);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glRenderMode(GL_SELECT);
		glLoadIdentity();
		gluPickMatrix(xPos, viewport[3] - yPos, 2,2, viewport);
		gluPerspective(45.0f, m_aspectRatio, .01f, 200.0f);
		RenderScene();
		hits = glRenderMode(GL_RENDER);

		ProcessObject(selectBuff,hits);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

	}





	void Co4View::ProcessObject(GLuint* pSelectBuff, GLint hits)
	{

		int i;
		unsigned int j;
		GLuint names, *ptr;

		char objectname[2][10]={"sphere","cube"};

		CString str,msg;		

		//printf ("hits = %d\n", hits);

		str.Format(L"hits = %d\n", hits);
		msg=str;		

		ptr = (GLuint *) pSelectBuff;
		for (i = 0; i < hits; i++) { /* for each hit */
			names = *ptr;
			//printf(" number of names for hit = %d\n", names); ptr++;
			str.Format(L" number of names for hit = %d\n", names); ptr++;
			msg=msg+str;

			//printf(" z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
			str.Format(L" z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
			msg=msg+str;

			//printf(" z2 is %g\n", (float) *ptr/0x7fffffff); ptr++;
			str.Format(L" z2 is %g\n", (float) *ptr/0x7fffffff); ptr++;
			msg=msg+str;

			//printf(" the name is ");
			str.Format(L" the name is ");
			msg=msg+str;

			for (j = 0; j < names; j++) { /* for each name */
				//printf("%d ", *ptr); ptr++;
				//str.Format("%d ", *ptr); ptr++;
				str.Format(L"%s ", objectname[(*ptr)-1]); ptr++;
				msg=msg+str;
			}
			//printf ("\n");
			str.Format(L"\n");
			msg=msg+str;
		}

		MessageBox(msg);
	}


	void Co4View::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		//ProcessSelection(point);

		m_MouseDownPoint=point;
		SetCapture();


		CView::OnLButtonDown(nFlags, point);
	}



	void Co4View::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		m_MouseDownPoint=CPoint(0,0);
		ReleaseCapture();



		CView::OnLButtonUp(nFlags, point);
	}


	void Co4View::OnMouseMove(UINT nFlags, CPoint point)
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
		};



		CView::OnMouseMove(nFlags, point);
	}


	BOOL Co4View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default

		m_zPos *=(zDelta>0) ? 0.8:1.25;

		TRACE("%f,",m_zPos);

		InvalidateRect(NULL,FALSE);

		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}



	void Co4View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: Add your message handler code here and/or call default

		switch(nChar){
		case 'f':
			raidus*=0.8;
			InvalidateRect(NULL,FALSE);
			break;
		case 'd':
			raidus/=0.8;
			InvalidateRect(NULL,FALSE);
			break;
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


	void Co4View::OnFileOpen()
	{
		// TODO: Add your command handler code here

		CFileDialog fileDlg(true);

		//fileDlg.m_ofn.lpstrFilter=L"OBJ File(*.obj)\0*.obj\0Text File(*.txt)\0*.txt\0\0";
		fileDlg.m_ofn.lpstrFilter=L"WRL File(*.wrl)\0*.wrl\0Text File(*.txt)\0*.txt\0\0";
		if( fileDlg.DoModal()==IDOK){
			CString str;
			str=fileDlg.GetPathName();
			this->GetParent()->SetWindowTextW(str);
			/////////////////////////////////////////
			pc.load(str.GetBuffer());
			pc.unity(1);
			InvalidateRect(NULL,FALSE);
		}


	}
