
// v2Doc.cpp : implementation of the Cv2Doc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "v2.h"
#endif

#include "v2Doc.h"

#include "OpenImageDlg.h"

#include <propkey.h>
#include "v2View.h"

#include "opencv2/opencv.hpp"

//#include "hetu.h"
//#include "engine.h"

//本文来自: 高校自动化网(Www.zdh1909.com) 详细出处参考(转载请保留本链接)：http://www.zdh1909.com/html/matlab/15433.html

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cv2Doc

IMPLEMENT_DYNCREATE(Cv2Doc, CDocument)

	BEGIN_MESSAGE_MAP(Cv2Doc, CDocument)
		//	ON_COMMAND(ID_EDIT_OPEN1, &Cv2Doc::OnEditOpen1)
		//	ON_COMMAND(ID_EDIT_ADD, &Cv2Doc::OnEditAdd)
		ON_COMMAND(ID_EDIT_HETU, &Cv2Doc::OnEditHetu)
	END_MESSAGE_MAP()


	// Cv2Doc construction/destruction

	Cv2Doc::Cv2Doc()
		: idx(-1)
		, imgfp0(_T("C:\\Users\\r8anw2x\\Documents\\MATLAB\\New Folder (4)\\touying\\Photo-0001.jpg"))
		, imgfp1(_T("C:\\Users\\r8anw2x\\Documents\\MATLAB\\New Folder (4)\\touying\\Photo-0002.jpg"))
	{
		// TODO: add one-time construction code here

	}

	Cv2Doc::~Cv2Doc()
	{
	}

	BOOL Cv2Doc::OnNewDocument()
	{
		if (!CDocument::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		OpenImageDlg oidlg;

		oidlg.m_fp1=imgfp0;
		oidlg.m_fp2=imgfp1;

		if(oidlg.DoModal()==IDOK){
			imgfp0=oidlg.m_fp1;
			imgfp1=oidlg.m_fp2;
			return TRUE;
		}



		return FALSE;

	}




	// Cv2Doc serialization

	void Cv2Doc::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
			// TODO: add storing code here
		}
		else
		{
			// TODO: add loading code here
		}
	}

#ifdef SHARED_HANDLERS

	// Support for thumbnails
	void Cv2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
	{
		// Modify this code to draw the document's data
		dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

		CString strText = _T("TODO: implement thumbnail drawing here");
		LOGFONT lf;

		CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
		pDefaultGUIFont->GetLogFont(&lf);
		lf.lfHeight = 36;

		CFont fontDraw;
		fontDraw.CreateFontIndirect(&lf);

		CFont* pOldFont = dc.SelectObject(&fontDraw);
		dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
		dc.SelectObject(pOldFont);
	}

	// Support for Search Handlers
	void Cv2Doc::InitializeSearchContent()
	{
		CString strSearchContent;
		// Set search contents from document's data. 
		// The content parts should be separated by ";"

		// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
		SetSearchContent(strSearchContent);
	}

	void Cv2Doc::SetSearchContent(const CString& value)
	{
		if (value.IsEmpty())
		{
			RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
		}
		else
		{
			CMFCFilterChunkValueImpl *pChunk = NULL;
			ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
			if (pChunk != NULL)
			{
				pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
				SetChunkValue(pChunk);
			}
		}
	}

#endif // SHARED_HANDLERS

	// Cv2Doc diagnostics

#ifdef _DEBUG
	void Cv2Doc::AssertValid() const
	{
		CDocument::AssertValid();
	}

	void Cv2Doc::Dump(CDumpContext& dc) const
	{
		CDocument::Dump(dc);
	}
