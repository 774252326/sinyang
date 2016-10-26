
// o3View.cpp : implementation of the Co3View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "o3.h"
#endif

#include "o3Doc.h"
#include "o3View.h"

#include "../../funT\nrutilT.h"
//#include "glm.h"
#include "ro.h"
//#include "../../funT\findmT.h"
//#include "../../funT\matmulT.h"
//#include "tm\triangleMesh.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Co3View

IMPLEMENT_DYNCREATE(Co3View, CView)

	BEGIN_MESSAGE_MAP(Co3View, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Co3View::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		ON_WM_DESTROY()
		ON_COMMAND(ID_VIEW_ROTATE, &Co3View::OnViewRotate)
		ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE, &Co3View::OnUpdateViewRotate)
		ON_COMMAND(ID_VIEW_TRANSLATE, &Co3View::OnViewTranslate)
		ON_UPDATE_COMMAND_UI(ID_VIEW_TRANSLATE, &Co3View::OnUpdateViewTranslate)
		ON_COMMAND(ID_VIEW_ZOOM, &Co3View::OnViewZoom)
		ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, &Co3View::OnUpdateViewZoom)
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_COMMAND(ID_VIEW_RESET, &Co3View::OnViewReset)
		ON_COMMAND(ID_FILE_OPEN, &Co3View::OnFileOpen)
		ON_COMMAND(ID_VIEW_ORTHO, &Co3View::OnViewOrtho)
		ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHO, &Co3View::OnUpdateViewOrtho)
		ON_COMMAND(ID_VIEW_MESH, &Co3View::OnViewMesh)
		ON_UPDATE_COMMAND_UI(ID_VIEW_MESH, &Co3View::OnUpdateViewMesh)
		ON_COMMAND(ID_VIEW_SURFACE, &Co3View::OnViewSurface)
		ON_UPDATE_COMMAND_UI(ID_VIEW_SURFACE, &Co3View::OnUpdateViewSurface)
		ON_COMMAND(ID_VIEW_SMOOTHSURFACE, &Co3View::OnViewSmoothsurface)
		ON_UPDATE_COMMAND_UI(ID_VIEW_SMOOTHSURFACE, &Co3View::OnUpdateViewSmoothsurface)
		ON_COMMAND(ID_VIEW_CONTOUR, &Co3View::OnViewContour)
		ON_UPDATE_COMMAND_UI(ID_VIEW_CONTOUR, &Co3View::OnUpdateViewContour)
		ON_COMMAND(ID_VIEW_PLANE, &Co3View::OnViewPlane)
		ON_UPDATE_COMMAND_UI(ID_VIEW_PLANE, &Co3View::OnUpdateViewPlane)
	END_MESSAGE_MAP()

	// Co3View construction/destruction

	Co3View::Co3View()
		: m_pDC(NULL)
		, mouseMode(1)
		, xRot(0)
		, yRot(0)
		, mouseDownPoint(0)
		, rotateStep(0.2)
		, translateStep(0.2)
		, zoomStep(0.2)
		, xPos(0)
		, yPos(0)
		, zoomFactor(1)
		, m_filePath(_T(""))
		, m_DPList(0)
		, isOrtho(false)
		, m_surfaceList(0)
		, m_meshList(0)
		, checkMesh(true)
		, checkSurface(true)
		, checkSmoothSurface(true)
		, m_unityCube(0)
		, checkContour(true)
		, m_contourList(0)
		, m_surfaceList3(0)
		, isplane(0)
	{
		// TODO: add construction code here

	}

	Co3View::~Co3View()
	{
	}

	BOOL Co3View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs


		//An OpenGL Window must be created with the following flags
		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		return CView::PreCreateWindow(cs);
	}

	// Co3View drawing

	void Co3View::OnDraw(CDC* /*pDC*/)
	{
		Co3Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here


		renderScene();

		//CPen pen

		// Now Swap the buffers
		SwapBuffers( m_pDC->GetSafeHdc() );
	}


	// Co3View printing


	void Co3View::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL Co3View::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void Co3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void Co3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void Co3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void Co3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Co3View diagnostics

