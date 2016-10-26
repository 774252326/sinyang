
// z8View.cpp : implementation of the Cz8View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "z8.h"
#endif

#include "z8Doc.h"
#include "z8View.h"

#include "pcct.h"
#include <algorithm>
#include "../../funT/xRescaleT.h"
#include "../../funT/calgridT.h"
#include "colormap.h"

//color
#define black RGB(0,0,0)
#define red RGB(255,0,0)
#define green RGB(0,255,0)
#define blue RGB(0,0,255)
#define yellow RGB(255,255,0)
#define cyan RGB(0,255,255)
#define magenta RGB(255,0,255)
#define white RGB(255,255,255)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cz8View

IMPLEMENT_DYNCREATE(Cz8View, CFormView)

	BEGIN_MESSAGE_MAP(Cz8View, CFormView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_COMMAND(ID_FILE_OPEN, &Cz8View::OnFileOpen)
		ON_WM_PAINT()
		ON_WM_TIMER()
		ON_WM_ERASEBKGND()
		ON_WM_MOUSEMOVE()
		ON_WM_SETCURSOR()
		ON_WM_KEYDOWN()
		ON_COMMAND(ID_PLAY, &Cz8View::OnPlay)
		ON_UPDATE_COMMAND_UI(ID_PLAY, &Cz8View::OnUpdatePlay)
	END_MESSAGE_MAP()

	// Cz8View construction/destruction

	Cz8View::Cz8View()
		: CFormView(Cz8View::IDD)
		, m_filePath(_T(""))
		, isLoad(false)
		, xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
		, ci(0)
		, x(NULL)
		, y(NULL)
		, windowTitle(_T("z8"))
		, xmouse(0)
		, ymouse(0)
		, selectPointIndex(0)
		, pnt(0)
		, timer1(0)
	{
		// TODO: add construction code here

	}

	Cz8View::~Cz8View()
	{
	}

	void Cz8View::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
		DDX_Check(pDX, IDC_CHECK1, m_isplay);
	}

	BOOL Cz8View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CFormView::PreCreateWindow(cs);
	}

	void Cz8View::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();
	}

	void Cz8View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void Cz8View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Cz8View diagnostics

