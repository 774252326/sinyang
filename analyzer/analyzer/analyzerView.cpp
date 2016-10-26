
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
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_ANALYSIS_ABORTANALYSIS, &CanalyzerView::OnAnalysisAbortanalysis)
	ON_COMMAND(ID_ANALYSIS_DATALISTING, &CanalyzerView::OnAnalysisDatalisting)
END_MESSAGE_MAP()

// CanalyzerView construction/destruction

CanalyzerView::CanalyzerView()
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

void CanalyzerView::OnDraw(CDC* /*pDC*/)
{
	CanalyzerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
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


void CanalyzerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

		//CClientDC pDC(this);
		//CRect windrect;
		//this->GetWindowRect(&windrect);
		//int nWidth=windrect.Width();
		//int nHeight=windrect.Height();

		//CDC MemDC; //���ȶ���һ����ʾ�豸����
		//CBitmap MemBitmap;//����һ��λͼ����

		////���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
		//MemDC.CreateCompatibleDC(NULL);
		////��ʱ�����ܻ�ͼ����Ϊû�еط��� ^_^
		////���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С

		//MemBitmap.CreateCompatibleBitmap(&pDC,nWidth,nHeight);
		////��λͼѡ�뵽�ڴ���ʾ�豸��
		////ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
		//CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

		////���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ����
		////��Ҳ�������Լ�Ӧ���õ���ɫ
		//MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

		//		//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
		//pDC.BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		////pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		////��ͼ��ɺ������
		//MemBitmap.DeleteObject();
		//MemDC.DeleteDC();
}


BOOL CanalyzerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CanalyzerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	//CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd; 
	////if(	pMain->GetOutputWnd()->GetSafeHwnd())
	//	pMain->GetOutputWnd()->SetWindowPos(NULL, 0, cy*2/3, cx, cy/3, SWP_SHOWWINDOW);
}


void CanalyzerView::OnAnalysisAbortanalysis()
{
	// TODO: Add your command handler code here

	//this->GetMenu()->GetSubMenu(3)->EnableMenuItem(ID_OPTIONS_PLOTSETTINGS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);


}


void CanalyzerView::OnAnalysisDatalisting()
{
	// TODO: Add your command handler code here

	 //AfxMessageBox(L"BTN CLICKED!");
	//this->GetMenu()->GetSubMenu(3)->EnableMenuItem(ID_OPTIONS_PLOTSETTINGS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

}


BOOL CanalyzerView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	//switch(wParam)
 //{
 //case ID_ANALYSIS_DATALISTING:
 //AfxMessageBox(L"BTN CLICKED!");
 //GetMenu()->GetSubMenu(0)->EnableMenuItem(1,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
 //}



	return CView::OnCommand(wParam, lParam);
}
