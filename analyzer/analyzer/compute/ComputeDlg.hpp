
#pragma once

#include "ParaList.hpp"

#include "afxdialogex.h"
#include "../analyzerDoc.h"
#include "../MainFrm.h"
#include "../analyzerView.h"

#include "../messagemaphpp.h"

// ComputeDlg dialog
///
/// \brief The ComputeDlg class
///计算结果对话框
class ComputeDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(ComputeDlg)

protected:
    ///
    /// \brief m_list
    /// 输入参数的可编辑表格
	ParaList m_list;
    ///
    /// \brief EditEDRS_ID
    ///用于显示结果
	CEdit EditEDRS_ID;
    ///
    /// \brief formulaImg
    ///计算公式的位图
	CBitmap formulaImg;
    ///
    /// \brief BtnIDS_BTN_COMPUTE
    ///计算按钮
	CButton BtnIDS_BTN_COMPUTE;
    ///
    /// \brief imgrc
    ///公式位图的位置和大小
	CRect imgrc;

public:
	ComputeDlg(CWnd* pParent = NULL)
		: CDialogEx(ComputeDlg::IDD, pParent)
	{

	};   // standard constructor
	virtual ~ComputeDlg(){};

	// Dialog Data
	enum { IDD = IDD_DIALOG_COMPUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	};    // DDX/DDV support

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CDialogEx::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here
		return 0;
	};

    ///
    /// \brief ClickCompute
    ///计算结果并刷新显示

	void ClickCompute(void)
	{

		CFrameWndEx* pframe = (CFrameWndEx*) AfxGetMainWnd();
		//CMDIChildWnd* pchild = pframe->MDIGetActive();
		CanalyzerDoc* doc = (CanalyzerDoc*) pframe->GetActiveDocument();

		// Tell the view to paint with the new selected color.
		//CanalyzerView* view = (CanalyzerView*)(pframe->GetActiveView());

		//CanalyzerViewR *rv=(CanalyzerViewR*)(((CMainFrame*)pframe)->RightPane());

		std::vector<CString> namel;
		std::vector<CString> valuel;

		CString str=L"";
		if(DocDataEx::GetResultString(&(doc->da),m_list.vl,namel,valuel)){			
			for(size_t i=0;i<namel.size();i++){
				str+=namel[i]+L" "+valuel[i]+L"\r\n";
			}

			for(size_t i=0;i<m_list.vl.size() && i<doc->da.p1.correction.size(); i++){
				doc->da.p1.correction[i]=m_list.vl[i].correction;
			}
			if(!(doc->da.p1.correction.empty()))
				doc->SetModifiedFlag();

			::PostMessage((((CMainFrame*)pframe)->RightPane())->GetSafeHwnd(),MESSAGE_UPDATE_TEST,PW_INIT,(LPARAM)true);

		}
		else{
			str.LoadStringW(IDS_STRING_ERROR);
		}

						//((CEdit*)GetDlgItem(EDRS_ID))->SetWindowTextW(str);
			EditEDRS_ID.SetWindowTextW(str);
	};

    ///
    /// \brief OnInitDialog
    /// 布局对话框
    /// 计算并显示结果
    ///
    /// \return
    ///
	virtual BOOL OnInitDialog()
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
		int btnH=22;
		int offset=30;

		CPoint pt(gap1);
		int w=winSize.cx-2*gap1.cx;

		CString str;

		str.LoadStringW(IDS_STRING_RESULT);
		this->SetWindowTextW(str);

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

		if( (doc->da.GetVL(m_list.vl)) && 
			((CMainFrame*)pframe)->GetCurAuth()==UserAccount::authority::admin ){
				switch(doc->da.p1.analysistype){
				case 2:
				case 4:
				case 6:
				case 8:
				case 10:
				case 12:
					{


						formulaImg.LoadBitmapW(doc->da.p1.GetFormulaImgID());
						BITMAP bm;
						formulaImg.GetBitmap(&bm);
						int imgH=winSize.cx*bm.bmHeight/bm.bmWidth;

						imgrc=CRect(pt,CSize(w,imgH));

						int listgapH=2;
						int listheaderH=24;
						int listH=(m_list.rowH+listgapH)*m_list.vl.size()+listheaderH;

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

						pt.y+=btnH;


						break;


					}
				default:

					break;
				}
		}

		pt.y+=gap2.cy;
		winSize.cy+=gap2.cy;
		GetDlgItem(IDCANCEL)->MoveWindow( CRect(pt,CSize(w,btnH)) );
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
		str.LoadStringW(IDS_STRING_CLOSE1);
		GetDlgItem(IDCANCEL)->SetWindowTextW(str);
		winSize.cy+=btnH;

		::SetWindowPos(this->GetSafeHwnd(),
			HWND_TOP,
			rc.CenterPoint().x-winSize.cx/2,
			rc.CenterPoint().y-winSize.cy/2,
			winSize.cx,
			winSize.cy,		
			SWP_SHOWWINDOW);


		ClickCompute();


		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE

	};


	virtual void OnCancel()
	{
		// TODO: Add your specialized code here and/or call the base class

		//if(fd!=NULL){
		//	fd->ShowWindow(SW_HIDE);
		//	delete fd;
		//	fd=NULL;
		//}

		CDialogEx::OnCancel();
	};


	afx_msg void OnPaint()
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
	};

//	DECLARE_MESSAGE_MAP()
//
//};
//
//BEGIN_MESSAGE_MAP(ComputeDlg, CDialogEx)
//	ON_WM_CREATE()
//	ON_BN_CLICKED(IDS_BTN_COMPUTE, &ComputeDlg::ClickCompute)
//	ON_WM_PAINT()
//END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP_HPP(ComputeDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDS_BTN_COMPUTE, &ComputeDlg::ClickCompute)
	ON_WM_PAINT()
END_MESSAGE_MAP_HPP()
};