#endif //_DEBUG


	// Cv2Doc commands


	CPoint & Cv2Doc::SelectPoint(int lri)
	{
		if(idx>=0 && idx<rl.size()){
			switch(lri){
			case 0:
				return rl[idx].TopLeft();
			case 1:
				return rl[idx].BottomRight();
			default:
				break;
			}
		}

		return CPoint(0,0);		

	}



	int Cv2Doc::AddRect(const CRect & rect)
	{

		rl.push_back(rect);

		return SetSelectIndex(rl.size()-1);
	}


	int Cv2Doc::DelRect(int di)
	{

		if(di>=0 && di<rl.size()){
			rl.erase(rl.begin()+di);
			if(idx>rl.size()){
				return SetSelectIndex(rl.size()-1);
			}
			else{
				this->UpdateAllViews(NULL);
			}

		}

		return idx;
	}




	int Cv2Doc::SetSelectIndex(int i)
	{
		if(i>=0 && i<rl.size()){
			idx=i;
			this->UpdateAllViews(NULL);
		}
		return idx;
	}

	//template <typename T>
	//void printmata(cv::Mat h, const int & cha)
	//{
	//	for(int i=0;i<cha;i++){
	//		TRACE(L"\nchannel %d:\n",i);
	//	for(int j=0;j<h.rows;j++){
	//	for(int k=0;k<h.cols;k++){
	//		T a=h.at< cv::Vec<T, cha> >(j,k)[i];
	//		std::wostringstream optlist;
	//		optlist.str(L"");
	//		optlist<<a<<L",";
	//		TRACE(optlist.str());
	//	}
	//	TRACE(L"\n");
	//}
	//	}
	//
	//}
	//
	//void printmat(cv::Mat h){
	//
	//	int de=h.depth();
	//int dee=CV_64F;
	//
	//const int cha=h.channels();
	//
	//switch(de){
	//case CV_8U:
	//case CV_8S:
	//	printmata<uchar>(h,cha);
	//	break;
	//case CV_16U:
	//case CV_16S:
	//	printmata<short>(h,cha);
	//	break;
	//case CV_32S:
	//	printmata<int>(h,cha);
	//	break;
	//case CV_32F:
	//	printmata<float>(h,cha);
	//	break;
	//case CV_64F:
	//	printmata<double>(h,cha);
	//	break;
	//default:
	//	break;
	//}
	//
	//
	//}


	void Cv2Doc::OnEditHetu()
	{

		cv::Mat srcPoints(rl.size(),1,CV_32FC2);
		cv::Mat dstPoints(rl.size(),1,CV_32FC2);

		for(size_t i=0;i<rl.size();i++){
			dstPoints.at<cv::Vec2f>(i,0)[0]=rl[i].left;
			dstPoints.at<cv::Vec2f>(i,0)[1]=rl[i].top;
			srcPoints.at<cv::Vec2f>(i,0)[0]=rl[i].right;
			srcPoints.at<cv::Vec2f>(i,0)[1]=rl[i].bottom;
		}


		cv::Mat h=cv::findHomography(srcPoints, dstPoints, 0);

		
		//for(int j=0;j<h.rows;j++){
		//	for(int k=0;k<h.cols;k++){
		//		double a=h.at<cv::Vec<double,1>>(j,k)[0];
		//		TRACE(L"%g,",a);
		//	}
		//	TRACE(L"\n");
		//}

		POSITION pos = GetFirstViewPosition();
		Cv2View* lv=(Cv2View*)GetNextView(pos);
		Cv2View* rv=(Cv2View*)GetNextView(pos);

		cv::Mat osz(4,1,CV_64FC2,cv::Scalar(0,0));
		osz.at<cv::Vec2d>(2,0)[1]=rv->imgsz.cy-1;
		osz.at<cv::Vec2d>(3,0)[1]=rv->imgsz.cy-1;
		osz.at<cv::Vec2d>(1,0)[0]=rv->imgsz.cx-1;
		osz.at<cv::Vec2d>(3,0)[0]=rv->imgsz.cx-1;
		
		cv::Mat nsz(4,1,CV_64FC2,cv::Scalar(0,0));

		cv::perspectiveTransform(osz,nsz,h);

		cv::Vec2d pt=nsz.at<cv::Vec2d>(0,0);

		cv::Vec2d sz=pt;

		if(nsz.at<cv::Vec2d>(1,0)[0]<pt[0])
			pt[0]=nsz.at<cv::Vec2d>(1,0)[0];
		if(nsz.at<cv::Vec2d>(1,0)[0]>sz[0])
			sz[0]=nsz.at<cv::Vec2d>(1,0)[0];
		if(nsz.at<cv::Vec2d>(1,0)[1]<pt[1])
			pt[1]=nsz.at<cv::Vec2d>(1,0)[1];
		if(nsz.at<cv::Vec2d>(1,0)[1]>sz[1])
			sz[1]=nsz.at<cv::Vec2d>(1,0)[1];

		if(nsz.at<cv::Vec2d>(2,0)[0]<pt[0])
			pt[0]=nsz.at<cv::Vec2d>(2,0)[0];
		if(nsz.at<cv::Vec2d>(2,0)[0]>sz[0])
			sz[0]=nsz.at<cv::Vec2d>(2,0)[0];
		if(nsz.at<cv::Vec2d>(2,0)[1]<pt[1])
			pt[1]=nsz.at<cv::Vec2d>(2,0)[1];
		if(nsz.at<cv::Vec2d>(2,0)[1]>sz[1])
			sz[1]=nsz.at<cv::Vec2d>(2,0)[1];

		if(nsz.at<cv::Vec2d>(3,0)[0]<pt[0])
			pt[0]=nsz.at<cv::Vec2d>(3,0)[0];
		if(nsz.at<cv::Vec2d>(3,0)[0]>sz[0])
			sz[0]=nsz.at<cv::Vec2d>(3,0)[0];
		if(nsz.at<cv::Vec2d>(3,0)[1]<pt[1])
			pt[1]=nsz.at<cv::Vec2d>(3,0)[1];
		if(nsz.at<cv::Vec2d>(3,0)[1]>sz[1])
			sz[1]=nsz.at<cv::Vec2d>(3,0)[1];


		CRect nrect;
		nrect.left=floor(pt[0]);
		nrect.top=floor(pt[1]);
		nrect.right=ceil(sz[0]);
		nrect.bottom=ceil(sz[1]);

		char * buffer0=new char[imgfp0.GetLength()+1]; 
		wcstombs(buffer0,imgfp0.GetBuffer(),imgfp0.GetLength()+1);


		char * buffer1=new char[imgfp1.GetLength()+1]; 
		wcstombs(buffer1,imgfp1.GetBuffer(),imgfp1.GetLength()+1);

		cv::Mat srcimg1=cv::imread(buffer0);
		cv::Mat srcimg=cv::imread(buffer1);

		cv::Mat dstimg;

		cv::warpPerspective(srcimg,
			dstimg,
			h,
			cv::Size(srcimg1.cols>nrect.right?srcimg1.cols:nrect.right,srcimg1.rows>nrect.bottom?srcimg1.rows:nrect.bottom)
			//,cv::INTER_LINEAR|cv::WARP_INVERSE_MAP
			);

		cv::imwrite("pi.bmp",dstimg);

		cv::Mat tempRoi(dstimg,cv::Rect(0,0,srcimg1.cols,srcimg1.rows));


		
		//srcimg1.copyTo(tempRoi);

		if(srcimg1.rows<dstimg.rows)
			srcimg1.resize(dstimg.rows,cv::Scalar());

		if(srcimg1.cols<dstimg.cols){
			srcimg1=srcimg1.t();
			srcimg1.resize(dstimg.cols,cv::Scalar());
			srcimg1=srcimg1.t();
		}

		cv::Mat dstimg1;

		cv::addWeighted(srcimg1,0.5,dstimg,0.5,0,dstimg1);

		cv::namedWindow("res",CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO );
		cv::imshow("res",dstimg1);

		cv::imwrite("pi1.bmp",dstimg1);

	}
