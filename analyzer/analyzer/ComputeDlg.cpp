// ComputeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "analyzer.h"
#include "ComputeDlg.h"
#include "afxdialogex.h"
#include "analyzerDoc.h"
//#include "filefunc.h"
#include "MainFrm.h"
#include "analyzerViewR.h"
#include "user\StartDlg.hpp"

//void ScaleBitmap(CBitmap *pSrc, CBitmap *pDst, int dstW, int dstH);
//bool FitLine(std::vector<double> &x, std::vector<double> &y, LineSeg &ls, int nFront=0, int nBack=0);

// ComputeDlg dialog

IMPLEMENT_DYNAMIC(ComputeDlg, CDialogEx)

	ComputeDlg::ComputeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ComputeDlg::IDD, pParent)
	//, fd(NULL)
{

}

ComputeDlg::~ComputeDlg()
{
}

void ComputeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MFCBUTTON1, m_btn1);
	//DDX_Control(pDX, IDC_MFCBUTTON2, m_btn2);
}


BEGIN_MESSAGE_MAP(ComputeDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDS_BTN_COMPUTE, &ComputeDlg::ClickCompute)
	//ON_BN_CLICKED(IDS_CHECK_SHOW_FORMULA, &ComputeDlg::Showfml)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ComputeDlg message handlers


int ComputeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here




	return 0;
}



BOOL ComputeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here





	CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
	//CMDIChildWnd* pchild = pframe->MDIGetActive();
	CanalyzerDoc* doc = (CanalyzerDoc*) pframe->GetActiveDocument();
	//doc->m_Color = m_Color;

	// Tell the view to paint with the new selected color.
	//CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());


	CRect rc;
	pframe->GetWindowRect(&rc);

	CSize winSize(400,500);

	CSize gap1(20,20);
	CSize gap2(20,20);

	int editH=50;

	int offset=30;

	CPoint pt(gap1);
	int w=winSize.cx-2*gap1.cx;
	//CSize btnSz((w-gap2.cx)/2,btnH);

	CString str;

	CButton *pBtn;

	str.LoadStringW(IDS_STRING_RESULT);
	this->SetWindowTextW(str);



	CEdit *pEdit;

	//pEdit=new CEdit;
	EditEDRS_ID.CreateEx(
		NULL,
		//WS_EX_CLIENTEDGE,
		L"Edit", 
		L"",
		ES_LEFT
		|ES_READONLY
		|ES_MULTILINE
		//|ES_FLAT
		//|WS_TILED   
		|WS_CHILD
		|WS_VISIBLE,
		CRect(pt,CSize(w,editH)),
		this,
		EDRS_ID);

	pt.y+=editH;

	winSize.cy=offset+editH+gap1.cy*2;

	if(!(doc->da.GetVL(m_list.vl))){
		return FALSE;
	}

	if( ((CMainFrame*)pframe)->GetCurAuth()==UserAccount::authority::admin )
		switch(doc->da.p1.analysistype){
		case 2:
		case 4:
		case 6:
		case 8:
		case 10:
		case 12:
			{


				formulaImg.LoadBitmapW(GetFormulaImgID());
				BITMAP bm;
				formulaImg.GetBitmap(&bm);
				int imgH=winSize.cx*bm.bmHeight/bm.bmWidth;

				imgrc=CRect(pt,CSize(w,imgH));

				int btnH=22;
				int listH=(m_list.rowH+2)*m_list.vl.size()+24;

				winSize.cy+=imgH+gap2.cy*2+listH+btnH;


				//::SetWindowPos(this->GetSafeHwnd(),
				//	HWND_TOP,
				//	rc.CenterPoint().x-winSize.cx/2,
				//	rc.CenterPoint().y-winSize.cy/2,
				//	winSize.cx,
				//	winSize.cy,		
				//	SWP_SHOWWINDOW);


				pt.y+=gap2.cy+imgH;


				const DWORD dwStyle = WS_VISIBLE 
					| WS_CHILD 
					| WS_BORDER
					| WS_HSCROLL 
					| WS_VSCROLL 
					| LBS_NOINTEGRALHEIGHT;

				if(!m_list.Create(dwStyle, CRect(pt,CSize(w,listH)), this, 1256) ){
					TRACE0("Failed to create output windows\n");
					return -1;      // fail to create
				}

				pt.y+=gap2.cy+listH;


				m_list.Refresh();


				str.LoadStringW(IDS_BTN_COMPUTE);
				//pBtn=new CButton;
				BtnIDS_BTN_COMPUTE.Create(str,
					BS_PUSHBUTTON
					|WS_CHILD
					|WS_VISIBLE,
					CRect(pt,CSize(w,btnH)),
					this,
					IDS_BTN_COMPUTE);

				break;


			}
		default:

			break;
	}


	::SetWindowPos(this->GetSafeHwnd(),
		HWND_TOP,
		rc.CenterPoint().x-winSize.cx/2,
		rc.CenterPoint().y-winSize.cy/2,
		winSize.cx,
		winSize.cy,		
		SWP_SHOWWINDOW);


	ClickCompute();

	return TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

}


