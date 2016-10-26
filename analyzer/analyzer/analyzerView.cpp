
// analyzerView.cpp : implementation of the CanalyzerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "analyzerDoc.h"
#include "analyzerView.h"


#include "func.h"
#include "PlotSettingPage.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CanalyzerView

IMPLEMENT_DYNCREATE(CanalyzerView, CView)

	BEGIN_MESSAGE_MAP(CanalyzerView, CView)
		// Standard printing commands
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CanalyzerView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()

		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerView::OnOptionsPlotsettings)
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerView::OnViewFitwindow)
		ON_NOTIFY(UDN_DELTAPOS, 1, &CanalyzerView::OnDeltaposSpin)

		ON_COMMAND(ID_VIEW_DATACURSOR, &CanalyzerView::OnViewDatacursor)
		ON_UPDATE_COMMAND_UI(ID_VIEW_DATACURSOR, &CanalyzerView::OnUpdateViewDatacursor)
	END_MESSAGE_MAP()

	// CanalyzerView construction/destruction

	CanalyzerView::CanalyzerView()
		: xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
		, m_mouseDownPoint(0)
		, pct(0.02)
		, bMouseCursor(false)
		, selectIdx(0)
	{
		// TODO: add construction code here
		spBtnSize=CSize(23*2,23);
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

		if(!pDoc->lp.empty()){

			CDC dcMem;                                                  //用于缓冲作图的内存DC
			CBitmap bmp;                                                 //内存中承载临时图象的位图
			CRect rect;
			GetClientRect(&rect);
			dcMem.CreateCompatibleDC(pDC);               //依附窗口DC创建兼容内存DC
			bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图
			dcMem.SelectObject(&bmp);  	//将位图选择进内存DC
			CRect plotrect=rect;

			DrawData(plotrect,&dcMem,pDoc->lp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);
			if(bMouseCursor){
				//CString str;
				//str.Format(L"%g,%g",pDoc->lp[m_spBtn.GetPos32()].xll[selectIdx],pDoc->lp[m_spBtn.GetPos32()].yll[selectIdx]);
				//dcMem.TextOutW(m_mouseDownPoint.x,m_mouseDownPoint.y,str);

				DrawData1(plotrect
					,&dcMem
					,pDoc->lp[m_spBtn.GetPos32()].xll[selectIdx]
				,pDoc->lp[m_spBtn.GetPos32()].yll[selectIdx]
				,xmin,xmax,ymin,ymax);
			}

			pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
			dcMem.DeleteDC(); //删除DC
			bmp.DeleteObject(); //删除位图

			//////////////////////////////////////////////
			//CRect rect;
			//GetClientRect(&rect);
			//CRect plotrect=rect;
			//DrawData(plotrect,pDC,pDoc->lp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);


		}
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


	int CanalyzerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		if(m_spBtn.Create(WS_CHILD
			//|WS_VISIBLE
			|UDS_HORZ
			|UDS_WRAP
			//|WS_CLIPCHILDREN
			//|WS_CLIPSIBLINGS
			, CRect()
			, this
			, 1)==FALSE)
			return -1;

		m_spBtn.SetRange32(0,0);


		m_tool.Create(this);
		m_tool.AddTool(this);
		//m_tool.SetDelayTime(1);
		m_tool.Activate(true);


		LONG style0=GetWindowLongW(m_hWnd, GWL_EXSTYLE);
		SetWindowLongW(GetSafeHwnd(), GWL_EXSTYLE, style0|WS_CLIPCHILDREN);

		return 0;
	}


	void CanalyzerView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: Add your message handler code here
		m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));
		Invalidate();
	}


	void CanalyzerView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if (!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
			CRect plotrect;
			this->GetClientRect(&plotrect);	

			int re=DownUpdate(plotrect
				, pDoc->lp[idx].psp.metricSize
				, pDoc->lp[idx].psp.labelSize
				, point
				, m_mouseDownPoint
				, xmin, xmax, ymin, ymax
				, bMouseCursor
				, pDoc->lp[idx].xll
				, pDoc->lp[idx].yll
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

		CView::OnLButtonDown(nFlags, point);
	}


	void CanalyzerView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		//m_mouseDownPoint=CPoint(0,0);
		ReleaseCapture();

		HCURSOR hCur  =  LoadCursor( NULL  , IDC_ARROW ) ;
		::SetCursor(hCur);

		CView::OnLButtonUp(nFlags, point);
	}


	void CanalyzerView::clear(void)
	{
		xmin=xmax=ymin=ymax=0;

		bMouseCursor=false;
		selectIdx=0;

		m_spBtn.SetRange32(0,0);
		m_spBtn.SetPos32(0);
		m_spBtn.ShowWindow(SW_HIDE);
		CanalyzerDoc* pDoc=GetDocument();
		if( !(pDoc->lp.empty()) )
			pDoc->lp.clear();
	}


	bool CanalyzerView::updatePlotRange(int plotIndex, const std::vector<double> &x, const std::vector<double> &y, bool flg)
	{
		if(plotIndex==m_spBtn.GetPos32()){
			UpdateRange(x,xmin,xmax,pct,flg);
			UpdateRange(y,ymin,ymax,pct,flg);
			return true;
		}
		return false;
	}

	void CanalyzerView::OnOptionsPlotsettings()
	{
		// TODO: Add your command handler code here

		//AfxMessageBox(L"left");


		// 创建属性表对象   
		CString str;
		str.LoadStringW(IDS_STRING_POLT_SETTINGS);
		CPropertySheet sheet(str);
		//abc sheet(777);
		// 设置属性对话框为向导对话框   
		//sheet.SetWizardMode();   
		//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

		CanalyzerDoc* pDoc=GetDocument();
		if(!pDoc->lp.empty()){
			int il=m_spBtn.GetPos32();
			str.LoadStringW(IDS_STRING_FIGURE1);
			PlotSettingPage fig1setting(str
				,pDoc->lp[il].psp
				,pDoc->lp[il].ps
				,pDoc->lp[il].xlabel
				,pDoc->lp[il].ylabel);

			sheet.AddPage(&fig1setting);

			// 打开模态向导对话框   
			if(sheet.DoModal()==IDOK){

				pDoc->lp[il].xlabel=fig1setting.xlabel;
				pDoc->lp[il].ylabel=fig1setting.ylabel;
				pDoc->lp[il].psp=fig1setting.fs;
				pDoc->lp[il].ps.clear();
				pDoc->lp[il].ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());
				Invalidate(FALSE);
				//this->UpdateWindow();
			}
		}
	}


	void CanalyzerView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if (!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
			CRect plotrect;
			this->GetClientRect(&plotrect);	
			//ScreenToClient(&point);

			if(GetCapture()==this){
				if(MoveUpdateA(plotrect
					, pDoc->lp[idx].psp.metricSize
					, pDoc->lp[idx].psp.labelSize
					, point
					, this->m_mouseDownPoint
					, xmin,xmax,ymin,ymax))
					this->Invalidate(FALSE);
			}
			else{

				CString str;
				if(MoveUpdateB(plotrect
					, pDoc->lp[idx].psp.metricSize
					, pDoc->lp[idx].psp.labelSize
					, point
					, this->m_mouseDownPoint
					, xmin,xmax,ymin,ymax
					, str))
					m_tool.UpdateTipText(str,this);

				//str.Format(L"%d,%d",point.x,point.y);
				//m_tool.UpdateTipText(str,this);

				//CPoint pt;
				//int flg=MoveUpdateC(plotrect
				//	, pDoc->lp[idx].psp.metricSize
				//	, pDoc->lp[idx].psp.labelSize
				//	, point
				//	, this->m_mouseDownPoint
				//	, xmin,xmax,ymin,ymax
				//	, str
				//	, pDoc->lp[idx].xll
				//	, pDoc->lp[idx].yll
				//	, pt);
				//if(flg==2){
				//	//this->ClientToScreen(&pt);
				//	//::SetCursorPos(pt.x,pt.y);
				//	m_tool.UpdateTipText(str,this);
				//}
				//if(flg==1){
				//	m_tool.UpdateTipText(str,this);
				//}

			}

		}

		CView::OnMouseMove(nFlags, point);
	}


	BOOL CanalyzerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default


		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if(!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
			ScreenToClient(&pt);
			CRect plotrect;
			this->GetClientRect(&plotrect);
			if( WheelUpdate(plotrect
				, pDoc->lp[idx].psp.metricSize
				, pDoc->lp[idx].psp.labelSize
				, pt
				, ((zDelta>0)?0.8:1.25)
				,xmin,xmax,ymin,ymax) ){
					this->Invalidate(FALSE);
			}

		}

		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	// update xmin,xmax,ymin,ymax
	bool CanalyzerView::updatePlotRange(int plotIndex, bool flg)
	{
		CanalyzerDoc* pDoc=GetDocument();
		if(pDoc->lp.empty())
			return false;

		return updatePlotRange(plotIndex,pDoc->lp[plotIndex].xll,pDoc->lp[plotIndex].yll,flg);
	}

	// update xmin,xmax,ymin,ymax
	bool CanalyzerView::updatePlotRange(bool flg)
	{
		CanalyzerDoc* pDoc=GetDocument();
		if(pDoc->lp.empty())
			return false;
		int ci=m_spBtn.GetPos32();
		UpdateRange(pDoc->lp[ci].xll,xmin,xmax,pct,flg);
		UpdateRange(pDoc->lp[ci].yll,ymin,ymax,pct,flg);
		return true;
	}


	int CanalyzerView::AddPlot(const PlotData & pda)
	{
		CanalyzerDoc* pDoc=GetDocument();
		pDoc->lp.push_back(pda);
		int newi=pDoc->lp.size()-1;
		SetSpin(newi);
		return newi;
	}

	void CanalyzerView::OnViewFitwindow()
	{
		// TODO: Add your command handler code here

		if(this->updatePlotRange())
			this->Invalidate(FALSE);

	}

	void CanalyzerView::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
	{
		NM_UPDOWN* pNMUpDown=(NM_UPDOWN*)pNMHDR;	

		CanalyzerDoc* pDoc=GetDocument();

		int n=pDoc->lp.size();
		int newpos=pNMUpDown->iPos+pNMUpDown->iDelta;
		if(newpos>=n)
			newpos=0;
		if(newpos<0)
			newpos=n-1;
		//int newpos=pNMUpDown->iPos;

		//if(updatePlotRange(newpos))
		//if(updatePlotRange())
		UpdateRange(pDoc->lp[newpos].xll,xmin,xmax,pct,true);
		UpdateRange(pDoc->lp[newpos].yll,ymin,ymax,pct,true);
		Invalidate(FALSE);

		*pResult = 0;
	}

	void CanalyzerView::SetSpin(int newi)
	{
		m_spBtn.SetRange32(0,newi);
		m_spBtn.SetPos32(newi);
		m_spBtn.ShowWindow( (newi>0 ? SW_SHOW : SW_HIDE) );
	}


	BOOL CanalyzerView::PreTranslateMessage(MSG* pMsg)
	{
		// TODO: Add your specialized code here and/or call the base class

		m_tool.RelayEvent(pMsg);


		return CView::PreTranslateMessage(pMsg);
	}

	void CanalyzerView::OnInitialUpdate()
	{
		CView::OnInitialUpdate();

		// TODO: Add your specialized code here and/or call the base class

	}


	void CanalyzerView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
	{
		// TODO: Add your specialized code here and/or call the base class

		pDC->SetMapMode(MM_ANISOTROPIC); //转换坐标映射方式
		CRect rect;
		this->GetClientRect(&rect);
		CSize wsize = rect.Size(); 
		pDC->SetWindowExt(wsize); 

		HDC hdc=::GetDC(this->GetSafeHwnd());
		int wmm=::GetDeviceCaps(hdc,HORZSIZE);
		int hmm=::GetDeviceCaps(hdc,VERTSIZE);
		int wpxl=::GetDeviceCaps(hdc,HORZRES);
		int hpxl=::GetDeviceCaps(hdc,VERTRES);
		int xLogPixPerInch0 = ::GetDeviceCaps(hdc,LOGPIXELSX); 
		int yLogPixPerInch0 = ::GetDeviceCaps(hdc,LOGPIXELSY); 
		::ReleaseDC(this->GetSafeHwnd(),hdc);

		//得到实际设备每逻辑英寸的象素数量
		int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX); 
		int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY); 
		//得到设备坐标和逻辑坐标的比例
		CSize vsize(wsize.cx * xLogPixPerInch/xLogPixPerInch0, wsize.cy * yLogPixPerInch/yLogPixPerInch0);
		pDC->SetViewportExt(vsize); //确定视口大小


		CView::OnPrepareDC(pDC, pInfo);
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
