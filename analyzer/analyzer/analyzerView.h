
// analyzerView.h : interface of the CanalyzerView class
//

#pragma once
#include "analyzerDoc.h"
#include "plot\PlotWnd.h"
#include <vector>

#define PW_LAST 0x01
#define PW_SHOW_ALL 0x02
#define PW_INIT 0x04

class CanalyzerView : public CView
{
public:
	static void CopyWndToClipboard( CWnd *pWnd )
	{
		CBitmap  bitmap;
		CClientDC dc(pWnd);
		CDC  memDC;
		CRect rect;

		memDC.CreateCompatibleDC(&dc); 

		pWnd->GetWindowRect(rect);

		bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height() );

		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
		memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY); 

		pWnd->OpenClipboard() ;
		EmptyClipboard() ;
		SetClipboardData (CF_BITMAP, bitmap.GetSafeHandle() ) ;
		CloseClipboard () ;

		memDC.SelectObject(pOldBitmap);
		bitmap.Detach();
	};

		 void Print()
	{
		CDC   PrintDC; 
		HDC   hPrintDC; 

		CPrintDialog   prtDlg(FALSE,PD_DISABLEPRINTTOFILE); 
		if   (prtDlg.DoModal()   ==   IDCANCEL)   return   ; 

		hPrintDC   =   prtDlg.GetPrinterDC(); 
		PrintDC.Attach(hPrintDC); 

		CString strTitle=L"";   //   Get   the   application   title 
		DOCINFO di;         //   Initialise   print   document   details  
		//strTitle.Format(_T("%s:%s"),CTextRes::GetInstance()->LoadString(IDS_STRING23592),m_strIndentID);//¥Ú”°∂©µ•


		::ZeroMemory(&di,   sizeof   (DOCINFO)); 
		di.cbSize   =   sizeof   (DOCINFO); 
		di.lpszDocName   =   strTitle; 

		//   Begin   a   new   print   job 
		if   FAILED(PrintDC.StartDoc(&di)   ==   -1)   return; 

		PrintDC.StartPage(); 

		int   nScreenW   =   GetDeviceCaps(*GetDC(),   LOGPIXELSX); 
		int   nScreenH   =   GetDeviceCaps(*GetDC(),   LOGPIXELSY); 
		int   nPrintW   =   GetDeviceCaps(hPrintDC,   LOGPIXELSX); 
		int   nPrintH   =   GetDeviceCaps(hPrintDC,   LOGPIXELSY); 

		float   fRateX   =   nPrintW   /   float(nScreenW)   ; 
		float   fRateY   =   nPrintH   /   float(nScreenH)   ; 

		CBitmap bitmap;
		CClientDC dc(this);
		CDC memdc;
		CRect rect;
		BOOL bRet = FALSE;

		memdc.CreateCompatibleDC(&dc);
		GetClientRect(rect);
		bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height()); 

		CBitmap * oldbitmap = memdc.SelectObject(&bitmap);
		bRet = memdc.BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY); 

		CBitmap * Printbitmap;
		Printbitmap = memdc.SelectObject(oldbitmap); 


		BITMAP       bmp;       
		bitmap.GetBitmap(&bmp);       

		long       nWidth=bmp.bmWidth;       
		long       nHeight=bmp.bmHeight;   

		VOID   *lpBits=new   BYTE[bmp.bmWidth*bmp.bmHeight*4]; 
		BITMAPINFO       bmi; 
		memset(&bmi,0,sizeof(bmi)); 
		bmi.bmiHeader.biSize=sizeof(BITMAPINFO);       
		bmi.bmiHeader.biWidth=bmp.bmWidth;       
		bmi.bmiHeader.biHeight=bmp.bmHeight;       
		bmi.bmiHeader.biPlanes=1;       
		bmi.bmiHeader.biBitCount=bmp.bmBitsPixel;       
		bmi.bmiHeader.biCompression=BI_RGB;       
		bmi.bmiHeader.biClrUsed=0;       

		GetDIBits(memdc.m_hDC,   (HBITMAP)   bitmap.m_hObject,   0,   bmp.bmHeight,   lpBits,     (LPBITMAPINFO)   &bmi,   DIB_RGB_COLORS); 

		int  nDestWidth = nWidth*fRateX; 
		int  nDestHeight = nHeight*fRateY; 
		int  xDest = ( nPrintW * fRateX  -  nDestWidth) * 3 / 4;
		int  yDest = nPrintH * fRateY - nDestHeight ;

		::StretchDIBits(hPrintDC,xDest,yDest,nDestWidth,nDestHeight,0,0,nWidth,nHeight,lpBits,&bmi,DIB_RGB_COLORS,   SRCCOPY)   ;   

		PrintDC.SelectObject(bitmap); 

		PrintDC.EndPage(); 
		PrintDC.EndDoc(); 
		PrintDC.Detach(); 

		delete   lpBits;  
	};

protected: // create from serialization only
	CanalyzerView();
	DECLARE_DYNCREATE(CanalyzerView)

	// Attributes
public:
	CanalyzerDoc* GetDocument() const;

	PlotWnd pw;
	CSpinButtonCtrl m_spBtn;
	CSize spBtnSize;
	std::vector<PlotDataEx> pdl;
	CCriticalSection m_CritSection;
	// Operations
public:

	int UpdateSpinButton(int np, bool bLast=false);
	int UpdateSpinButton(){return UpdateSpinButton(pdl.size());};
	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CanalyzerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
protected:
	afx_msg LRESULT OnMessageChangeApplook(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageUpdateView(WPARAM wParam, LPARAM lParam);
	void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCopy();
	afx_msg void OnViewDatacursor();
	afx_msg void OnViewFitwindow();
	afx_msg void OnOptionsPlotsettings();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAnalysisExportdata();



	DECLARE_MESSAGE_MAP()


public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	bool bPrint;
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	void PlotSettingSheet(BOOL bTwo=FALSE);
};

#ifndef _DEBUG  // debug version in analyzerView.cpp
inline CanalyzerDoc* CanalyzerView::GetDocument() const
{ return reinterpret_cast<CanalyzerDoc*>(m_pDocument); }
#endif

