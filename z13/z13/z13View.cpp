
// z13View.cpp : implementation of the Cz13View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "z13.h"
#endif

#include "z13Doc.h"
#include "z13View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cz13View

IMPLEMENT_DYNCREATE(Cz13View, CFormView)

	BEGIN_MESSAGE_MAP(Cz13View, CFormView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_BN_CLICKED(IDC_BUTTON1, &Cz13View::OnBnClickedButton1)
		ON_BN_CLICKED(IDC_BUTTON2, &Cz13View::OnBnClickedButton2)
	END_MESSAGE_MAP()

	// Cz13View construction/destruction

	Cz13View::Cz13View()
		: CFormView(Cz13View::IDD)
		, i(0)
		, ptipdlg(NULL)
	{
		// TODO: add construction code here

	}

	Cz13View::~Cz13View()
	{
	}

	void Cz13View::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_LIST1, m_ListBox);
	}

	BOOL Cz13View::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: Modify the Window class or styles here by modifying
		//  the CREATESTRUCT cs

		return CFormView::PreCreateWindow(cs);
	}

	void Cz13View::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

	}

	void Cz13View::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void Cz13View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// Cz13View diagnostics

#ifdef _DEBUG
	void Cz13View::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void Cz13View::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	Cz13Doc* Cz13View::GetDocument() const // non-debug version is inline
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cz13Doc)));
		return (Cz13Doc*)m_pDocument;
	}
#endif //_DEBUG


	// Cz13View message handlers


	void Cz13View::OnBnClickedButton1()
	{
		// TODO: Add your control notification handler code here

		CString str;
		str=L"C:\\Users\\r8anw2x\\Dropbox\\W\\data\\a.txt";



		CStdioFile file;
		BOOL readflag;
		readflag=file.Open(str, CFile::modeRead);


		if(readflag)
		{	
			CString strRead;

			TRACE("\n--Begin to read file");

			while(file.ReadString(strRead)){
				m_ListBox.AddString(strRead);
			}
			TRACE("\n--End reading\n");
			file.Close();
		}

		//str.Format(L"%d",i++);
		//m_ListBox.AddString(str);
			

	}


	void Cz13View::OnBnClickedButton2()
	{
		// TODO: Add your control notification handler code here

		
		ptipdlg = new tipdlg();   
		ptipdlg->Create(IDD_DIALOG1, this);
		CString str;

		m_ListBox.GetText(m_ListBox.GetCurSel(), str);
		ptipdlg->settext(str);

		ptipdlg->ShowWindow(SW_SHOW);   




	}