#ifdef _DEBUG
	void Cz8View::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void Cz8View::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	Cz8Doc* Cz8View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cz8Doc)));
		return (Cz8Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Cz8View message handlers


	void Cz8View::OnFileOpen()
	{
		// TODO: Add your command handler code here

		CFileDialog fileDlg(true);

		fileDlg.m_ofn.lpstrFilter=L"Text File(*.txt)\0*.txt\0\0";

		if( fileDlg.DoModal()==IDOK){

			m_filePath=fileDlg.GetPathName();



			//pcct dt1;
			dt1.clear();
			dt1.readFile(m_filePath);



			currentColor=genColor(genColorvFromIndex(dtlist.size()));
			////////////////////////////////////////////////////////////////////////////////////////



			////isLoad=true;
			////CString str;
			////this->GetParent()->GetWindowTextW(str);
			////str=dt1.FilePath+L" - "+windowTitle;
			////this->GetParent()->SetWindowTextW(str);
			/////////////////////////////////////////////////////////////////////////////
			//UpdateData();

			if(!m_isplay){
				dt1.seperate();
				dt1.AR=dt1.intg(0.8);
				cllist.push_back(currentColor);
				dtlist.push_back(dt1);
				updatePlotRange(dt1.potential,dt1.current);
				////::AfxMessageBox(dt1.segmentinfo);
				Invalidate();
			}
			////////////////////////////////////////////////////////////////////////////
			else{
				dt2.clear();
				dt2.segmentList=dt1.segmentList;
				dt2.segmentinfo=dt1.segmentinfo;
				dt2.label=dt1.label;
				dt2.FilePath=dt1.FilePath;
				dt2.FileName=dt1.FileName;
				dt2.seginfo=dt1.seginfo;

				////////////////////////////////////////////////timer////////////////////////////////////////////////

				timer1=SetTimer(1,10,NULL); 
				ci=0;
				//SetTimer(2,20,NULL);
			}

		}


	}


	void Cz8View::OnDraw(CDC* pDC)
	{
		// TODO: Add your specialized code here and/or call the base class

		//CPaintDC dc(this); // device context for painting
		//CPen pen;
		//pen.CreatePen(PS_SOLID,1,RGB(0,0,255));

		//long i;
		//double *x;
		//double *y;
		//dc.SelectObject(pen);
		//if(isLoad){
		//	x=dt1.potential.data();
		//	y=dt1.current.data();
		//	dc.MoveTo(100*x[0],100*y[0]);
		//	for(i=0;i<dt1.potential.size();i++){
		//		//dc.SetPixel(100*x[i],100*y[i],RGB(0,0,255));

		//		dc.LineTo(100*x[i],100*y[i]);
		//	}
		//}
		//pen.DeleteObject();

		////////////////////////////////////////////////////////////

		//		CRect rect;
		//		this->GetWindowRect(&rect);
		//		int nWidth=rect.Width();
		//		int nHeight=rect.Height();
		//		
		//		CDC MemDC; //首先定义一个显示设备对象
		//CBitmap MemBitmap;//定义一个位图对象
		//
		////随后建立与屏幕显示兼容的内存显示设备
		//MemDC.CreateCompatibleDC(NULL);
		////这时还不能绘图，因为没有地方画 ^_^
		////下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
		//
		//MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);
		////将位图选入到内存显示设备中
		////只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		//CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
		//
		////先用背景色将位图清除干净，这里我用的是白色作为背景
		////你也可以用自己应该用的颜色
		//MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));
		//
		////绘图
		//MemDC.MoveTo(100,100);
		//MemDC.LineTo(100,200);
		//
		////将内存中的图拷贝到屏幕上进行显示
		//pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//
		////绘图完成后的清理
		//MemBitmap.DeleteObject();
		//MemDC.DeleteDC();

	}


	void Cz8View::OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CFormView::OnPaint() for painting messages

		////////////////////////////////////////////////////////////////////////

		//CRect plotrect;
		//CSize sz;
		//this->GetWindowRect(&plotrect);

		////GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
		//ScreenToClient(&plotrect);
		//plotrect.DeflateRect(60,60,200,60);

		//CPen pen;
		//pen.CreatePen(PS_SOLID,1,blue);

		//long i,j;
		////double *x;
		////double *y;
		//dc.SelectObject(pen);



		////if(isLoad){
		//if(!dtlist.empty()){
		//	if(!plotrect.IsRectEmpty()){
		//		std::vector<CPoint> pointlist;
		//		for(j=0;j<dtlist.size();j++){
		//			pointlist=genPointToPlot(dtlist[j].potential,dtlist[j].current,plotrect);

		//			//for(i=0;i<pointlist.size();i++){
		//				for(i=(0>ci-200)?0:ci-200;i<ci;i++){
		//				dc.SetPixel(pointlist[i],cllist[j]);					
		//			}
		//			//dc.Polyline(pointlist.data(),pointlist.size());
		//		}
		//		DrawXYAxis(plotrect,&dc);

		//		for(j=0;j<dt1.seginfo.size();j++){
		//		//str=dt1.label[0];
		//		//pdc->SelectObject(&font);
		//			sz=dc.GetTextExtent(dt1.seginfo[j]);
		//			dc.TextOutW(plotrect.right,plotrect.top+sz.cy*j,dt1.seginfo[j]);
		//		//font.DeleteObject();
		//		}


		//	}
		//}
		//pen.DeleteObject();




		//////////////////////////////////////////////////////////////////



		//		CClientDC pDC(this);
		//
		//
		//				CRect rect;
		//		this->GetWindowRect(&rect);
		//		int nWidth=rect.Width();
		//		int nHeight=rect.Height();
		//		
		//		CDC MemDC; //首先定义一个显示设备对象
		//CBitmap MemBitmap;//定义一个位图对象
		//
		////随后建立与屏幕显示兼容的内存显示设备
		//MemDC.CreateCompatibleDC(NULL);
		////这时还不能绘图，因为没有地方画 ^_^
		////下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
		//
		//MemBitmap.CreateCompatibleBitmap(&pDC,nWidth,nHeight);
		////将位图选入到内存显示设备中
		////只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		//CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
		//
		////先用背景色将位图清除干净，这里我用的是白色作为背景
		////你也可以用自己应该用的颜色
		//MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));
		//
		////绘图
		//MemDC.MoveTo(100,100);
		//MemDC.LineTo(100,200);
		//
		////将内存中的图拷贝到屏幕上进行显示
		//pDC.BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		////pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		////绘图完成后的清理
		//MemBitmap.DeleteObject();
		//MemDC.DeleteDC();

		/////////////////////////////////////////////////////////////////////////////



		CClientDC pDC(this);


		CRect winrect;
		this->GetWindowRect(&winrect);
		int nWidth=winrect.Width();
		int nHeight=winrect.Height();

		CDC MemDC; //首先定义一个显示设备对象
		CBitmap MemBitmap;//定义一个位图对象

		//随后建立与屏幕显示兼容的内存显示设备
		MemDC.CreateCompatibleDC(NULL);
		//这时还不能绘图，因为没有地方画 ^_^
		//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小

		MemBitmap.CreateCompatibleBitmap(&pDC,nWidth,nHeight);
		//将位图选入到内存显示设备中
		//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

		//先用背景色将位图清除干净，这里我用的是白色作为背景
		//你也可以用自己应该用的颜色
		MemDC.FillSolidRect(0,0,nWidth,nHeight,white);

		//绘图
		//MemDC.MoveTo(100,100);
		//MemDC.LineTo(100,200);

		//CRect plotrect;
		CSize sz;
		this->GetWindowRect(&plotrect);
		COLORREF oc;
		//GetDlgItem(IDC_PLOT)->GetWindowRect(&plotrect);
		ScreenToClient(&plotrect);
		plotrect.DeflateRect(60,60,310,60);

		CPen pen;

		CString str;
		long i,j;
		long ll=1000;
		//double *x;
		//double *y;

		//CBrush brush(cyan);
		//CBrush *pOldBrush;


		//if(isLoad){
		if( !dtlist.empty() || !dt2.potential.empty() ){
			if(!plotrect.IsRectEmpty()){
				std::vector<CPoint> pointlist;

				DrawXYAxis(plotrect,&MemDC);

				drawRectangle(plotrect,&MemDC);



				//str.Format(L"x=%f,y=%f",xmouse,ymouse);
				str.Format(L"Potential=%gV,Current=%gA,t=%gs",xmouse,ymouse,(double)ci/100);
				MemDC.TextOutW(0,0,str);

				for(j=0;j<dtlist.size();j++){
					pointlist=genPointToPlot(dtlist[j].potential,dtlist[j].current,plotrect);

					for(i=0;i<pointlist.size();i++){
						//for(i=(0>ci-ll)?0:ci-ll;i<ci;i++){
						MemDC.SetPixel(pointlist[i],cllist[j]);					
					}

					//pen.CreatePen(PS_SOLID,1,cllist[j]);
					//MemDC.SelectObject(pen);
					//MemDC.Polyline(pointlist.data(),pointlist.size());
					//pen.DeleteObject();

					oc=MemDC.SetTextColor(cllist[j]);
					sz=dc.GetTextExtent(dtlist[j].FileName);
					MemDC.TextOutW(plotrect.right,plotrect.top+sz.cy*j,dtlist[j].FileName);
					MemDC.SetTextColor(oc);

				}

				pointlist=genPointToPlot(dt2.potential,dt2.current,plotrect);

				for(i=0;i<pointlist.size();i++){
					//for(i=(0>ci-ll)?0:ci-ll;i<ci;i++){
					MemDC.SetPixel(pointlist[i],currentColor);					
				}



				MemDC.SetPixel(pnt,white);
				for(int e=1;e<=10;e++){
					MemDC.SetPixel(pnt.x-e,pnt.y,white);
					MemDC.SetPixel(pnt.x+e,pnt.y,white);
					MemDC.SetPixel(pnt.x,pnt.y-e,white);
					MemDC.SetPixel(pnt.x,pnt.y+e,white);
				}


				//for(j=0;j<dt1.seginfo.size();j++){
				//	//str=dt1.label[0];
				//	//pdc->SelectObject(&font);
				//	sz=dc.GetTextExtent(dt1.seginfo[j]);
				//	MemDC.TextOutW(plotrect.right,plotrect.top+sz.cy*j,dt1.seginfo[j]);
				//	//font.DeleteObject();
				//}


			}
		}



		//将内存中的图拷贝到屏幕上进行显示
		pDC.BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//绘图完成后的清理
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();



	}


	//CRect Cz8View::DrawXYAxis(CRect rect, CPaintDC* pdc)
	CRect Cz8View::DrawXYAxis(CRect rect, CDC* pdc)
	{
		//if(rect.IsRectEmpty()){

		//	return false;
		//}
		//else{

		CPoint ptOffset1(rect.left,rect.top);//y
		CPoint ptOffset2(rect.left,rect.bottom);//origin
		CPoint ptOffset3(rect.right,rect.top);
		CPoint ptOffset4(rect.right,rect.bottom);//x
		CFont font;
		CFont *pOldFont;
		CString str;
		CPen * pOldPen;
		CSize sz;
		CRect newrect=rect;
		CPoint textLocate;
		CPen pen;
		int metricH=15;
		int labelH=20;
		int lc=5;
		int lcs=lc-2;
		double gridi,XMAX,XMIN,YMAX,YMIN;
		int tmp;
		COLORREF oc;
		CRect xmrect(rect.left,rect.bottom+lc,rect.left,rect.bottom+lc);
		CRect ymrect(rect.left-lc,rect.bottom,rect.left-lc,rect.bottom);
		XMAX=xmax;
		XMIN=xmin;
		YMAX=ymax;
		YMIN=ymin;

		CString fontName=L"Arial";



		////////////////////////////////////////////////////////////////////


		//draw x axis

		pen.CreatePen(PS_SOLID, 1, red);
		pOldPen=pdc->SelectObject(&pen);
		pdc->MoveTo(ptOffset2);
		pdc->LineTo(ptOffset4);

		//draw x metric


		font.CreateFont(
			metricH,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename

		pOldFont=pdc->SelectObject(&font);

		//double resox=pow(10.0,calgrid(XMAX-XMIN));

		double resox=calreso(XMAX-XMIN);

		for(gridi=resox*ceil(XMIN/resox);gridi<=XMAX;gridi+=resox){

			tmp=xRescale(gridi,XMIN,XMAX,rect.left,rect.right);
			pdc->MoveTo(tmp,rect.bottom);
			pdc->LineTo(tmp,rect.bottom+lcs);

			str.Format(L"%.1e",gridi);
			sz=pdc->GetTextExtent(str);
			textLocate.x=tmp-sz.cx/2;
			textLocate.y=rect.bottom+lc;
			if(xmrect.right<textLocate.x){
				if(textLocate.x+sz.cx<rect.right){
					pdc->TextOutW(textLocate.x,textLocate.y,str);
					xmrect.right=textLocate.x+sz.cx;
					xmrect.bottom=textLocate.y+sz.cy;

					pdc->MoveTo(tmp,rect.bottom);
					pdc->LineTo(tmp,rect.bottom+lc);
				}
				TRACE("%d,",sz.cy);
			}
		}
		newrect.bottom+=lc+sz.cy;
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();


		//draw x axis label
		font.CreateFont(
			labelH,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename
		oc=pdc->SetTextColor(green);
		//font.CreatePointFont(200,L"MS Gothic",NULL);
		//str.Format(L"time(s)");
		str=dt1.label[0];
		pOldFont=pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		//pdc->SetTextAlign(TA_UPDATECP);
		pdc->TextOutW(rect.CenterPoint().x-(sz.cx/2),newrect.bottom,str);
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SetTextColor(oc);
		TRACE("%d,",sz.cy);
		newrect.bottom+=sz.cy;

		///////////////////////////////////////////////////////




		//draw y axis

		pen.CreatePen(PS_SOLID, 1, red);
		pOldPen=pdc->SelectObject(&pen);
		pdc->MoveTo(ptOffset2);
		pdc->LineTo(ptOffset1);



		font.CreateFont(
			metricH,                        // nHeight
			0,                         // nWidth
			900,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename


		//draw y metric
		double resoy=calreso(YMAX-YMIN);
		//double resoy=pow(10.0,calgrid(YMAX-YMIN));
		pOldFont=pdc->SelectObject(&font);
		for(gridi=resoy*ceil(YMIN/resoy);gridi<=YMAX;gridi+=resoy){
			tmp=xRescale(gridi,YMIN,YMAX,rect.bottom,rect.top);
			pdc->MoveTo(rect.left,tmp);
			pdc->LineTo(rect.left-lcs,tmp);
			str.Format(L"%.1e",gridi);
			sz=pdc->GetTextExtent(str);
			//pdc->TextOutW(rect.left-lc-sz.cy,tmp+sz.cx/2,str);
			//TRACE("%d,",sz.cy);


			textLocate.x=rect.left-lc-sz.cy;
			textLocate.y=tmp+sz.cx/2;
			if(ymrect.top>textLocate.y){
				if(textLocate.y-sz.cx>rect.top){
					pdc->TextOutW(textLocate.x,textLocate.y,str);
					ymrect.left=textLocate.x;
					ymrect.top=textLocate.y-sz.cx;

					pdc->MoveTo(rect.left,tmp);
					pdc->LineTo(rect.left-lc,tmp);
				}
				TRACE("%d,",sz.cy);
			}


		}
		newrect.left-=lc+sz.cy;
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		pdc->SelectObject(pOldPen);
		pen.DeleteObject();



		//draw y axis label
		font.CreateFont(
			labelH,                        // nHeight
			0,                         // nWidth
			900,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			fontName);                 // lpszFacename

		oc=pdc->SetTextColor(green);

		//str.Format(L"current(A)");
		str=dt1.label[1];
		pOldFont=pdc->SelectObject(&font);
		sz=pdc->GetTextExtent(str);
		pdc->TextOutW(newrect.left-sz.cy,rect.CenterPoint().y+(sz.cx/2),str);
		pdc->SelectObject(pOldFont);
		font.DeleteObject();
		TRACE("%d,",sz.cy);
		pdc->SetTextColor(oc);
		newrect.left-=sz.cy;



		///////////////////////////////////////////////////////////////////////




		//return true;
		return newrect;
		//}
	}


	void Cz8View::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: Add your message handler code here and/or call default

		switch(nIDEvent){
		case 1:
			ci+=1;
			if(ci>=dt1.potential.size()){
				dt2.seperate();
				dt2.intg(0.8);
				cllist.push_back(currentColor);
				dtlist.push_back(dt2);
				dt1.clear();
				dt2.clear();

				KillTimer(timer1);
			}
			else{

				dt2.potential.push_back(dt1.potential[ci]);
				dt2.current.push_back(dt1.current[ci]);
				dt2.charge.push_back(dt1.charge[ci]);
				dt2.time.push_back(dt1.time[ci]);
				updatePlotRange(dt2.potential,dt2.current);


				//InvalidateRect(NULL,0);
			}
			Invalidate();
			break;
		case 2:
			ci+=80;
			if(ci>dt1.potential.size()){
				ci-=dt1.potential.size();
			}
			Invalidate();
			break;

			//default:
			//MessageBeep(0xFFFFFFFF);  
			//break;

		};



		CFormView::OnTimer(nIDEvent);
	}


	BOOL Cz8View::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: Add your message handler code here and/or call default

		return true;

		return CFormView::OnEraseBkgnd(pDC);
	}


	// update xmin,xmax,ymin,ymax
	void Cz8View::updatePlotRange(std::vector<double> x, std::vector<double> y)
	{

		double iv,pct=0.02;
		double tmin,tmax;
		auto resultx=std::minmax_element(x.begin(),x.end());
		tmin=*resultx.first;
		tmax=*resultx.second;
		iv=tmax-tmin;
		tmin-=iv*pct;
		tmax+=iv*pct;

		if(xmin>tmin)
			xmin=tmin;
		if(xmax<tmax)
			xmax=tmax;


		auto resulty=std::minmax_element(y.begin(),y.end());
		tmin=*resulty.first;
		tmax=*resulty.second;

		iv=tmax-tmin;
		tmin-=iv*pct;
		tmax+=iv*pct;


		if(ymin>tmin)
			ymin=tmin;
		if(ymax<tmax)
			ymax=tmax;
	}


	std::vector<CPoint> Cz8View::genPointToPlot(const std::vector<double> &px, const std::vector<double> &py, CRect rect)
	{

		std::vector<CPoint> pointlist;
		std::vector<long> pointx(px.size());
		std::vector<long> pointy(py.size());;
		long *p;
		p=pointx.data();
		xRescaleVT(px.data(),px.size(),xmin,xmax,p,rect.left,rect.right);
		p=pointy.data();
		xRescaleVT(py.data(),py.size(),ymin,ymax,p,rect.bottom,rect.top);

		long nd=( (px.size()>py.size())? py.size():px.size() );

		for(long i=0;i<nd;i++){
			pointlist.push_back(CPoint(pointx[i],pointy[i]));
		}

		return pointlist;
	}


	void Cz8View::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		////CRect plotrect;
		//this->GetWindowRect(&plotrect);
		//ScreenToClient(&plotrect);
		//plotrect.DeflateRect(60,60,310,60);

		//xmouse=xRescale(point.x,plotrect.left,plotrect.right,xmin,xmax);
		//ymouse=xRescale(point.y,plotrect.top,plotrect.bottom,ymin,ymax);

		if(	!dtlist.empty() && plotrect.PtInRect(point) ){
			pnt=findNearestPoint(point);
			xmouse=dtlist.back().potential[selectPointIndex];
			ymouse=dtlist.back().current[selectPointIndex];
			Invalidate();
		}



		CFormView::OnMouseMove(nFlags, point);
	}


	void Cz8View::drawRectangle(CRect rect, CDC * pDC)
	{
		// create and select a solid blue brush

		CBrush brushBlue(black);
		CBrush* pOldBrush = pDC->SelectObject(&brushBlue);

		// create and select a thick, black pen

		CPen penBlack;
		penBlack.CreatePen(PS_SOLID, 0, red);
		CPen* pOldPen = pDC->SelectObject(&penBlack);

		// get our client rectangle

		//CRect rect;
		//GetClientRect(rect);

		// shrink our rect 20 pixels in each direction

		//rect.DeflateRect(20, 20);

		// draw a thick black rectangle filled with blue

		pDC->Rectangle(rect);

		// put back the old objects

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

	}


	BOOL Cz8View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	{
		// TODO: Add your message handler code here and/or call default

		//CPoint pt;

		//if(::GetCursorPos(&pt)){
		//	ScreenToClient(&pt);
		//	if(plotrect.PtInRect(pt)){
		//		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		//		return true;
		//	}
		//}

		return CFormView::OnSetCursor(pWnd, nHitTest, message);
	}


	CPoint Cz8View::findNearestPoint(CPoint pt)
	{
		long i;
		double ptx,pty;
		ptx=xRescale(pt.x,plotrect.left,plotrect.right,xmin,xmax);
		pty=xRescale(pt.y,plotrect.bottom,plotrect.top,ymin,ymax);

		double biy=(double)plotrect.Height()/(ymax-ymin);
		double bix=(double)plotrect.Width()/(xmax-xmin);
		double dst,temp;
		dst=fabs(ptx-dtlist.back().potential[0])*bix+fabs(pty-dtlist.back().current[0])*biy;
		selectPointIndex=0;
		for(i=1;i<dtlist.back().potential.size();i++){
			temp=fabs(ptx-dtlist.back().potential[i])*bix+fabs(pty-dtlist.back().current[i])*biy;
			if(temp<dst){
				dst=temp;
				selectPointIndex=i;
			}
		}

		return CPoint(xRescale(dtlist.back().potential[selectPointIndex],xmin,xmax,plotrect.left,plotrect.right),
			xRescale(dtlist.back().current[selectPointIndex],ymin,ymax,plotrect.bottom,plotrect.top));
	}


	void Cz8View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: Add your message handler code here and/or call default
		if(!dtlist.empty())
			switch(nChar){

			case VK_LEFT:
				if(selectPointIndex>0){
					selectPointIndex--;			
					pnt=CPoint(xRescale(dtlist.back().potential[selectPointIndex],xmin,xmax,plotrect.left,plotrect.right),
						xRescale(dtlist.back().current[selectPointIndex],ymin,ymax,plotrect.bottom,plotrect.top));
					xmouse=dtlist.back().potential[selectPointIndex];
					ymouse=dtlist.back().current[selectPointIndex];
					Invalidate();
				}
				break;
			case VK_RIGHT:
				if(selectPointIndex<dtlist.back().potential.size()-1){
					selectPointIndex++;
					pnt=CPoint(xRescale(dtlist.back().potential[selectPointIndex],xmin,xmax,plotrect.left,plotrect.right),
						xRescale(dtlist.back().current[selectPointIndex],ymin,ymax,plotrect.bottom,plotrect.top));
					xmouse=dtlist.back().potential[selectPointIndex];
					ymouse=dtlist.back().current[selectPointIndex];
					Invalidate();
				}
				break;

			case VK_UP:
				if(selectPointIndex>10){
					selectPointIndex-=10;			
					pnt=CPoint(xRescale(dtlist.back().potential[selectPointIndex],xmin,xmax,plotrect.left,plotrect.right),
						xRescale(dtlist.back().current[selectPointIndex],ymin,ymax,plotrect.bottom,plotrect.top));
					xmouse=dtlist.back().potential[selectPointIndex];
					ymouse=dtlist.back().current[selectPointIndex];
					Invalidate();
				}
				break;
			case VK_DOWN:
				if(selectPointIndex<dtlist.back().potential.size()-10){
					selectPointIndex+=10;
					pnt=CPoint(xRescale(dtlist.back().potential[selectPointIndex],xmin,xmax,plotrect.left,plotrect.right),
						xRescale(dtlist.back().current[selectPointIndex],ymin,ymax,plotrect.bottom,plotrect.top));
					xmouse=dtlist.back().potential[selectPointIndex];
					ymouse=dtlist.back().current[selectPointIndex];
					Invalidate();
				}
				break;

			default:
				break;

		}


		CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	}


	void Cz8View::OnPlay()
	{
		// TODO: Add your command handler code here
		m_isplay=!m_isplay;
	}


	void Cz8View::OnUpdatePlay(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(m_isplay==1);
	}
