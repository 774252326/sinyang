
// analyzerView.cpp : implementation of the CanalyzerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

//#include "analyzerDoc.h"
#include "analyzerView.h"

//#include "MainFrm.h"
#include "func.h"
#include "PlotSettingPageA.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const double zoomrate=0.8;

// CanalyzerView

IMPLEMENT_DYNCREATE(CanalyzerView, CView)

	BEGIN_MESSAGE_MAP(CanalyzerView, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CanalyzerView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_MESSAGE(MESSAGE_CHANGE_APPLOOK, &CanalyzerView::OnMessageChangeApplook)
		ON_MESSAGE(MESSAGE_GET_PLOTSPEC, &CanalyzerView::OnMessageGetPlotspec)
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_MESSAGE(MESSAGE_UPDATE_VIEW, &CanalyzerView::OnMessageUpdateView)
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerView::OnOptionsPlotsettings)
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_COMMAND(ID_VIEW_DATACURSOR, &CanalyzerView::OnViewDatacursor)
		ON_UPDATE_COMMAND_UI(ID_VIEW_DATACURSOR, &CanalyzerView::OnUpdateViewDatacursor)
		ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerView::OnViewFitwindow)
	END_MESSAGE_MAP()

	// CanalyzerView construction/destruction

	CanalyzerView::CanalyzerView()
		: xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
		, pct(0.02)
		, spBtnSize(CSize(23*2,23))
		, m_mouseDownPoint(CPoint())
		, bMouseCursor(false)
	{
		// TODO: add construction code here

	}

	CanalyzerView::~CanalyzerView()
	{
	}

	BOOL CanalyzerView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CView::PreCreateWindow(cs);
	}

	// CanalyzerView drawing

	void CanalyzerView::OnDraw(CDC* pDC)
	{
		CanalyzerDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: add draw code for native data here

		if(pdl.empty())
			return;

		CDC dcMem;//用于缓冲作图的内存DC
		CBitmap bmp;//内存中承载临时图象的位图
		CRect rect;
		GetClientRect(&rect);
		dcMem.CreateCompatibleDC(pDC);//依附窗口DC创建兼容内存DC
		CSize winsz=rect.Size();
		bmp.CreateCompatibleBitmap(pDC,winsz.cx,winsz.cy);//创建兼容位图
		dcMem.SelectObject(&bmp);  	//将位图选择进内存DC

		DrawData(rect,&dcMem,pdl[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);

		//if(bMouseCursor && !pd->ps.empty()){
		//	//CString str;
		//	//str.Format(L"%g,%g",pDoc->lp[m_spBtn.GetPos32()].xll[selectIdx],pDoc->lp[m_spBtn.GetPos32()].yll[selectIdx]);
		//	//dcMem.TextOutW(m_mouseDownPoint.x,m_mouseDownPoint.y,str);

		//	DrawData1(rect
		//		,&dcMem
		//		,pd->xll[selectIdx]
		//		,pd->yll[selectIdx]
		//		,xmin
		//		,xmax
		//		,ymin
		//		,ymax
		//		,inv(pd->psp.bkgndC));
		//}

		pDC->BitBlt(0,0,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
		//pDC->BitBlt(100,100,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台

		dcMem.DeleteDC(); //删除DC
		bmp.DeleteObject(); //删除位图

		//////////////////////////////////////////////
		//CRect rect;
		//GetClientRect(&rect);
		//CRect plotrect=rect;
		//DrawData(plotrect,pDC,pDoc->lp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);







	}


	// CanalyzerView printing


	void CanalyzerView::OnFilePrintPreview()
	{
#ifndef SHARED_HANDLERS
		AFXPrintPreview(this);
#endif
	}

	BOOL CanalyzerView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// default preparation
		return DoPreparePrinting(pInfo);
	}

	void CanalyzerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add extra initialization before printing
	}

	void CanalyzerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: add cleanup after printing
	}

	void CanalyzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CanalyzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CanalyzerView diagnostics

