#pragma once
#include "ListCtrlLine.hpp"

// ExportDataDlg dialog

class ExportDataDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(ExportDataDlg)
protected:
	CMFCEditBrowseCtrl m_c;
	CMFCButton m_exp;
	CButton btnSelAll;
public:
	ListCtrlLine lcl;
public:
	ExportDataDlg(CWnd* pParent = NULL)
		: CDialogEx(ExportDataDlg::IDD, pParent)
	{

	};   // standard constructor
	virtual ~ExportDataDlg(){};

	// Dialog Data
	enum { IDD = IDD_DIALOG_EXPORT };

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

		CRect rc;
		this->GetParentFrame()->GetWindowRect(&rc);

		CSize winSize(400,400);



		CSize gap1(20,20);
		CSize gap2(10,10);

		int btnH=22;
		int listH=250;

		int btnW=winSize.cx-2*gap1.cx;

		CPoint pt(gap1);
		CStatic *pStatic;
		CEdit *pEdit;
		CString str;

		str.LoadStringW(IDS_STRING_EXPORT_DLG);
		this->SetWindowTextW(str);

		str.LoadStringW(IDS_EDIT_LABEL_SIZE);
		m_c.CreateEx(
			WS_EX_CLIENTEDGE,
			L"Edit", 
			str,
			//ES_LEFT
			ES_RIGHT
			|ES_AUTOHSCROLL   
			|WS_CHILD
			|WS_VISIBLE,
			CRect(pt,CSize(btnW,btnH)),
			this,
			IDS_EDIT_LABEL_SIZE);

		m_c.EnableFolderBrowseButton();

		pt.y+=gap2.cy+btnH;

		const DWORD dwStyle = WS_VISIBLE 
			| WS_CHILD 
			| WS_BORDER
			//| WS_HSCROLL 
			//| WS_VSCROLL 
			//| LBS_NOINTEGRALHEIGHT
			|LVS_SHOWSELALWAYS 
			|LVS_LIST
			|LVS_EDITLABELS
			;

		if(!lcl.Create(dwStyle, CRect(pt,CSize(btnW,listH)), this, 1444) ){
			TRACE0("Failed to create output windows\n");
			return -1;      // fail to create
		}

		pt.y+=gap2.cy+listH;

		str.LoadStringW(IDS_STRING_SELECT_ALL);
		if(btnSelAll.Create(str, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(pt,CSize(btnW,btnH)), this, IDS_STRING_SELECT_ALL)==FALSE){
			return -1;
		}

		pt.y+=gap2.cy+btnH;

		str.LoadStringW(IDS_STRING_EXPORT);
		if(m_exp.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(pt,CSize(btnW,btnH)), this, IDOK)==FALSE){
			return -1;
		}

		pt.y+=gap1.cy+btnH;

		winSize.cy=pt.y+20;

		::SetWindowPos(this->GetSafeHwnd(),
			HWND_TOPMOST,
			rc.CenterPoint().x-winSize.cx/2,
			rc.CenterPoint().y-winSize.cy/2,
			winSize.cx,
			winSize.cy,		
			SWP_SHOWWINDOW);

		return 0;
	};

	virtual BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		// TODO:  Add extra initialization here

		lcl.SetList();
		TCHAR path[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH,path);
		m_c.SetWindowTextW(path);
		btnSelAll.SetCheck(BST_CHECKED);

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	};

	virtual void OnOK()
	{
		// TODO: Add your specialized code here and/or call the base class

		CString str;

		m_c.GetWindowTextW(str);

		//CString str=ChooseFolderDlg();
		for(int i=0;i<lcl.GetItemCount();i++){
			if(lcl.GetCheck(i)!=FALSE){
				lcl.rde.title[i]=str+L"\\"+lcl.rde.title[i];
				BOOL flg=lcl.rde.SaveToText(i);
				if(flg==FALSE){
					::AfxMessageBox(IDS_STRING_RENAME_TIPS);
					lcl.SetFocus();
					lcl.EnsureVisible(i,FALSE);
					//lcl.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					lcl.EditLabel(i);
					return;
				}
				lcl.SetCheck(i,FALSE);				
			}
		}


		CDialogEx::OnOK();
	};

	void OnCheck()
	{
		int flg=btnSelAll.GetCheck();
		switch(flg){
		case BST_CHECKED:
			for(int i=0;i<lcl.GetItemCount();i++){
				lcl.SetCheck(i);
			}
			break;
		case BST_UNCHECKED:
			for(int i=0;i<lcl.GetItemCount();i++){
				lcl.SetCheck(i,FALSE);
			}
			break;
		default:
			break;
		}
	};

	DECLARE_MESSAGE_MAP()

};

BEGIN_MESSAGE_MAP(ExportDataDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDS_STRING_SELECT_ALL, &ExportDataDlg::OnCheck)
END_MESSAGE_MAP()
