
// analyzerView.cpp : implementation of the CanalyzerViewR class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "analyzer.h"
#endif

#include "analyzerDoc.h"
#include "analyzerViewR.h"


#include "func.h"
#include "PlotSettingPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CanalyzerViewR

IMPLEMENT_DYNCREATE(CanalyzerViewR, CView)

BEGIN_MESSAGE_MAP(CanalyzerViewR, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CanalyzerViewR::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_COMMAND(ID_OPTIONS_PLOTSETTINGS, &CanalyzerViewR::OnOptionsPlotsettings)
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_COMMAND(ID_VIEW_FITWINDOW, &CanalyzerViewR::OnViewFitwindow)
		ON_NOTIFY(UDN_DELTAPOS, 1, &CanalyzerViewR::OnDeltaposSpin)

		ON_COMMAND(ID_VIEW_DATACURSOR, &CanalyzerViewR::OnViewDatacursor)
		ON_UPDATE_COMMAND_UI(ID_VIEW_DATACURSOR, &CanalyzerViewR::OnUpdateViewDatacursor)
END_MESSAGE_MAP()

// CanalyzerViewR construction/destruction

CanalyzerViewR::CanalyzerViewR()
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

CanalyzerViewR::~CanalyzerViewR()
{
}

BOOL CanalyzerViewR::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CanalyzerViewR drawing

void CanalyzerViewR::OnDraw(CDC* pDC)
{
	CanalyzerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	if(!pDoc->rp.empty()){

			CDC dcMem;                                                  //用于缓冲作图的内存DC
			CBitmap bmp;                                                 //内存中承载临时图象的位图
			CRect rect;
			GetClientRect(&rect);
			dcMem.CreateCompatibleDC(pDC);               //依附窗口DC创建兼容内存DC
			bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图
			dcMem.SelectObject(&bmp);                          //将位图选择进内存DC

			CRect plotrect=rect;

			DrawData(plotrect,&dcMem,pDoc->rp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);

					if(bMouseCursor){
				DrawData1(plotrect
					,&dcMem
					,pDoc->rp[m_spBtn.GetPos32()].xll[selectIdx]
				,pDoc->rp[m_spBtn.GetPos32()].yll[selectIdx]
				,xmin,xmax,ymin,ymax);
			}


			pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
			dcMem.DeleteDC(); //删除DC
			bmp.DeleteObject(); //删除位图

			//////////////////////////////////////////////
			//CRect rect;
			//GetClientRect(&rect);
			//CRect plotrect=rect;
			//DrawData(plotrect,pDC,pDoc->rp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);


		}
}


// CanalyzerViewR printing


void CanalyzerViewR::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CanalyzerViewR::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CanalyzerViewR::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CanalyzerViewR::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CanalyzerViewR::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CanalyzerViewR::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CanalyzerViewR diagnostics

#ifdef _DEBUG
void CanalyzerViewR::AssertValid() const
{
	CView::AssertValid();
}

void CanalyzerViewR::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CanalyzerDoc* CanalyzerViewR::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CanalyzerDoc)));
	return (CanalyzerDoc*)m_pDocument;
}
#endif //_DEBUG


// CanalyzerViewR message handlers


int CanalyzerViewR::OnCreate(LPCREATESTRUCT lpCreateStruct)
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


void CanalyzerViewR::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_spBtn.MoveWindow(CRect(CPoint(cx,cy)-spBtnSize,spBtnSize));
	Invalidate();
}