#ifdef _DEBUG
	void CanalyzerView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CanalyzerView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CanalyzerDoc* CanalyzerView::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CanalyzerDoc)));
		return (CanalyzerDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CanalyzerView message handlers


	void CanalyzerView::OnInitialUpdate()
	{
		CView::OnInitialUpdate();

		// TODO: Add your specialized code here and/or call the base class

		//CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		//COutputList* ol=mf->GetOutputWnd()->GetListCtrl();

		//::SendMessage(ol->GetSafeHwnd(),MESSAGE_UPDATE_DOL,(WPARAM)this,NULL);
	}



	afx_msg LRESULT CanalyzerView::OnMessageChangeApplook(WPARAM wParam, LPARAM lParam)
	{

		//bkcr=(COLORREF)wParam;

		COLORREF oc=(COLORREF)wParam;


		for(size_t i=0;i<pdl.size();i++){
			pdl[i].psp.winbkC=oc;
			int ci=pdl[i].psp.GetCrType();
			pdl[i].psp.SetCr(ci);
		}

		//int i=0;
		//PlotData * pd=GetPD(i);	
		//while(pd!=NULL){
		//	pd->psp.winbkC=bkcr;
		//	int ci=pd->psp.GetCrType();
		//	//pd->psp=PlotSpec(ci,oc);
		//	pd->psp.SetCr(ci);
		//	i++;
		//	pd=GetPD(i);
		//}

		::SendMessage(this->GetSafeHwnd(),MESSAGE_GET_PLOTSPEC,wParam,NULL);

		this->Invalidate(FALSE);

		return 0;
	}


	afx_msg LRESULT CanalyzerView::OnMessageGetPlotspec(WPARAM wParam, LPARAM lParam)
	{

		//PlotData * pd=GetPD();


		if(pdl.empty()){
			COLORREF oc=(COLORREF)wParam;
			psview=PlotSpec(0,oc);
		}
		else{
			psview=pdl[m_spBtn.GetPos32()].psp;
		}

		return 0;
	}


	int CanalyzerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		LONG style0=GetWindowLongW(this->GetSafeHwnd(), GWL_EXSTYLE);
		SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

		if(//m_spBtn.Create(
			m_spBtn.CreateEx(
			WS_EX_TRANSPARENT
			//|WS_EX_CLIENTEDGE
			,WS_CHILD
			|WS_VISIBLE
			|UDS_HORZ
			|UDS_WRAP
			//|WS_CLIPCHILDREN
			//|WS_CLIPSIBLINGS
			, CRect()
			, this
			, 1
			)==FALSE
			)
			return -1;

		m_spBtn.SetRange32(0,0);

		m_tool.Create(this);
		m_tool.AddTool(this);
		m_tool.Activate(true);

		return 0;
	}


	void CanalyzerView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here

		m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));

		Invalidate();
	}


	afx_msg LRESULT CanalyzerView::OnMessageUpdateView(WPARAM wParam, LPARAM lParam)
	{

		//CanalyzerDoc* pDoc = GetDocument();

		//CMainFrame *mf=(CMainFrame*)(GetParentFrame());
		//COutputList* ol=mf->GetOutputWnd()->GetListCtrl();

		//pdl.clear();
		//UINT flg=RawData2PlotDataList(pDoc->raw, ol->dol, psview, pdl);

		if(pdl.empty())
			return 1;

		m_spBtn.SetRange32(0,pdl.size()-1);
		m_spBtn.ShowWindow( (pdl.size()>1 ? SW_SHOW : SW_HIDE) );

		int selecti=m_spBtn.GetPos32();
		if(selecti<0)
			selecti=0;

		UpdateRange(pdl[selecti].xll,xmin,xmax,pct,true);
		UpdateRange(pdl[selecti].yll,ymin,ymax,pct,true);

		this->Invalidate(FALSE);

		return 0;
	}


	void CanalyzerView::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here

		if(pdl.empty())
			return;

		// 创建属性表对象   
		CString str;
		str.LoadStringW(IDS_STRING_POLT_SETTINGS);
		CPropertySheet sheet(str);

		str.LoadStringW(IDS_STRING_FIGURE1);

		int selecti=m_spBtn.GetPos32();

		PlotSettingPage fig1setting(str,pdl[selecti].psp,pdl[selecti].ps);

		fig1setting.bkcr=pdl[selecti].psp.winbkC;

		sheet.AddPage(&fig1setting);

		// 打开模态向导对话框   
		if(sheet.DoModal()==IDOK){
			pdl[selecti].psp=fig1setting.fs;
			pdl[selecti].ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());

			::SendMessage(this->GetSafeHwnd(),MESSAGE_GET_PLOTSPEC,NULL,NULL);

			Invalidate(FALSE);

		}


	}


	void CanalyzerView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		if(!pdl.empty()){
			size_t selectIdx=m_spBtn.GetPos32();
			if(!pdl[selectIdx].ps.empty()){


				CRect plotrect;
				this->GetClientRect(&plotrect);	

				int re=DownUpdate(plotrect
					, pdl[selectIdx].psp.metricSize
					, pdl[selectIdx].psp.labelSize
					, point
					, m_mouseDownPoint
					, xmin, xmax, ymin, ymax
					, bMouseCursor
					, pdl[selectIdx].xll
					, pdl[selectIdx].yll
					, selectIdx);

				switch(re){
				case 1:
					SetCapture();
					break;
				case 2:
					this->ClientToScreen(&m_mouseDownPoint);
					::SetCursorPos(m_mouseDownPoint.x,m_mouseDownPoint.y);
					Invalidate(FALSE);
					break;
				default:
					break;
				}

			}
		}



		CView::OnLButtonDown(nFlags, point);
	}


	void CanalyzerView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		ReleaseCapture();

		HCURSOR hCur  =  LoadCursor( NULL  , IDC_ARROW ) ;
		::SetCursor(hCur);

		CView::OnLButtonUp(nFlags, point);
	}


	void CanalyzerView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		if(!pdl.empty()){
			size_t selectIdx=m_spBtn.GetPos32();
			if(!pdl[selectIdx].ps.empty()){

				//if(pd!=NULL && !pd->ps.empty() ){
				CRect plotrect;
				this->GetClientRect(&plotrect);	
				//ScreenToClient(&point);

				if(GetCapture()==this){
					if(MoveUpdateA(plotrect
						, pdl[selectIdx].psp.metricSize
						, pdl[selectIdx].psp.labelSize
						, point
						, this->m_mouseDownPoint
						, xmin,xmax,ymin,ymax))
						this->Invalidate(FALSE);
				}
				else{

					CString str;
					if(MoveUpdateB(plotrect
						, pdl[selectIdx].psp.metricSize
						, pdl[selectIdx].psp.labelSize
						, point
						, this->m_mouseDownPoint
						, xmin,xmax,ymin,ymax
						, str))
						m_tool.UpdateTipText(str,this);

				}

			}
		}


		CView::OnMouseMove(nFlags, point);
	}


	BOOL CanalyzerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default

		if(!pdl.empty()){
			size_t selectIdx=m_spBtn.GetPos32();
			if(!pdl[selectIdx].ps.empty()){

				//if(pd!=NULL && !pd->ps.empty() ){
				ScreenToClient(&pt);
				CRect plotrect;
				this->GetClientRect(&plotrect);
				if( WheelUpdate(plotrect
					, pdl[selectIdx].psp.metricSize
					, pdl[selectIdx].psp.labelSize
					, pt
					, ((zDelta>0)?zoomrate:1/zoomrate)
					,xmin,xmax,ymin,ymax) ){
						this->Invalidate(FALSE);
				}

			}
		}


		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void CanalyzerView::OnViewDatacursor()
	{
		// TODO: Add your command handler code here
		bMouseCursor=!bMouseCursor;
	}


	void CanalyzerView::OnUpdateViewDatacursor(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(bMouseCursor);
	}


	BOOL CanalyzerView::PreTranslateMessage(MSG* pMsg)
	{
		// TODO: Add your specialized code here and/or call the base class
		m_tool.RelayEvent(pMsg);

		return CView::PreTranslateMessage(pMsg);
	}


	void CanalyzerView::OnViewFitwindow()
	{
		// TODO: Add your command handler code here

		if(pdl.empty())
			return;
		size_t selectIdx=m_spBtn.GetPos32();

		if(pdl[selectIdx].ps.empty())
			return;
		
		UpdateRange(pdl[selectIdx].xll,xmin,xmax,pct,true);
		UpdateRange(pdl[selectIdx].yll,ymin,ymax,pct,true);
		//return true;

		this->Invalidate(FALSE);
	}
