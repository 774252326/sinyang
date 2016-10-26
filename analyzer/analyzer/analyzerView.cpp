
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

#include "MainFrm.h"

#include "func.h"
#include "PlotSettingPageA.h"
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
		, lri(0)
		, bkcr(0)
	{
		// TODO: add construction code here
		spBtnSize=CSize(23*2,23);
	}


	CanalyzerView::CanalyzerView(int i)
		: xmin(0)
		, xmax(0)
		, ymin(0)
		, ymax(0)
		, m_mouseDownPoint(0)
		, pct(0.02)
		, bMouseCursor(false)
		, selectIdx(0)
		, lri(i)
		, bkcr(0)
	{
		// TODO: add construction code here
		spBtnSize=CSize(23*2,23);
		lri=i;
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

		PlotData *pd=GetPD();
		if(pd!=NULL){
			CDC dcMem;                                                  //���ڻ�����ͼ���ڴ�DC
			CBitmap bmp;                                                 //�ڴ��г�����ʱͼ���λͼ
			CRect rect;
			GetClientRect(&rect);
			dcMem.CreateCompatibleDC(pDC);               //��������DC���������ڴ�DC
			CSize winsz=rect.Size();
			bmp.CreateCompatibleBitmap(pDC,winsz.cx,winsz.cy);//��������λͼ
			dcMem.SelectObject(&bmp);  	//��λͼѡ����ڴ�DC

			DrawData(rect,&dcMem,*pd,xmin,xmax,ymin,ymax,bkcr);
			if(bMouseCursor && !pd->ps.empty()){
				//CString str;
				//str.Format(L"%g,%g",pDoc->lp[m_spBtn.GetPos32()].xll[selectIdx],pDoc->lp[m_spBtn.GetPos32()].yll[selectIdx]);
				//dcMem.TextOutW(m_mouseDownPoint.x,m_mouseDownPoint.y,str);

				DrawData1(rect
					,&dcMem
					,pd->xll[selectIdx]
				,pd->yll[selectIdx]
				,xmin,xmax,ymin,ymax
					,inv(pd->psp.bkgndC));
			}

			pDC->BitBlt(0,0,winsz.cx,winsz.cy,&dcMem,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
			dcMem.DeleteDC(); //ɾ��DC
			bmp.DeleteObject(); //ɾ��λͼ

			//////////////////////////////////////////////
			//CRect rect;
			//GetClientRect(&rect);
			//CRect plotrect=rect;
			//DrawData(plotrect,pDC,pDoc->lp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);


		}


		//if(!pDoc->lp.empty()){

		//	CDC dcMem;                                                  //���ڻ�����ͼ���ڴ�DC
		//	CBitmap bmp;                                                 //�ڴ��г�����ʱͼ���λͼ
		//	CRect rect;
		//	GetClientRect(&rect);
		//	dcMem.CreateCompatibleDC(pDC);               //��������DC���������ڴ�DC
		//	bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//��������λͼ
		//	dcMem.SelectObject(&bmp);  	//��λͼѡ����ڴ�DC
		//	CRect plotrect=rect;

		//	DrawData(plotrect,&dcMem,pDoc->lp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);
		//	if(bMouseCursor){
		//		//CString str;
		//		//str.Format(L"%g,%g",pDoc->lp[m_spBtn.GetPos32()].xll[selectIdx],pDoc->lp[m_spBtn.GetPos32()].yll[selectIdx]);
		//		//dcMem.TextOutW(m_mouseDownPoint.x,m_mouseDownPoint.y,str);

		//		DrawData1(plotrect
		//			,&dcMem
		//			,pDoc->lp[m_spBtn.GetPos32()].xll[selectIdx]
		//		,pDoc->lp[m_spBtn.GetPos32()].yll[selectIdx]
		//		,xmin,xmax,ymin,ymax);
		//	}

		//	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
		//	dcMem.DeleteDC(); //ɾ��DC
		//	bmp.DeleteObject(); //ɾ��λͼ

		//	//////////////////////////////////////////////
		//	//CRect rect;
		//	//GetClientRect(&rect);
		//	//CRect plotrect=rect;
		//	//DrawData(plotrect,pDC,pDoc->lp[m_spBtn.GetPos32()],xmin,xmax,ymin,ymax);


		//}
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

		//CanalyzerDoc* pDoc=GetDocument();
		////int idx=m_spBtn.GetPos32();

		//PlotData * pd=pDoc->GetPD(lri,idx);
		PlotData * pd=GetPD();
		if(pd!=NULL && !pd->ps.empty() ){
			CRect plotrect;
			this->GetClientRect(&plotrect);	

			int re=DownUpdate(plotrect
				, pd->psp.metricSize
				, pd->psp.labelSize
				, point
				, m_mouseDownPoint
				, xmin, xmax, ymin, ymax
				, bMouseCursor
				, pd->xll
				, pd->yll
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


		//if (!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
		//	CRect plotrect;
		//	this->GetClientRect(&plotrect);	

		//	int re=DownUpdate(plotrect
		//		, pDoc->lp[idx].psp.metricSize
		//		, pDoc->lp[idx].psp.labelSize
		//		, point
		//		, m_mouseDownPoint
		//		, xmin, xmax, ymin, ymax
		//		, bMouseCursor
		//		, pDoc->lp[idx].xll
		//		, pDoc->lp[idx].yll
		//		, selectIdx);

		//	switch(re){
		//	case 1:
		//		SetCapture();
		//		break;
		//	case 2:
		//		this->ClientToScreen(&m_mouseDownPoint);
		//		::SetCursorPos(m_mouseDownPoint.x,m_mouseDownPoint.y);
		//		Invalidate(FALSE);
		//		break;
		//	default:
		//		break;
		//	}

		//}

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

		switch(lri){
		case 0:
			if( !(pDoc->lp.empty()) )
				pDoc->lp.clear();
			return;
		case 1:
			if( !(pDoc->rp.empty()) )
				pDoc->rp.clear();
			return;
		default:
			return;
		}

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


		//SavePic(CSize(1000,600),L"view.jpg");


		// �������Ա����   
		CString str;
		str.LoadStringW(IDS_STRING_POLT_SETTINGS);
		CPropertySheet sheet(str);
		//abc sheet(777);
		// �������ԶԻ���Ϊ�򵼶Ի���   
		//sheet.SetWizardMode();   
		//sheet.SetWindowPos(&CWnd::wndTopMost,10,10,800,600,SWP_SHOWWINDOW);

		PlotData * pd=GetPD();
		if(pd!=NULL){
			str.LoadStringW(IDS_STRING_FIGURE1+lri);
			PlotSettingPage fig1setting(str
				,pd->psp
				,pd->ps
				,pd->xlabel
				,pd->ylabel);

			sheet.AddPage(&fig1setting);

			// ��ģ̬�򵼶Ի���   
			if(sheet.DoModal()==IDOK){
				pd->xlabel=fig1setting.xlabel;
				pd->ylabel=fig1setting.ylabel;
				pd->psp=fig1setting.fs;
				pd->ps.clear();
				pd->ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());
				Invalidate(FALSE);
				//this->UpdateWindow();
			}
		}



		//CanalyzerDoc* pDoc=GetDocument();
		//if(!pDoc->lp.empty()){
		//	int il=m_spBtn.GetPos32();
		//	str.LoadStringW(IDS_STRING_FIGURE1);
		//	PlotSettingPage fig1setting(str
		//		,pDoc->lp[il].psp
		//		,pDoc->lp[il].ps
		//		,pDoc->lp[il].xlabel
		//		,pDoc->lp[il].ylabel);

		//	sheet.AddPage(&fig1setting);

		//	// ��ģ̬�򵼶Ի���   
		//	if(sheet.DoModal()==IDOK){

		//		pDoc->lp[il].xlabel=fig1setting.xlabel;
		//		pDoc->lp[il].ylabel=fig1setting.ylabel;
		//		pDoc->lp[il].psp=fig1setting.fs;
		//		pDoc->lp[il].ps.clear();
		//		pDoc->lp[il].ps.assign(fig1setting.ps.begin(),fig1setting.ps.end());
		//		Invalidate(FALSE);
		//		//this->UpdateWindow();
		//	}
		//}
	}


	void CanalyzerView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: Add your message handler code here and/or call default

		//CanalyzerDoc* pDoc=GetDocument();
		//int idx=m_spBtn.GetPos32();

		//PlotData * pd=pDoc->GetPD(lri,idx);
		PlotData * pd=GetPD();
		if(pd!=NULL && !pd->ps.empty() ){
			CRect plotrect;
			this->GetClientRect(&plotrect);	
			//ScreenToClient(&point);

			if(GetCapture()==this){
				if(MoveUpdateA(plotrect
					, pd->psp.metricSize
					, pd->psp.labelSize
					, point
					, this->m_mouseDownPoint
					, xmin,xmax,ymin,ymax))
					this->Invalidate(FALSE);
			}
			else{

				CString str;
				if(MoveUpdateB(plotrect
					, pd->psp.metricSize
					, pd->psp.labelSize
					, point
					, this->m_mouseDownPoint
					, xmin,xmax,ymin,ymax
					, str))
					m_tool.UpdateTipText(str,this);

			}

		}




		//if (!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
		//	CRect plotrect;
		//	this->GetClientRect(&plotrect);	
		//	//ScreenToClient(&point);

		//	if(GetCapture()==this){
		//		if(MoveUpdateA(plotrect
		//			, pDoc->lp[idx].psp.metricSize
		//			, pDoc->lp[idx].psp.labelSize
		//			, point
		//			, this->m_mouseDownPoint
		//			, xmin,xmax,ymin,ymax))
		//			this->Invalidate(FALSE);
		//	}
		//	else{

		//		CString str;
		//		if(MoveUpdateB(plotrect
		//			, pDoc->lp[idx].psp.metricSize
		//			, pDoc->lp[idx].psp.labelSize
		//			, point
		//			, this->m_mouseDownPoint
		//			, xmin,xmax,ymin,ymax
		//			, str))
		//			m_tool.UpdateTipText(str,this);

		//		//str.Format(L"%d,%d",point.x,point.y);
		//		//m_tool.UpdateTipText(str,this);

		//		//CPoint pt;
		//		//int flg=MoveUpdateC(plotrect
		//		//	, pDoc->lp[idx].psp.metricSize
		//		//	, pDoc->lp[idx].psp.labelSize
		//		//	, point
		//		//	, this->m_mouseDownPoint
		//		//	, xmin,xmax,ymin,ymax
		//		//	, str
		//		//	, pDoc->lp[idx].xll
		//		//	, pDoc->lp[idx].yll
		//		//	, pt);
		//		//if(flg==2){
		//		//	//this->ClientToScreen(&pt);
		//		//	//::SetCursorPos(pt.x,pt.y);
		//		//	m_tool.UpdateTipText(str,this);
		//		//}
		//		//if(flg==1){
		//		//	m_tool.UpdateTipText(str,this);
		//		//}

		//	}

		//}

		CView::OnMouseMove(nFlags, point);
	}


	BOOL CanalyzerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: Add your message handler code here and/or call default


		//CanalyzerDoc* pDoc=GetDocument();
		//int idx=m_spBtn.GetPos32();

		//PlotData * pd=pDoc->GetPD(lri,idx);
		PlotData * pd=GetPD();

		if(pd!=NULL && !pd->ps.empty() ){
			ScreenToClient(&pt);
			CRect plotrect;
			this->GetClientRect(&plotrect);
			if( WheelUpdate(plotrect
				, pd->psp.metricSize
				, pd->psp.labelSize
				, pt
				, ((zDelta>0)?0.8:1.25)
				,xmin,xmax,ymin,ymax) ){
					this->Invalidate(FALSE);
			}

		}

		//if(!pDoc->lp.empty() && !pDoc->lp[idx].ps.empty()){
		//	ScreenToClient(&pt);
		//	CRect plotrect;
		//	this->GetClientRect(&plotrect);
		//	if( WheelUpdate(plotrect
		//		, pDoc->lp[idx].psp.metricSize
		//		, pDoc->lp[idx].psp.labelSize
		//		, pt
		//		, ((zDelta>0)?0.8:1.25)
		//		,xmin,xmax,ymin,ymax) ){
		//			this->Invalidate(FALSE);
		//	}

		//}

		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	// update xmin,xmax,ymin,ymax
	bool CanalyzerView::updatePlotRange(int plotIndex, bool flg)
	{
		//CanalyzerDoc* pDoc=GetDocument();
		//if(pDoc->lp.empty())
		//	return false;

		//return updatePlotRange(plotIndex,pDoc->lp[plotIndex].xll,pDoc->lp[plotIndex].yll,flg);



		PlotData *pd=GetPD(plotIndex);
		if(pd==NULL)
			return false;

		return updatePlotRange(plotIndex,pd->xll,pd->yll,flg);
	}

	// update xmin,xmax,ymin,ymax
	bool CanalyzerView::updatePlotRange(bool flg)
	{
		//CanalyzerDoc* pDoc=GetDocument();
		//if(pDoc->lp.empty())
		//	return false;
		//int ci=m_spBtn.GetPos32();
		//UpdateRange(pDoc->lp[ci].xll,xmin,xmax,pct,flg);
		//UpdateRange(pDoc->lp[ci].yll,ymin,ymax,pct,flg);
		//return true;


		PlotData *pd=GetPD();
		if(pd==NULL)
			return false;
		UpdateRange(pd->xll,xmin,xmax,pct,flg);
		UpdateRange(pd->yll,ymin,ymax,pct,flg);
		return true;
	}


	int CanalyzerView::AddPlot(const PlotData & pda)
	{
		CanalyzerDoc* pDoc=GetDocument();

		//pDoc->lp.push_back(pda);
		//int newi=pDoc->lp.size()-1;
		int newi=pDoc->AddPD(pda,lri)-1;
		selectIdx=0;
		bMouseCursor=false;
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

		int n=pDoc->GetNPD(lri);
		int newpos=pNMUpDown->iPos+pNMUpDown->iDelta;
		if(newpos>=n)
			newpos=0;
		if(newpos<0)
			newpos=n-1;
		//int newpos=pNMUpDown->iPos;

		//if(updatePlotRange(newpos))
		//if(updatePlotRange())

		PlotData *pd=GetPD(newpos);

		//UpdateRange(pDoc->lp[newpos].xll,xmin,xmax,pct,true);
		//UpdateRange(pDoc->lp[newpos].yll,ymin,ymax,pct,true);

		UpdateRange(pd->xll,xmin,xmax,pct,true);
		UpdateRange(pd->yll,ymin,ymax,pct,true);

		selectIdx=0;
		bMouseCursor=false;

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

		selectIdx=0;
		SetSpin((this->GetDocument()->GetNPD(lri))-1);
		updatePlotRange();

		if(lri==0){
			CMainFrame *mf=(CMainFrame*)(GetParentFrame());
			COutputList* ol=mf->GetOutputWnd()->GetListCtrl();
			ol->DeleteAllItems();

			CanalyzerDoc* pDoc=GetDocument();

			size_t c=0;
			for(size_t i=0;i<pDoc->dol.size();i++){
				for(size_t j=0;j<pDoc->dol[i].Ar.size();j++){
					ol->InsertListCtrl(c,pDoc->dol[i],i,j);
					c++;
				}
			}

			CString str=Compute(pDoc->dol,pDoc->p1);
			mf->GetCaptionBar()->ShowMessage(str);
		}

	}


	void CanalyzerView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
	{
		// TODO: Add your specialized code here and/or call the base class

		pDC->SetMapMode(MM_ANISOTROPIC); //ת������ӳ�䷽ʽ
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

		//�õ�ʵ���豸ÿ�߼�Ӣ�����������
		int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX); 
		int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY); 
		//�õ��豸������߼�����ı���
		CSize vsize(wsize.cx * xLogPixPerInch/xLogPixPerInch0, wsize.cy * yLogPixPerInch/yLogPixPerInch0);
		pDC->SetViewportExt(vsize); //ȷ���ӿڴ�С


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


	PlotData * CanalyzerView::GetPD(void)
	{
		return GetPD(m_spBtn.GetPos32());	
	}

	PlotData * CanalyzerView::GetPD(int idx)
	{
		CanalyzerDoc* pDoc=GetDocument();

		//ASSERT_VALID(pDoc);
		//if (!pDoc)
		//	return NULL;

		return pDoc->GetPD(lri,idx);	
	}

	bool CanalyzerView::SavePic(CSize sz, CString filename)
	{

		PlotData *pd=GetPD();
		if(pd!=NULL){  			
			CDC* pdc=this->GetDC();
			CDC dcMem;   //���ڻ�����ͼ���ڴ�DC
			dcMem.CreateCompatibleDC(pdc);               //��������DC���������ڴ�DC		

			CBitmap bmp;           //�ڴ��г�����ʱͼ���λͼ
			bmp.CreateCompatibleBitmap(pdc,sz.cx,sz.cy);//��������λͼ

			dcMem.SelectObject(&bmp);  	//��λͼѡ����ڴ�DC

			CRect plotrect(0,0,sz.cx,sz.cy);
			DrawData(plotrect,&dcMem,*pd,xmin,xmax,ymin,ymax);

			CImage img;
			img.Attach(HBITMAP(bmp));
			HRESULT hResult = img.Save((LPCWSTR)filename);

			dcMem.DeleteDC(); //ɾ��DC
			bmp.DeleteObject(); //ɾ��λͼ
			if (SUCCEEDED(hResult))
				return true;

		}
		return false;
	}
