
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

//#include "hetu.h"
#include "engine.h"

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


	//
	//bool res = false;

 //      res = mclInitializeApplication(NULL,0);

 //      


 //      if (!res)

 //      {

 //             AfxMessageBox(L"初始化Application错误！");
	//		  return FALSE;
 //      }

 //      res = hetuInitialize();

 //      if (!res)

 //      {

 //             AfxMessageBox(L"初始化Lib错误！");
	//		  return FALSE;

 //      }


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


void Cv2Doc::OnEditHetu()
{
	// TODO: Add your command handler code here

	//bool res = false;

 //      res = mclInitializeApplication(NULL,0);

 //      

 //      if (!res)

 //      {

 //             AfxMessageBox(L"初始化Application错误！");
	//		  return;
 //      }

 //      res = hetuInitialize();

 //      if (!res)

 //      {

 //             AfxMessageBox(L"初始化Lib错误！");
	//		  return;

 //      }
//
//	   //std::vector<double> x1;
//	   std::vector<double> x2;
//	   std::vector<double> y1;
//	   std::vector<double> y2;
//
//	   double *x1=new double[rl.size()];
//
//	   for(size_t i=0;i<rl.size();i++){
//		   //x1.push_back(rl[i].left);
//		   x1[i]=rl[i].left;
//		   y1.push_back(rl[i].top);
//		   x2.push_back(rl[i].right);
//		   y2.push_back(rl[i].bottom);
//	   }
//
//	   int ll=rl.size();
//	   mwSize lll=ll;
//
//	   mwArray hh;
//
//mwArray fpo("he.bmp");
//char * buffer0=new char[imgfp0.GetLength()+1]; 
//		wcstombs(buffer0,imgfp0.GetBuffer(),imgfp0.GetLength()+1);
//mwArray fp1(buffer0);
//
//char * buffer1=new char[imgfp1.GetLength()+1]; 
//		wcstombs(buffer1,imgfp1.GetBuffer(),imgfp1.GetLength()+1);
//mwArray fp2(buffer1);
//
//
//
//
//mwArray mx1(rl.size(), 1, mxDOUBLE_CLASS);
//mx1.SetData(x1, rl.size());
//
//mwArray my1(rl.size(), 1, mxDOUBLE_CLASS);
//my1.SetData(y1.data(), rl.size());
//
//mwArray mx2(rl.size(), 1, mxDOUBLE_CLASS);
//mx2.SetData(x2.data(), rl.size());
//
//mwArray my2(rl.size(), 1, mxDOUBLE_CLASS);
//my2.SetData(y2.data(), rl.size());





//hetu(1,hh,fp1,fp2,fpo,mx1,my1,mx2,my2);

       //mclWaitForFiguresToDie(NULL);



	   //hetuTerminate();
		//mclTerminateApplication();


	Engine *ep;
  
  ep=engOpen(NULL);//打开引擎；
 
  engSetVisible(ep, 0);
  
  engEvalString(ep,"x=0:0.001:2*pi;");
  
  engEvalString(ep,"y=sin(x);");     
  
  engEvalString(ep,"plot(x,y)");//会显示出图
//本文来自: 高校自动化网(Www.zdh1909.com) 详细出处参考(转载请保留本链接)：http://www.zdh1909.com/html/matlab/15433.html



}
