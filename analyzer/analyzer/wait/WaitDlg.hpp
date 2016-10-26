#pragma once
#include "StaticEdit.hpp"
#include "../resource.h"
#include "afxdialogex.h"
// WaitDlg dialog


#include "../messagemaphpp.h"

////
/// \brief The WaitDlg class
///提示对话框

class WaitDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(WaitDlg)
public:
    ///
    /// \brief m_tips
    ///提示字符串
	CString m_tips;
    ///
    /// \brief EditIDS_EDIT_TIPS
    ///编辑框
    /// 用于显示提示信息
	StaticEdit EditIDS_EDIT_TIPS;
    ///
    /// \brief btnIDOK
    ///继续按钮
	CButton btnIDOK;
	// Dialog Data
	enum { IDD = IDD_DIALOG_WAIT };	
public:
	WaitDlg(CWnd* pParent = NULL)
		: CDialogEx(WaitDlg::IDD, pParent)
	{
		m_tips = _T("");
	};   // standard constructor
	virtual ~WaitDlg(){};

    ///
    /// \brief ShowTitleAndButton
    /// \param bShow
    ///为真
    /// 显示窗口标题和继续按钮
    /// 为假
    /// 清空窗口标题并隐藏继续按钮
	void ShowTitleAndButton(BOOL bShow=TRUE)
	{
		btnIDOK.ShowWindow(bShow);
		CString str=L"";
		if(bShow)
		{
			str.LoadStringW(IDS_STRING_PAUSE);
			btnIDOK.SetFocus();
		}
		this->SetWindowTextW(str);		
	};

protected:

    ///
    /// \brief OnOK
    ///相当于按下继续
	virtual void OnOK()
	{
		// TODO: Add your specialized code here and/or call the base class

		this->GetParentFrame()->SendMessage(WM_COMMAND,ID_ANALYSIS_PAUSE,0);

		//CDialogEx::OnOK();
	};

//    布局按钮
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CDialogEx::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		CRect rc;
		this->GetParentFrame()->GetWindowRect(&rc);

		CSize winSize(400,100);

		::SetWindowPos(this->GetSafeHwnd(),
			HWND_TOPMOST,
			rc.CenterPoint().x-winSize.cx/2,
			rc.CenterPoint().y-winSize.cy/2,
			winSize.cx,
			winSize.cy,		
			SWP_SHOWWINDOW);

		CSize gap1(10,10);

		int btnH=22;
		int btnW=100;


		CPoint pt(gap1);
		//CStatic *pStatic;
		CEdit *pEdit;
		CButton *pButton;
		CString str;

		//str.LoadStringW(IDS_STRING_PAUSE);

		//this->SetWindowTextW(str);
		this->ModifyStyle(WS_SYSMENU,NULL);

		//CFont * pFont = new CFont; 
		EditIDS_EDIT_TIPS.editfont.CreateFont(btnH, // nHeight
			0, // nWidth 
			0, // nEscapement 
			0, // nOrientation 
			FW_BOLD, // nWeight 
			TRUE, // bItalic 
			FALSE, // bUnderline 
			0, // cStrikeOut 
			ANSI_CHARSET, // nCharSet 
			OUT_DEFAULT_PRECIS, // nOutPrecision 
			CLIP_DEFAULT_PRECIS, // nClipPrecision 
			DEFAULT_QUALITY, // nQuality 
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
			_T("Arial")); // lpszFac 

		//pEdit=new CEdit;
		EditIDS_EDIT_TIPS.CreateEx(
			//WS_EX_CLIENTEDGE,
			NULL,
			L"Edit", 
			L"",
			//ES_LEFT
			ES_CENTER
			//|ES_AUTOHSCROLL 
			//|ES_MULTILINE
			|ES_READONLY
			//|WS_DISABLED
			|WS_CHILD
			|WS_VISIBLE,
			CRect(pt,CSize(winSize.cx-2*gap1.cx,btnH)),
			this,
			IDS_EDIT_TIPS);

		pt.y+=btnH+gap1.cy;
		pt.x+=(winSize.cx-btnW)/2-gap1.cx;

		str.LoadStringW(IDS_STRING_RESUME);
		//pButton=new CButton;

		if(btnIDOK.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,CSize(btnW,btnH)), this, IDOK)==FALSE){
			return -1;
		}

		ShowTitleAndButton();

		

		return 0;
	};


	//BOOL PreCreateWindow(CREATESTRUCT& cs)
	//	{
	//		cs.style = cs.style&~WS_MAXIMIZEBOX&~WS_SYSMENU&~WS_MINIMIZEBOX; 
	//if( !CWnd::PreCreateWindow(cs) )
	//	return FALSE;
	//// TODO: Modify the Window class or styles here by modifying
	////  the CREATESTRUCT cs

	//return TRUE;
	//};




	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
		DDX_Text(pDX, IDS_EDIT_TIPS, m_tips);
	};    // DDX/DDV support




	BEGIN_MESSAGE_MAP_HPP(WaitDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_HELPINFO()
	END_MESSAGE_MAP_HPP()




};