#ifdef _DEBUG
	void Co3View::AssertValid() const
	{
		CView::AssertValid();
	}

	void Co3View::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	Co3Doc* Co3View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Co3Doc)));
		return (Co3Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Co3View message handlers


	int Co3View::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		Init();

		return 0;
	}


	// initialize opengl
	bool Co3View::Init(void)
	{

		//Get a DC for the Client Area
		m_pDC = new CClientDC(this);
		//Failure to Get DC
		if(m_pDC == NULL){
			AfxMessageBox(L"Error Obtaining DC");
			return false;
		}
		//Failure to set the pixel format
		if(!setupPixelFormat()){
			return false;
		}

		//Create Rendering Context
		m_hRC = wglCreateContext (m_pDC->GetSafeHdc());
		//Failure to Create Rendering Context
		if(m_hRC == 0){
			AfxMessageBox(L"Error Creating RC");
			return false;
		}
		//Make the RC Current
		if(wglMakeCurrent (m_pDC->GetSafeHdc(), m_hRC)==FALSE){
			AfxMessageBox(L"Error making RC Current");
			return false;
		}

		InitGL();

		return true;

	}


	// Setup the Pixel Format
	bool Co3View::setupPixelFormat(void)
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
	}


	// render scene
	void Co3View::renderScene(void)
	{
		// Clear out the color & depth buffers
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


		glMatrixMode(GL_PROJECTION);

		//gluLookAt(0,3,3,0,0,0,0,1,0);
		glPushMatrix();


		//glLoadIdentity();

		glTranslatef(xPos,yPos,-3);
		glScalef(zoomFactor,zoomFactor,1);
		glRotatef(xRot,1,0,0);
		glRotatef(yRot,0,1,0);


		//	glBegin (GL_POLYGON);
		//glColor3f(1,0,0);
		//glVertex3f (0, 0, 0);


		//glColor3f(0,1,0);
		//glVertex3f (-1, 0, 0);
		//glColor3f(0,1,1);
		//glVertex3f (-1, -1, 0);
		//glColor3f(0,0,1);
		//glVertex3f (0, -1, 0);
		//glEnd();


		glCallList(m_unityCube);
		//glCallList(m_DPList);

		if(checkContour)
			glCallList(m_contourList);
		if(checkMesh)
			glCallList(m_meshList);
		if(checkSurface){
			if(checkSmoothSurface)
				glCallList(m_surfaceList2);
			else
				glCallList(m_surfaceList);
		}




		//glutWireCube(1);

		glPopMatrix();

		// Tell OpenGL to flush its pipeline
		//glFinish();
		glFlush();
	}


	// reshape window
	void Co3View::reshape(int w, int h)
	{
		GLdouble aspect_ratio; // width/height ratio
		GLdouble fieldOfViewAngleY=60;
		GLdouble znear=0.1;
		GLdouble zfar=200;
		GLdouble xylen=3;


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



		//glEnable(GL_LIGHTING);
		//GLfloat light_position[] = { 0.0, 0.0, 1.0, 1.0 };
		//GLfloat light_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
		//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		//glEnable(GL_LIGHT0);
	}


	void Co3View::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		if ( cx>0 && cy>0 ){
			reshape(cx,cy);
			//CRect prect;
			//this->GetWindowRect(prect);
			//CString str;
			//str.Format(L"%d=%d,%d=%d",cx,prect.Width(),cy,prect.Height());
			//AfxMessageBox(str);

		}
		else{
			//AfxMessageBox(L"empty rect");
		}
	}


	BOOL Co3View::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: Add your message handler code here and/or call default

		return TRUE;

		return CView::OnEraseBkgnd(pDC);
	}


	void Co3View::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: Add your message handler code here

		//Make the RC non-current
		if(wglMakeCurrent(0,0) == FALSE){
			AfxMessageBox(L"Could not make RC non-current");
		}

		//Delete the rendering context
		if(wglDeleteContext(m_hRC)==FALSE){
			AfxMessageBox(L"Could not delete RC");
		}
		//Delete the DC
		if(m_pDC){
			delete m_pDC;
		}
		//Set it to NULL
		m_pDC = NULL;

		glDeleteLists(m_DPList,1);
		glDeleteLists(m_meshList,1);
		glDeleteLists(m_surfaceList,1);
		glDeleteLists(m_surfaceList2,1);
		glDeleteLists(m_surfaceList3,1);
	}

	void Co3View::InitGL(void)
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


		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

		//gluLookAt(0,3,3,0,0,0,0,1,0);

		//glEnable(GL_LIGHTING);
		//GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

		//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		//glEnable(GL_LIGHT0);


		//setupLighting();


	}

	void Co3View::OnViewRotate()
	{
		// TODO: Add your command handler code here

		mouseMode=1;


	}





	void Co3View::OnUpdateViewRotate(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(mouseMode==1);
	}


	void Co3View::OnViewTranslate()
	{
		// TODO: Add your command handler code here
		mouseMode=2;
	}


	void Co3View::OnUpdateViewTranslate(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(mouseMode==2);
	}


	void Co3View::OnViewZoom()
	{
		// TODO: Add your command handler code here
		mouseMode=3;
	}


	void Co3View::OnUpdateViewZoom(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(mouseMode==3);

	}


	void Co3View::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		mouseDownPoint=point;
		SetCapture();

		CView::OnLButtonDown(nFlags, point);
	}


	void Co3View::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		mouseDownPoint=CPoint(0,0);
		ReleaseCapture();

		CView::OnLButtonUp(nFlags, point);
	}


	void Co3View::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		// Check if we have captured the mouse
		if (GetCapture()==this)
		{
			switch(mouseMode){
			case 1:
				//Increment the object rotation angles
				xRot+=(point.y-mouseDownPoint.y)*rotateStep;
				if(xRot>360)
					xRot-=360;

				yRot+=(point.x-mouseDownPoint.x)*rotateStep;
				if(yRot>360)
					yRot-=360;

				//Redraw the view
				InvalidateRect(NULL,FALSE);
				//Set the mouse point
				mouseDownPoint=point;
				break;
			case 2:

				xPos+=(point.x-mouseDownPoint.x)*translateStep;
				yPos+=(mouseDownPoint.y-point.y)*translateStep;

				//Redraw the view
				InvalidateRect(NULL,FALSE);
				//Set the mouse point
				mouseDownPoint=point;

				break;
			case 3:
				zoomFactor*=exp(zoomStep*(point.x-mouseDownPoint.x));

				//Redraw the view
				InvalidateRect(NULL,FALSE);
				//Set the mouse point
				mouseDownPoint=point;

				break;

			default:
				break;
			}

		}



		CView::OnMouseMove(nFlags, point);
	}


	void Co3View::OnViewReset()
	{
		// TODO: Add your command handler code here
		mouseMode=1;
		xRot=yRot=xPos=yPos=0;
		zoomFactor=1;
		//Redraw the view
		InvalidateRect(NULL,FALSE);
	}


	void Co3View::OnFileOpen()
	{
		// TODO: Add your command handler code here

		CFileDialog fileDlg(true);

		//fileDlg.m_ofn.lpstrFilter=L"OBJ File(*.obj)\0*.obj\0Text File(*.txt)\0*.txt\0\0";
		fileDlg.m_ofn.lpstrFilter=L"Text File(*.txt)\0*.txt\0OBJ File(*.obj)\0*.obj\0\0";
		if( fileDlg.DoModal()==IDOK){
			m_filePath=fileDlg.GetPathName();


			/////////////////////////////////////////



			//GLfloat scalefactor = 0.0;
			////Load Computer from file
			//GLMmodel *object1;
			////object1 = glmReadOBJ("models/computer.obj");
			//object1 = glmReadOBJWideChar(m_filePath.GetBuffer());
			//if(!scalefactor) 
			//{
			//	scalefactor = glmUnitize(object1);
			//} 
			//else 
			//{
			//	glmScale(object1, scalefactor);
			//}
			////glmScale(object1, 2);    
			////* build a display list */
			//m_DPList = glmList(object1, GLM_SMOOTH);
			////* nuke it, we don't need it anymore */
			//glmDelete(object1);  

			//////////////////////////////////////////////


			//long r=1850,c=3,h=3;
			////long r=1604,c=3,h=4;
			//	//long r=3696,c=3,h=4;
			//float ***dt=f3tensor<float>(1,r,1,c,1,h);

			//float *colorv=vector<float>(1,r);

			//readf3(m_filePath.GetBuffer(),dt,colorv,r,c,h);
			////readf3(m_filePath.GetBuffer(),dt,r,c,h);
			//createSceneList(dt,colorv,r,c,h);
			//free_f3tensor(dt,1,r,1,c,1,h);
			//free_vector(colorv,1,r);



			/////////////////////////////////////////////////



			float **dt;
			long nf,np;

			dt=readf2<float>(m_filePath.GetBuffer(),&nf,10);

			float *facecolor=vector<float>(1,nf);
			long **face=matrix<long>(1,nf,1,3);
			float **point;

			point=genPFlist(dt,nf,face,facecolor,&np);

			float *pv=vector<float>(1,np);
			for(long i=1;i<=np;i++){
				pv[i]=point[i][4];
				//pv[i]=point[i][1]*point[i][1]+point[i][2]*point[i][2];
			}

			//long **face=readf2<long>(L"C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\05021013\\gridSurface05022013_2.triangle.txt",&nf,3);
			//float **point=readf2<float>(L"C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\05021013\\gridSurface05022013_2.point.txt",&np,3);
			//float *pv=readf1<float>(L"C:\\Users\\r8anw2x\\Dropbox\\OpenGL\\data\\05021013\\gridSurface05022013_2.pointvaluei.txt",&np);
			
			////////////////////////////////////////////////////////////////////////////////
			//triangleMesh trm1;
			//trm1.loadFace(face,nf);
			////trm1.loadFaceV(facecolor,nf);
			//trm1.loadPoint(point,np);
			//trm1.loadPointV(pv,np);



			//trm1.getFaceNext();
			//trm1.genEdge(0);
			//trm1.genPointToFaceMap();
			//trm1.genFaceCentroid();
			//trm1.interpPointV();
			//trm1.genContourMap(50);
			//trm1.genContourMapInterp(50);



			triangleMesh trm1;

			trm1.loadPointAndFace(point,np,face,nf,isplane);
			//for(long i=1;i<=nf;i++){
			//	facecolor[i]=trm1.faceCentroid[i][1]*trm1.faceCentroid[i][1]+trm1.faceCentroid[i][2]*trm1.faceCentroid[i][2];
			//}
			trm1.loadFaceV(facecolor,nf);
			//trm1.loadPointV(pv,np);
			trm1.interpPointV();
			trm1.genContourMap(50);

			
			////////////////////////create display list //////////////////////////////////
			//createMeshList(face,nf,point,np);
			//createSurfaceList(face,facecolor,nf,point,np);
			//createSurfaceList2(face,facecolor,nf,point,np);
			//createUnityCubeList();


			createMeshList(trm1);
			createSurfaceList(trm1);
			createSurfaceList2(trm1);
			//createSurfaceList3(trm1);
			createContourList(trm1);
			createUnityCubeList();
			///////////////////////////draw window////////////////////////////////////
			//InvalidateRect(NULL,FALSE);

			free_matrix(face,1,nf,1,3);
			free_matrix(point,1,np,1,4);
			free_vector(facecolor,1,nf);
			free_vector(pv,1,np);

			OnViewReset();

		}

	}


	//void Co3View::createSceneList(float ***dt, float *colorv, long r, long c, long h)
	//{//创建显示列表

	//	////////////////////////////////////////////////////////////////////
	//	//m_DPList = glGenLists(1);
	//	//glNewList(m_DPList, GL_COMPILE);

	//	//long i,j,k;

	//	//float v1[3];
	//	//float v2[3];
	//	//float nor[3];
	//	//float innerp[3]={0,0,0};

	//	//for(i=1;i<=r;i++){
	//	//	glEnable(GL_NORMALIZE);
	//	//	glColor3f(0.0f, 0.0f, 1.0f); 
	//	//	glBegin(GL_TRIANGLES);


	//	//	for(k=1;k<=h;k++){
	//	//		v1[k-1]=dt[i][1][k]-dt[i][3][k];
	//	//		v2[k-1]=dt[i][2][k]-dt[i][3][k];
	//	//	}
	//	//	nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
	//	//	nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
	//	//	nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


	//	//	for(j=1;j<=c;j++){
	//	//		selectNormal(nor,&dt[i][j][1],innerp,true);
	//	//		glNormal3fv(nor);
	//	//		glVertex3f(dt[i][j][1],dt[i][j][2],dt[i][j][3]);
	//	//	}
	//	//	glEnd();
	//	//	glDisable(GL_NORMALIZE);

	//	//	glLineWidth(2);
	//	//	glColor3f(1.0f, 0.0f, 0.0f); 
	//	//	glBegin (GL_LINE_LOOP);
	//	//	for(j=1;j<=c;j++){
	//	//		glVertex3f(dt[i][j][1],dt[i][j][2],dt[i][j][3]);
	//	//	}
	//	//	glEnd();
	//	//	glLineWidth(1);
	//	//}

	//	//glEndList();

	//	/////////////////////////////////////////////////////////////


	//	//m_surfaceList = glGenLists(1);
	//	//glNewList(m_surfaceList, GL_COMPILE);

	//	//long i,j,k;

	//	//float v1[3];
	//	//float v2[3];
	//	//float nor[3];
	//	//float innerp[3]={0,0,0};

	//	//float rgba[4];

	//	//float mxcv=findmax(colorv,r);
	//	//float mncv=findmin(colorv,r);

	//	//for(i=1;i<=r;i++){
	//	//	glEnable(GL_NORMALIZE);
	//	//	genColor(rgba,(colorv[i]-mncv)/(mxcv-mncv)*0.8);

	//	//	glColor4fv(rgba);

	//	//	glBegin(GL_TRIANGLES);


	//	//	for(k=1;k<=h;k++){
	//	//		v1[k-1]=dt[i][1][k]-dt[i][3][k];
	//	//		v2[k-1]=dt[i][2][k]-dt[i][3][k];
	//	//	}
	//	//	nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
	//	//	nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
	//	//	nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


	//	//	for(j=1;j<=c;j++){
	//	//		selectNormal(nor,&dt[i][j][1],innerp,true);
	//	//		glNormal3fv(nor);
	//	//		glVertex3f(dt[i][j][1],dt[i][j][2],dt[i][j][3]);
	//	//	}
	//	//	glEnd();


	//	//	glDisable(GL_NORMALIZE);


	//	//}
	//	//glEndList();

	//	//
	//	//GLfloat linewidth=2;

	//	//m_meshList = glGenLists(1);
	//	//glNewList(m_meshList, GL_COMPILE);
	//	//glLineWidth(linewidth);
	//	//for(i=1;i<=r;i++){

	//	//	glColor3f(1.0f, 1.0f, 1.0f); 
	//	//	glBegin (GL_LINE_LOOP);
	//	//	for(j=1;j<=c;j++){
	//	//		glVertex3f(dt[i][j][1],dt[i][j][2],dt[i][j][3]);
	//	//	}
	//	//	glEnd();
	//	//	//glLineWidth(1);
	//	//}
	//	//glEndList();



	//	///////////////////////////////////////////////////////////////////////////////

	//	m_surfaceList = glGenLists(1);
	//	glNewList(m_surfaceList, GL_COMPILE);

	//	long i,j,k;

	//	float v1[3];
	//	float v2[3];
	//	float nor[3];
	//	float innerp[3]={0,0,0};

	//	float rgba[4];

	//	float mxcv=findmax(colorv,r);
	//	float mncv=findmin(colorv,r);
	//	//float mxcv=0.5;
	//	//float mncv=0.2;

	//	for(i=1;i<=r;i++){
	//		glEnable(GL_NORMALIZE);

	//		genColor(rgba,(colorv[i]-mncv)/(mxcv-mncv)*0.8);
	//		glColor4fv(rgba);

	//		glBegin(GL_TRIANGLES);


	//		for(k=1;k<=3;k++){
	//			v1[k-1]=dt[i][1][k]-dt[i][3][k];
	//			v2[k-1]=dt[i][2][k]-dt[i][3][k];
	//		}
	//		nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
	//		nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
	//		nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


	//		for(j=1;j<=c;j++){
	//			//genColor(rgba,(dt[i][j][4]-mncv)/(mxcv-mncv)*0.8);
	//			//glColor4fv(rgba);

	//			selectNormal(nor,&dt[i][j][1],innerp,true);
	//			glNormal3fv(nor);
	//			glVertex3f(dt[i][j][1],dt[i][j][2],dt[i][j][3]);
	//		}
	//		glEnd();


	//		glDisable(GL_NORMALIZE);


	//	}
	//	glEndList();


	//	GLfloat linewidth=2;

	//	m_meshList = glGenLists(1);
	//	glNewList(m_meshList, GL_COMPILE);
	//	glLineWidth(linewidth);
	//	for(i=1;i<=r;i++){

	//		glColor3f(1.0f, 1.0f, 1.0f); 
	//		glBegin (GL_LINE_LOOP);
	//		for(j=1;j<=c;j++){
	//			glVertex3f(dt[i][j][1],dt[i][j][2],dt[i][j][3]);
	//		}
	//		glEnd();
	//		//glLineWidth(1);
	//	}
	//	glEndList();


	//	//////////////////////////////////////////////////////////////////////////






	//}


	void Co3View::OnViewOrtho()
	{
		// TODO: Add your command handler code here
		isOrtho=!isOrtho;
		CRect plotrect;
		this->GetWindowRect(plotrect);
		plotrect.DeflateRect(2,2,2,2);
		reshape(plotrect.Width(),plotrect.Height());
		InvalidateRect(NULL,FALSE);
	}


	void Co3View::OnUpdateViewOrtho(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here

		if(isOrtho){
			pCmdUI->SetText(L"Orthographic");
		}
		else{
			pCmdUI->SetText(L"Perspective");
		}

	}


	void Co3View::setupLighting(void)
	{
		//Material Properties
		GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat matShininess[] = { 50.0f};
		GLfloat matAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f};
		GLfloat matDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f};
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
		//Lighting Parameters
		//Enable Lighting
		glEnable(GL_LIGHTING);
		//Specify a single directional light
		GLfloat amb[] = { 0.0f,0.0f,0.0f};
		GLfloat dif[] = { 1.0f,1.0f,1.0f};
		GLfloat spe[] = { 1.0f,1.0f,1.0f};
		GLfloat pos[] = { 3.0f,0.0f,3.0f,1.0};

		lighting(GL_LIGHT0,amb,dif,spe,pos);
		//dif[0]=0;
		//dif[2]=1;
		pos[0]=-3;
		lighting(GL_LIGHT1,amb,dif,spe,pos);

		glEnable(GL_NORMALIZE);
	}






	void Co3View::OnViewMesh()
	{
		// TODO: Add your command handler code here
		checkMesh=!checkMesh;
		InvalidateRect(NULL,FALSE);
	}


	void Co3View::OnUpdateViewMesh(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(checkMesh);

	}


	void Co3View::OnViewSurface()
	{
		// TODO: Add your command handler code here
		checkSurface=!checkSurface;
		InvalidateRect(NULL,FALSE);
	}


	void Co3View::OnUpdateViewSurface(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(checkSurface);

	}


	void Co3View::createMeshList(long ** face, long nf, float ** point, long np)
	{
		GLfloat linewidth=2;
		long i,j;
		m_meshList = glGenLists(1);
		glNewList(m_meshList, GL_COMPILE);

		glLineWidth(linewidth);
		glColor3f(1.0f, 1.0f, 1.0f); 

		for(i=1;i<=nf;i++){			
			glBegin (GL_LINE_LOOP);
			for(j=1;j<=3;j++){
				glVertex3fv(&point[face[i][j]][1]);
			}
			glEnd();
			//glLineWidth(1);
		}
		glEndList();
	}


	void Co3View::createMeshList(const triangleMesh& trm)
	{
		GLfloat linewidth=2;
		long i,j;
		m_meshList = glGenLists(1);
		glNewList(m_meshList, GL_COMPILE);

		glLineWidth(linewidth);
		glColor3f(1.0f, 1.0f, 1.0f); 


		for(i=1;i<=trm.nface;i++){			
			glBegin (GL_LINE_LOOP);
			for(j=1;j<=3;j++){

				glVertex3f(trm.point[trm.face[i][j]][1],trm.point[trm.face[i][j]][2],trm.point[trm.face[i][j]][3]);
			}
			glEnd();
			//glLineWidth(1);
		}
		glEndList();
	}


	//void Co3View::createSurfaceList(long ** face, float *facecolor, long nf, float ** point, long np){

	//	m_surfaceList = glGenLists(1);
	//	glNewList(m_surfaceList, GL_COMPILE);

	//	long i,j,k;

	//	float v1[3];
	//	float v2[3];
	//	float nor[3];
	//	float innerp[3]={0,0,0};

	//	float rgba[4];

	//	float mxcv=findmax(facecolor,nf);
	//	float mncv=findmin(facecolor,nf);
	//	//float mxcv=0.5;
	//	//float mncv=0.2;

	//	for(i=1;i<=nf;i++){
	//		glEnable(GL_NORMALIZE);

	//		genColor(rgba,(facecolor[i]-mncv)/(mxcv-mncv)*0.8);
	//		glColor4fv(rgba);

	//		glBegin(GL_TRIANGLES);


	//		for(k=1;k<=3;k++){
	//			v1[k-1]=point[face[i][1]][k]-point[face[i][3]][k];
	//			v2[k-1]=point[face[i][2]][k]-point[face[i][3]][k];
	//		}
	//		nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
	//		nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
	//		nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


	//		for(j=1;j<=3;j++){
	//			//genColor(rgba,(dt[i][j][4]-mncv)/(mxcv-mncv)*0.8);
	//			//glColor4fv(rgba);

	//			selectNormal(nor,&point[face[i][j]][1],innerp,true);
	//			glNormal3fv(nor);
	//			glVertex3fv(&point[face[i][j]][1]);
	//		}
	//		glEnd();


	//		glDisable(GL_NORMALIZE);


	//	}
	//	glEndList();
	//}


	void Co3View::createSurfaceList(const triangleMesh& trm)
	{

		m_surfaceList = glGenLists(1);
		glNewList(m_surfaceList, GL_COMPILE);

		long i,j,k;

		float v1[3];
		float v2[3];
		float nor[3];
		float innerp[3]={0,0,0};

		float rgba[4];

		float mxcv=trm.maxFaceValue;
		float mncv=trm.minFaceValue;


		for(i=1;i<=trm.nface;i++){
			glEnable(GL_NORMALIZE);

			genColor(rgba,(trm.faceValue[i]-mncv)/(mxcv-mncv)*0.8);
			glColor4fv(rgba);

			glBegin(GL_TRIANGLES);


			//for(k=1;k<=3;k++){
			//	v1[k-1]=trm.point[trm.face[i][1]][k]-trm.point[trm.face[i][3]][k];
			//	v2[k-1]=trm.point[trm.face[i][2]][k]-trm.point[trm.face[i][3]][k];
			//}
			//nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
			//nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
			//nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


			for(j=1;j<=3;j++){
				//genColor(rgba,(dt[i][j][4]-mncv)/(mxcv-mncv)*0.8);
				//glColor4fv(rgba);

				//selectNormal(nor,&trm.point[trm.face[i][j]][1],innerp,true);
				//glNormal3fv(nor);
				glVertex3f(trm.point[trm.face[i][j]][1],trm.point[trm.face[i][j]][2],trm.point[trm.face[i][j]][3]);

				//std::cout << trm.point[trm.face[i][j]][1] << trm.point[trm.face[i][j]][2] << trm.point[trm.face[i][j]][3] << std::endl;
			}
			glEnd();


			glDisable(GL_NORMALIZE);


		}
		glEndList();
	}


	//void Co3View::createSurfaceList2(long ** face, float *facecolor, long nf, float ** point, long np){

	//	m_surfaceList2 = glGenLists(1);
	//	glNewList(m_surfaceList2, GL_COMPILE);

	//	long i,j,k;

	//	float v1[3];
	//	float v2[3];
	//	float nor[3];
	//	float innerp[3]={0,0,0};

	//	float rgba[4];

	//	float mxcv=findmax(facecolor,nf);
	//	float mncv=findmin(facecolor,nf);
	//	//float mxcv=0.5;
	//	//float mncv=0.2;

	//	for(i=1;i<=nf;i++){
	//		glEnable(GL_NORMALIZE);

	//		//genColor(rgba,(facecolor[i]-mncv)/(mxcv-mncv)*0.8);
	//		//glColor4fv(rgba);

	//		glBegin(GL_TRIANGLES);


	//		for(k=1;k<=3;k++){
	//			v1[k-1]=point[face[i][1]][k]-point[face[i][3]][k];
	//			v2[k-1]=point[face[i][2]][k]-point[face[i][3]][k];
	//		}
	//		nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
	//		nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
	//		nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


	//		for(j=1;j<=3;j++){
	//			genColor(rgba,(point[face[i][j]][4]-mncv)/(mxcv-mncv)*0.8);
	//			glColor4fv(rgba);

	//			selectNormal(nor,&point[face[i][j]][1],innerp,true);
	//			glNormal3fv(nor);
	//			glVertex3fv(&point[face[i][j]][1]);
	//		}
	//		glEnd();


	//		glDisable(GL_NORMALIZE);


	//	}
	//	glEndList();
	//}

	void Co3View::createSurfaceList2(const triangleMesh& trm)
	{

		m_surfaceList2 = glGenLists(1);
		glNewList(m_surfaceList2, GL_COMPILE);

		long i,j,k;

		float v1[3];
		float v2[3];
		float nor[3];
		float innerp[3]={0,0,0};

		float rgba[4];

		float mxcv=trm.maxPointValue;
		float mncv=trm.minPointValue;

		//float mxcv=trm.maxPointValueInterp;
		//float mncv=trm.minPointValueInterp;

		//float mxcv=0.5;
		//float mncv=0.2;

		for(i=1;i<=trm.nface;i++){
			glEnable(GL_NORMALIZE);

			//genColor(rgba,(facecolor[i]-mncv)/(mxcv-mncv)*0.8);
			//glColor4fv(rgba);

			glBegin(GL_TRIANGLES);


			//for(k=1;k<=3;k++){
			//	v1[k-1]=trm.point[trm.face[i][1]][k]-trm.point[trm.face[i][3]][k];
			//	v2[k-1]=trm.point[trm.face[i][2]][k]-trm.point[trm.face[i][3]][k];
			//}
			//nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
			//nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
			//nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


			for(j=1;j<=3;j++){
				genColor(rgba,(trm.pointValue[trm.face[i][j]]-mncv)/(mxcv-mncv)*0.8);
				//genColor(rgba,(trm.pointValueInterp[trm.face[i][j]]-mncv)/(mxcv-mncv)*0.8);
				glColor4fv(rgba);

				//selectNormal(nor,&trm.point[trm.face[i][j]][1],innerp,true);
				//glNormal3fv(nor);
				glVertex3fv(&trm.point[trm.face[i][j]][1]);
			}
			glEnd();


			glDisable(GL_NORMALIZE);


		}
		glEndList();
	}


	void Co3View::OnViewSmoothsurface()
	{
		// TODO: Add your command handler code here
		checkSmoothSurface=!checkSmoothSurface;
		InvalidateRect(NULL,FALSE);
	}


	void Co3View::OnUpdateViewSmoothsurface(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(checkSmoothSurface);
	}


	void Co3View::createUnityCubeList(void)
	{
		m_unityCube = glGenLists(1);
		glNewList(m_unityCube, GL_COMPILE);

		glBegin(GL_LINES);
		glLineWidth(1);
		//x axis
		glColor3f(1,0,0);
		glVertex3f(-1,-1,-1);
		glVertex3f(1,-1,-1);

		//glVertex3f(-1,-1,1);
		//glVertex3f(1,-1,1);

		//glVertex3f(-1,1,-1);
		//glVertex3f(1,1,-1);

		//glVertex3f(-1,1,1);
		//glVertex3f(1,1,1);

		//y axis
		glColor3f(0,1,0);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1,1,-1);

		//glVertex3f(-1,-1,1);
		//glVertex3f(-1,1,1);

		//glVertex3f(1,-1,-1);
		//glVertex3f(1,1,-1);

		//glVertex3f(1,-1,1);
		//glVertex3f(1,1,1);

		//z axis
		glColor3f(0,0,1);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1,-1,1);

		//glVertex3f(1,-1,-1);
		//glVertex3f(1,-1,1);

		//glVertex3f(-1,1,-1);
		//glVertex3f(-1,1,1);

		//glVertex3f(1,1,-1);
		//glVertex3f(1,1,1);
		glEnd();



		glEndList();
	}


	long **Co3View::genFrontFaceList(long ** face, long nf, float ** point, long np, long * nff)
	{
		bool *idx=vector<bool>(1,nf);

		long i,j;
		float zo=0.5;
		j=0;
		for(i=1;i<=nf;i++){
			if( (point[face[i][1]][3]==zo)&&(point[face[i][2]][3]==zo)&&(point[face[i][3]][3]==zo) ){
				idx[i]=true;
				j++;
			}
			else{
				idx[i]=false;
			}

		}

		long **frontFace=matrix<long>(1,j,1,3);
		j=0;
		for(i=1;i<=nf;i++){
			if(idx[i]){
				j++;
				copyvt(&face[i][1],3,&frontFace[j][1]);
			}
		}

		free_vector(idx,1,nf);

		return frontFace;
	}


	void Co3View::OnViewContour()
	{
		// TODO: Add your command handler code here

		checkContour=!checkContour;
		InvalidateRect(NULL,FALSE);
	}


	void Co3View::OnUpdateViewContour(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here

		pCmdUI->SetCheck(checkContour);
	}


	void Co3View::createContourList(const triangleMesh& trm)
	{
		GLfloat linewidth=1;
		long i,j;
		float rgba[4]={1,1,1,1};
		m_contourList = glGenLists(1);
		glNewList(m_contourList, GL_COMPILE);

		glLineWidth(linewidth);

		for(i=0;i<trm.contourv.size();i++){

			genColor(rgba,(trm.contourValue[i]-trm.minPointValue)/(trm.maxPointValue-trm.minPointValue)*0.8);
			glColor4fv(rgba);

			glBegin (GL_LINE_STRIP);
			for(j=0;j<trm.contourv[i][0].size();j++){
				glVertex3f(trm.contourv[i][0][j],trm.contourv[i][1][j],trm.contourv[i][2][j]);
			}
			glEnd();
		}
		glEndList();

	}



		void Co3View::createSurfaceList3(const triangleMesh& trm)
	{

		m_surfaceList3 = glGenLists(1);
		glNewList(m_surfaceList3, GL_COMPILE);

		long i,j,k;

		float v1[3];
		float v2[3];
		float nor[3];
		float innerp[3]={0,0,0};

		float rgba[4];

		//float mxcv=trm.maxPointValue;
		//float mncv=trm.minPointValue;

		float mxcv=trm.maxPointValueInterp;
		float mncv=trm.minPointValueInterp;

		//float mxcv=0.5;
		//float mncv=0.2;

		for(i=1;i<=trm.nface;i++){
			glEnable(GL_NORMALIZE);

			//genColor(rgba,(facecolor[i]-mncv)/(mxcv-mncv)*0.8);
			//glColor4fv(rgba);

			glBegin(GL_TRIANGLES);


			//for(k=1;k<=3;k++){
			//	v1[k-1]=trm.point[trm.face[i][1]][k]-trm.point[trm.face[i][3]][k];
			//	v2[k-1]=trm.point[trm.face[i][2]][k]-trm.point[trm.face[i][3]][k];
			//}
			//nor[0]=v1[1]*v2[2]-v1[2]*v2[1];
			//nor[1]=v1[2]*v2[0]-v1[0]*v2[2];
			//nor[2]=v1[0]*v2[1]-v1[1]*v2[0];


			for(j=1;j<=3;j++){
				//genColor(rgba,(trm.pointValue[trm.face[i][j]]-mncv)/(mxcv-mncv)*0.8);
				genColor(rgba,(trm.pointValueInterp[trm.face[i][j]]-mncv)/(mxcv-mncv)*0.8);
				glColor4fv(rgba);

				//selectNormal(nor,&trm.point[trm.face[i][j]][1],innerp,true);
				//glNormal3fv(nor);
				glVertex3fv(&trm.point[trm.face[i][j]][1]);
			}
			glEnd();


			glDisable(GL_NORMALIZE);


		}
		glEndList();
		}

		void Co3View::OnViewPlane()
		{
			isplane=!isplane;
			// TODO: Add your command handler code here
		}


		void Co3View::OnUpdateViewPlane(CCmdUI *pCmdUI)
		{
			// TODO: Add your command update UI handler code here

					if(isplane==0){
			pCmdUI->SetText(L"plane");
		}
		else{
			pCmdUI->SetText(L"volume");
		}
		}
