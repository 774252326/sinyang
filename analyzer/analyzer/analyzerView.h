
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

public:
	static bool IsVMSStep(DataOutA d) {
		return d.stepFilter&DOA_VMS;
	};

	static UINT RawData2PlotDataList(const RawData &raw, 
		const std::vector<DataOutA> dol, 
		COLORREF bkc,
		std::vector<PlotDataEx> &pdl,
		BOOL btwo=FALSE,
		COLORREF newClr=RGB(255,0,0),
		COLORREF oldClr=RGB(80,80,100))
	{
		if(dol.empty()){
			pdl.clear();
			return 1;
		}

		std::vector<DataOutA> doltmp;
		doltmp.assign(dol.begin(),dol.end());

		std::vector<DataOutA>::iterator it0 = doltmp.begin();
		std::vector<DataOutA>::iterator it1 = (it0+1);

		size_t pi0=0;
		size_t pn0=0;

		while(it0!=doltmp.end()){

			if(pi0>=pdl.size()){
				PlotSpec ps;
				//ps.winbkC=bkc;
				//ps.SetCr();
				ps.SetPlotBKCr();
				ps.RefreshWinCr(bkc);

				ps.xlabel.LoadStringW(IDS_STRING_POTENTIAL);
				ps.ylabel.LoadStringW(IDS_STRING_CURRENT);

				pdl.push_back(PlotDataEx(ps));
			}
			else{
				pdl[pi0].pd.ps.xlabel.LoadStringW(IDS_STRING_POTENTIAL);
				pdl[pi0].pd.ps.ylabel.LoadStringW(IDS_STRING_CURRENT);
			}
			///////////////////////////////////////////////
			size_t n0=it0 - doltmp.begin();

			it1=std::find_if (it0+1, doltmp.end(), IsVMSStep);

			size_t n1=it1 - doltmp.begin();
			///////////////////////////////////////////////////////////////
			RawData rawtmp;
			rawtmp.ll.assign(raw.ll.begin()+n0,raw.ll.begin()+n1);
			size_t pn=rawtmp.ValidPointNumber();

			rawtmp.xll.assign(raw.xll.begin()+pn0,raw.xll.begin()+pn0+pn);
			rawtmp.yll.assign(raw.yll.begin()+pn0,raw.yll.begin()+pn0+pn);
			///////////////////////////////////////////////////////////////////
			std::vector<CString> namelist;
			for(size_t i=n0;i<n1;i++){
				namelist.push_back(doltmp[i].GetStepName(i));
			}
			////////////////////////////////////////////////////////////////////////
			COLORREF newCr;
			COLORREF oldCr;
			if(btwo){
				oldCr=pdl[pi0].pd.GetOldCr(oldClr);
				newCr=pdl[pi0].pd.GetNewCr(newClr);
			}
			int insertN=pdl[pi0].pd.SetLineData(rawtmp,namelist);

			insertN=pdl[pi0].pd.ls.size();

			if(btwo){
				pdl[pi0].pd.SetLineColor(insertN,newCr,oldCr);
			}
			else{
				pdl[pi0].pd.SetLineColor(insertN);
			}
			/////////////////////////////////////////////////////////////
			pi0++;
			pn0+=pn;

			it0=it1;
		}


		pdl.resize(pi0);

		if(!pdl.empty()){
			pdl.back().pd.ShowLastLast();
		}

		return 0;


	};