//void ComputeDlg::OnBnClickedMfcbutton1()
//{
//	// TODO: Add your control notification handler code here
//	
//	CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
//	//CMDIChildWnd* pchild = pframe->MDIGetActive();
//	CanalyzerDoc* doc = (CanalyzerDoc*) pframe->GetActiveDocument();
//
//	// Tell the view to paint with the new selected color.
//	//CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());
//
//	CanalyzerViewR *rv=(CanalyzerViewR*)(((CMainFrame*)pframe)->RightPane());
//
//	std::vector<CString> namel;
//	std::vector<CString> valuel;
//
//	if(DocDataEx::GetResultString(&(doc->da),m_list.vl,namel,valuel)){
//		CString str=L"";
//		for(size_t i=0;i<namel.size();i++){
//			str+=namel[i]+L" "+valuel[i]+L"\r\n";
//		}
//		((CEdit*)GetDlgItem(EDRS_ID))->SetWindowTextW(str);
//
//		for(size_t i=0;i<m_list.vl.size() && i<doc->da.p1.correction.size(); i++){
//			doc->da.p1.correction[i]=m_list.vl[i].correction;
//		}
//
//		::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,(PW_LAST|PW_SHOW_ALL),(LPARAM)true);
//
//	}
//}



void ComputeDlg::ClickCompute(void)
{

	CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
	//CMDIChildWnd* pchild = pframe->MDIGetActive();
	CanalyzerDoc* doc = (CanalyzerDoc*) pframe->GetActiveDocument();

	// Tell the view to paint with the new selected color.
	//CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());

	CanalyzerViewR *rv=(CanalyzerViewR*)(((CMainFrame*)pframe)->RightPane());

	std::vector<CString> namel;
	std::vector<CString> valuel;

	if(DocDataEx::GetResultString(&(doc->da),m_list.vl,namel,valuel)){
		CString str=L"";
		for(size_t i=0;i<namel.size();i++){
			str+=namel[i]+L" "+valuel[i]+L"\r\n";
		}
		//((CEdit*)GetDlgItem(EDRS_ID))->SetWindowTextW(str);
		EditEDRS_ID.SetWindowTextW(str);
		for(size_t i=0;i<m_list.vl.size() && i<doc->da.p1.correction.size(); i++){
			doc->da.p1.correction[i]=m_list.vl[i].correction;
		}

		::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,(PW_LAST|PW_SHOW_ALL),(LPARAM)true);

	}

}


void ComputeDlg::Showfml(void)
{
	//int bChk=((CButton*)GetDlgItem(IDS_CHECK_SHOW_FORMULA))->GetCheck();

	//switch(bChk){
	//case BST_UNCHECKED:
	//	{
	//		if(fd!=NULL){
	//			fd->ShowWindow(SW_HIDE);
	//			delete fd;
	//			fd=NULL;
	//		}
	//	}
	//	break;
	//case BST_CHECKED:
	//	{
	//		if(fd==NULL){
	//			fd=new StartDlg(GetFormulaImgID());
	//			fd->Create(StartDlg::IDD);
	//		}
	//		fd->ShowWindow(SW_SHOW);
	//	}
	//	break;
	//default:
	//	break;
	//}

}


void ComputeDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//if(fd!=NULL){
	//	fd->ShowWindow(SW_HIDE);
	//	delete fd;
	//	fd=NULL;
	//}

	CDialogEx::OnCancel();
}


UINT ComputeDlg::GetFormulaImgID(void)
{


	CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
	//CMDIChildWnd* pchild = pframe->MDIGetActive();
	CanalyzerDoc* doc = (CanalyzerDoc*)(pframe->GetActiveDocument());

	switch(doc->da.p1.analysistype){
	case 2:
		return IDB_BITMAP14;
	case 4:
		return IDB_BITMAP15;
	case 6:
		return IDB_BITMAP18;
	case 8:
		return IDB_BITMAP19;
	case 10:
		return IDB_BITMAP17;
	case 12:
		return IDB_BITMAP16;
	default:
		return IDB_BITMAP13;
	}

	return 0;
}


void ComputeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	if(formulaImg.GetSafeHandle()!=NULL){
		CDC srcDC;
		srcDC.CreateCompatibleDC(&dc); 
		srcDC.SelectObject(&formulaImg);

		BITMAP bm;
		formulaImg.GetBitmap(&bm); 

		dc.StretchBlt(imgrc.left,imgrc.top,imgrc.Size().cx,imgrc.Size().cy,&srcDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	}
}