void CanalyzerViewR::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if (!pDoc->rp.empty() && !pDoc->rp[idx].ps.empty()){
			CRect plotrect;
			this->GetClientRect(&plotrect);	
			//GetPlotRect(plotrect, pDoc->rp[idx].psp.labelSize, pDoc->rp[idx].psp.metricSize);

			//if(plotrect.PtInRect(point)){
			//	m_mouseDownPoint=point;
			//	SetCapture();

			//	HCURSOR hCur  =  LoadCursor( NULL  , IDC_SIZEALL ) ;
			//	::SetCursor(hCur);
			//}


			int re=DownUpdate(plotrect
				, pDoc->rp[idx].psp.metricSize
				, pDoc->rp[idx].psp.labelSize
				, point
				, m_mouseDownPoint
				, xmin, xmax, ymin, ymax
				, bMouseCursor
				, pDoc->rp[idx].xll
				, pDoc->rp[idx].yll
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


	void CanalyzerViewR::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default
		//m_mouseDownPoint=CPoint(0,0);
		ReleaseCapture();

		HCURSOR hCur  =  LoadCursor( NULL  , IDC_ARROW ) ;
		::SetCursor(hCur);

		CView::OnLButtonUp(nFlags, point);
	}


	void CanalyzerViewR::clear(void)
	{
		xmin=xmax=ymin=ymax=0;
		bMouseCursor=false;
		selectIdx=0;
		m_spBtn.SetRange32(0,0);
		m_spBtn.SetPos32(0);
		m_spBtn.ShowWindow(SW_HIDE);
		CanalyzerDoc* pDoc=GetDocument();
		if( !(pDoc->rp.empty()) )
			pDoc->rp.clear();
	}


	bool CanalyzerViewR::updatePlotRange(int plotIndex, const std::vector<double> &x, const std::vector<double> &y, bool flg)
	{
		if(plotIndex==m_spBtn.GetPos32()){
			double pct=0.02;
			UpdateRange(x,xmin,xmax,pct,flg);
			UpdateRange(y,ymin,ymax,pct,flg);
			return true;
		}
		return false;
	}

	void CanalyzerViewR::OnOptionsPlotsettings()
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
		if(!pDoc->rp.empty()){
			int il=m_spBtn.GetPos32();
			str.LoadStringW(IDS_STRING_FIGURE1);
			PlotSettingPage fig1setting(str
				,pDoc->rp[il].psp
				,pDoc->rp[il].ps
				,pDoc->rp[il].xlabel
				,pDoc->rp[il].ylabel);

			sheet.AddPage(&fig1setting);

			// 打开模态向导对话框   
			if(sheet.DoModal()==IDOK){

				pDoc->rp[il].xlabel=fig1setting.xlabel;
				pDoc->rp[il].ylabel=fig1setting.ylabel;
				pDoc->rp[il].psp=fig1setting.fs;
				pDoc->rp[il].ps.clear();
				pDoc->rp[il].ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());
				Invalidate(FALSE);
			}
		}
	}


	void CanalyzerViewR::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if (!pDoc->rp.empty() && !pDoc->rp[idx].ps.empty()){
			CRect plotrect;
			this->GetClientRect(&plotrect);	
			//ScreenToClient(&point);

			if(GetCapture()==this){
				if(MoveUpdateA(plotrect
					, pDoc->rp[idx].psp.metricSize
					, pDoc->rp[idx].psp.labelSize
					, point
					, this->m_mouseDownPoint
					, xmin,xmax,ymin,ymax))
					this->Invalidate(FALSE);
			}
			else{

				CString str;
				if(MoveUpdateB(plotrect
					, pDoc->rp[idx].psp.metricSize
					, pDoc->rp[idx].psp.labelSize
					, point
					, this->m_mouseDownPoint
					, xmin,xmax,ymin,ymax
					, str))
					m_tool.UpdateTipText(str,this);

				//str.Format(L"%d,%d",point.x,point.y);
				//m_tool.UpdateTipText(str,this);

				//CPoint pt;
				//int flg=MoveUpdateC(plotrect
				//	, pDoc->rp[idx].psp.metricSize
				//	, pDoc->rp[idx].psp.labelSize
				//	, point
				//	, this->m_mouseDownPoint
				//	, xmin,xmax,ymin,ymax
				//	, str
				//	, pDoc->rp[idx].xll
				//	, pDoc->rp[idx].yll
				//	, pt);
				//if(flg==2){
				//		//this->ClientToScreen(&pt);
				//		//::SetCursorPos(pt.x,pt.y);
				//	m_tool.UpdateTipText(str,this);
				//}
				//if(flg==1){
				//	m_tool.UpdateTipText(str,this);
				//}

			}

		}

		CView::OnMouseMove(nFlags, point);
	}


	BOOL CanalyzerViewR::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default


		CanalyzerDoc* pDoc=GetDocument();
		int idx=m_spBtn.GetPos32();

		if(!pDoc->rp.empty() && !pDoc->rp[idx].ps.empty()){
			ScreenToClient(&pt);
			CRect plotrect;
			this->GetClientRect(&plotrect);
			if( WheelUpdate(plotrect
				, pDoc->rp[idx].psp.metricSize
				, pDoc->rp[idx].psp.labelSize
				, pt
				, ((zDelta>0)?0.8:1.25)
				,xmin,xmax,ymin,ymax) ){
					this->Invalidate(FALSE);
			}

		}

		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	// update xmin,xmax,ymin,ymax
	bool CanalyzerViewR::updatePlotRange(int plotIndex, bool flg)
	{
		CanalyzerDoc* pDoc=GetDocument();
		if(pDoc->rp.empty())
			return false;

		return updatePlotRange(plotIndex,pDoc->rp[plotIndex].xll,pDoc->rp[plotIndex].yll,flg);
	}

	// update xmin,xmax,ymin,ymax
	bool CanalyzerViewR::updatePlotRange(bool flg)
	{
		CanalyzerDoc* pDoc=GetDocument();
		if(pDoc->rp.empty())
			return false;

		double pct=0.02;
		int ci=m_spBtn.GetPos32();
		UpdateRange(pDoc->rp[ci].xll,xmin,xmax,pct,flg);
		UpdateRange(pDoc->rp[ci].yll,ymin,ymax,pct,flg);
		return true;
	}


	int CanalyzerViewR::AddPlot(const PlotData & pda)
	{
		CanalyzerDoc* pDoc=GetDocument();
		pDoc->rp.push_back(pda);
		int newi=pDoc->rp.size()-1;
		SetSpin(newi);
		return newi;
	}

	void CanalyzerViewR::OnViewFitwindow()
	{
		// TODO: Add your command handler code here

		if(this->updatePlotRange())
			this->Invalidate(FALSE);

	}

	void CanalyzerViewR::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
	{
		NM_UPDOWN* pNMUpDown=(NM_UPDOWN*)pNMHDR;	

		CanalyzerDoc* pDoc=GetDocument();

		int n=pDoc->rp.size();
		int newpos=pNMUpDown->iPos+pNMUpDown->iDelta;
		if(newpos>=n)
			newpos=0;
		if(newpos<0)
			newpos=n-1;
		//int newpos=pNMUpDown->iPos;

		//if(updatePlotRange(newpos))
		//if(updatePlotRange())
		double pct=0.02;
		UpdateRange(pDoc->rp[newpos].xll,xmin,xmax,pct,true);
		UpdateRange(pDoc->rp[newpos].yll,ymin,ymax,pct,true);
		Invalidate(FALSE);

		*pResult = 0;
	}

	void CanalyzerViewR::SetSpin(int newi)
	{
		m_spBtn.SetRange32(0,newi);
		m_spBtn.SetPos32(newi);
		m_spBtn.ShowWindow( (newi>0 ? SW_SHOW : SW_HIDE) );
	}


	BOOL CanalyzerViewR::PreTranslateMessage(MSG* pMsg)
	{
		// TODO: Add your specialized code here and/or call the base class

		m_tool.RelayEvent(pMsg);


		return CView::PreTranslateMessage(pMsg);
	}

	void CanalyzerViewR::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
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


	void CanalyzerViewR::OnViewDatacursor()
	{
		// TODO: Add your command handler code here
		bMouseCursor=!bMouseCursor;
	}


	void CanalyzerViewR::OnUpdateViewDatacursor(CCmdUI *pCmdUI)
	{
		// TODO: Add your command update UI handler code here
		pCmdUI->SetCheck(bMouseCursor);
	}