public:

	static UINT DataOutAList2PlotDataExList(
		const std::vector<DataOutA> &dol, 
		const ANParaEx &p1, 
		COLORREF bkc, 
		std::vector<PlotDataEx> &pdl,
		bool bShowRs=false,
		bool bReadCb=true)
	{
		//std::vector<DWORD> sl;
		//bool flg=CanalyzerDoc::GetStepList(sl,p1.analysistype);
		bool bShowCb=false;

		//std::vector<PlotDataEx> cbpdl;
		//std::vector<CString> cbnamelist;

		std::vector<RawData> cbrdl;
		std::vector<CString> cbxlabellist;

		std::vector<CString> cbylabellist;
		//std::vector<size_t> cbdolastidx;

		std::vector< std::vector<CString> > cbnlist;


		if(bReadCb){
			switch(p1.analysistype){
			case 2:
				{
					switch(p1.calibrationfactortype){
					case 1:
						{
							DocDataEx ddex0;
							if(ddex0.Read(p1.calibrationfilepath)){
								UINT ff1=ddex0.ComputeStateData();
								if(ff1==0||ff1==7)
									bShowCb=DocDataEx::GetResultData(ddex0.dol,ddex0.p1,cbrdl,cbxlabellist,cbylabellist,cbnlist,true);
							}
						}
						break;
					default:
						bReadCb=false;
						break;
					}

				}
				break;

			case 6:
			case 10:
			case 12:
				{
					DocDataEx ddex0;
					if(ddex0.Read(p1.calibrationfilepath)){
						UINT ff1=ddex0.ComputeStateData();
						if(ff1==0||ff1==7)
							bShowCb=DocDataEx::GetResultData(ddex0.dol,ddex0.p1,cbrdl,cbxlabellist,cbylabellist,cbnlist,true);
						//bShowCb=false;
					}
				}
				break;
			default:
				bReadCb=false;
				break;
			}
		}


		if(bShowCb!=bReadCb)
			return 2;


		{


			std::vector<RawData> rdl;
			std::vector<CString> xlabellist;

			std::vector<CString> ylabellist;


			std::vector< std::vector<CString> > nlist;
			UINT ff;
			if(bShowRs){
				ff=DocDataEx::GetResultData(dol,p1,rdl,xlabellist,ylabellist,nlist);
			}
			else{
				std::vector<size_t> dolastidx;
				ff=DocDataEx::DataOutAList2RawDataList(dol,p1.analysistype,rdl,xlabellist,ylabellist,dolastidx);
				CString str;
				str.LoadStringW(IDS_STRING_TEST_CURVE);
				nlist.resize(rdl.size());
				for(size_t i=0;i<nlist.size();i++){
					nlist[i].assign(rdl[i].ll.size(),str);
				}
			}

			PlotSpec ps;
			ps.SetPlotBKCr();
			ps.RefreshWinCr(bkc);






			for(size_t i=0;i<rdl.size();i++){
				if(i>=pdl.size()){
					pdl.push_back(PlotDataEx(ps));
				}

				pdl[i].pd.ps.xlabel=xlabellist[i];
				pdl[i].pd.ps.ylabel=ylabellist[i];


				//std::vector<CString> namelist(rdl[i].ll.size(),str);

				if( bShowCb && i<cbrdl.size() ){
					//pdl[i].pd.raw=cbrdl[i];
					//pdl[i].pd.ls.assign(cbpdl.back().pd.ls.begin(),cbpdl.back().pd.ls.end());
					int insertN0=pdl[i].pd.SetLineData(cbrdl[i],cbnlist[i]);
					pdl[i].pd.SetLineColor(insertN0,3,1);
					int insertN=pdl[i].pd.AddLineData(rdl[i],nlist[i]);
					pdl[i].pd.SetLineColor(insertN,3,1);

					pdl[i].pd.ResetLineType();
				}			
				else{
					//int insertN=pdl[i].pd.SetLineData(rdl[i],namelist);
					int insertN=pdl[i].pd.SetLineData(rdl[i],nlist[i]);
					pdl[i].pd.SetLineColor(insertN,3,1);

					if(bShowRs){
						pdl[i].pd.ResetLineType();
					}

				}

			}

			pdl.resize(rdl.size());



			return 1;
		}

		return 0;



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
	BOOL b2;
	COLORREF newClr;
	COLORREF oldClr;
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

	afx_msg LRESULT OnMessageUpdateRaw(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageUpdateTest(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()


public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

};

#ifndef _DEBUG  // debug version in analyzerView.cpp
inline CanalyzerDoc* CanalyzerView::GetDocument() const
{ return reinterpret_cast<CanalyzerDoc*>(m_pDocument); }
#endif

