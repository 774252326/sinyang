#include "stdafx.h"
#include "filefunc.h"
#include "calfunc.h"
#include "struct1\pcct.hpp"

#include "windowsversion.hpp"

#include "StartDlg.hpp"
#include "MainFrm.h"

sapitemA sapitemdummy;
BYTE bytedummy;
double doubledummy;




const DWORD sleepms=100;

const size_t nd=50;
//const size_t nd=sleepms/10;


#ifndef _DEBUG
//CString folderp=(GetWinVer()==6)? L"data\\d\\" : L"..\\data\\d\\";
//CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
CString folderp=L"D:\\data\\d\\";
//CString folderp=L"C:\\Users\\G\\Desktop\\data\\d\\";
#else
//CString folderp=L"C:\\Users\\r8anw2x\\Desktop\\data\\d\\";
CString folderp=L"D:\\data\\d\\";
//CString folderp=L"C:\\Users\\G\\Desktop\\data\\d\\";
#endif

CString DEMOflist=folderp+L"fl1.txt";
CString DTRflist=folderp+L"dtr.txt";
CString DTAflist=folderp+L"dta.txt";
CString LATRflist=folderp+L"latr.txt";
CString LATAflist=folderp+L"lata.txt";
CString RCRflist=folderp+L"rcr.txt";
CString RCAflist=folderp+L"rca.txt";
CString SARRflist=folderp+L"sarr.txt";
CString SARAflist=folderp+L"sara.txt";
CString NEWRflist=folderp+L"j.txt";
CString NEWAflist=folderp+L"k.txt";
CString NERflist=folderp+L"l.txt";
CString NEAflist=folderp+L"m.txt";


CString flistlist[]={
	DEMOflist,
	DTRflist,
	DTAflist,
	LATRflist,
	LATAflist,
	RCRflist,
	RCAflist,
	SARRflist,
	SARAflist,
	NEWRflist,
	NEWAflist,
	NERflist,
	NEAflist
};

void WaitSecond(ProcessState &waitflg
	,int second=-1
	//,int second=3
	//,int second=0
	,int interval=1000
	)
{
	;
	while( waitflg!=running
		&& ( second<0 || second--!=0 )
		){
			Sleep(interval);
	}
	//waitflg=running;
}


BOOL ReadFileCustom(CObject * co, size_t n, CString fp)
{
	CFile theFile;
	if( theFile.Open(fp, /*CFile::modeCreate |*/ CFile::modeRead) ){
		CArchive archive(&theFile, CArchive::load);

		for(size_t i=0;i<n;i++){
			co[i].Serialize(archive);
		}

		archive.Close();
		theFile.Close();
		return TRUE;
	}
	return FALSE;
}


BOOL WriteFileCustom(CObject * co, size_t n, CString fp)
{
	CFile theFile;
	if( theFile.Open(fp, CFile::modeCreate | CFile::modeWrite) ){
		CArchive archive(&theFile, CArchive::store);

		for(size_t i=0;i<n;i++){
			co[i].Serialize(archive);
		}

		archive.Close();
		theFile.Close();
		return TRUE;
	}
	return FALSE;
}


void LoadFileList(const CString &m_filePath, std::vector<CString> &filelist)
{

	CString folderpath=m_filePath.Left(m_filePath.ReverseFind('\\'));

	filelist.clear();
	CStdioFile file;
	BOOL readflag;
	readflag=file.Open(m_filePath, CFile::modeRead);

	if(readflag)
	{	
		CString strRead;
		//TRACE("\n--Begin to read file");
		while(file.ReadString(strRead)){
			strRead=folderpath+"\\"+strRead;
			filelist.push_back(strRead);
		}
		//TRACE("\n--End reading\n");
		file.Close();
	}
}




UINT PROCESS(LPVOID pParam)
{
	//CanalyzerDoc* pDoc=(CanalyzerDoc*)pParam;


	CanalyzerViewL* lv=((mypara*)pParam)->leftp;
	CanalyzerViewR* rv=((mypara*)pParam)->rightp;
	//COutputWnd *ow=((mypara*)pParam)->outw;
	//CMFCCaptionBarA *cba=((mypara*)pParam)->cba;

	//ProcessState *pst=((mypara*)pParam)->psta;

	//WaitDlg *wd=((mypara*)pParam)->wd;

	//COutputListA* ol=ow->GetListCtrl();
	//ol->DeleteAllItems();

	CMainFrame *mf=((mypara*)pParam)->mf;

	COutputListA* ol=((mypara*)pParam)->ol;



	CanalyzerDoc* pDoc=lv->GetDocument();

	delete pParam;
	////////////////////////////////////////////////////
	std::vector<CString> filelist;
	LoadFileList(flistlist[pDoc->p1.analysistype],filelist);
	//////////////////////////////////////////////////////



	//UINT runstate=2;

	//double v2a;
	////sapitemA outitem;
	//BYTE outstep;
	//size_t nextidx;
	//size_t nowidx;
	//CSingleLock singleLock(&(pDoc->m_CritSection));
	//singleLock.Lock();
	//if (singleLock.IsLocked())  // Resource has been locked
	//{
	//	pDoc->raw.Clear();
	//	// Now that we are finished, 
	//	// unlock the resource for others.
	//	singleLock.Unlock();
	//}

	//runstate=pDoc->ComputeStateData(nowidx,nextidx,outstep,v2a);

	//TRACE(L"rs=%d,ci=%d,ni=%d\n",runstate,nowidx,nextidx);
	//if(runstate==3){
	//	CString strerr;
	//	strerr.LoadStringW(IDS_STRING_STEP_ERROR);
	//	//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
	//	mf->pst=stop;
	//	return 1;
	//}
	////::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(v2a),NULL);

	////mf->pst=pause;

	//CString str;
	//str.Format(L"add solution %g ml",v2a);

	//mf->SendMessage(MESSAGE_WAIT_RESPONSE,(WPARAM)(str.GetBuffer()),NULL);

	//WaitSecond(mf->pst);


	///////////////////////////////////////////
	//{		
	//	//TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	//	//CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
	//	//	OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);
	//	//if(fileDlg.DoModal() == IDOK)
	//	//{ 
	//	//	filelist.assign(1,fileDlg.GetPathName());
	//	//}
	//	//else{
	//	//	return 9;
	//	//}
	//}
	//////////////////////////////////////////

	//lv->pw.bMouseCursor=rv->pw.bMouseCursor=false;

	//while(true){

	//	if(filelist.empty()){

	//		CString strerr;
	//		strerr.LoadStringW(IDS_STRING_STEP_ERROR);
	//		//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
	//		mf->pst=stop;
	//		return 1;

	//	}



	//	/////load data from file////////////
	//	pcct data;
	//	data.readFile(filelist.front());
	//	data.TomA();

	//	std::vector<double> x;
	//	std::vector<double> y;
	//	size_t rnd;
	//	pDoc->raw.ll.push_back(0);

	//	while(true){
	//		rnd=data.popData(x,y,nd);

	//		if(x.empty()||y.empty()){
	//			TRACE("input empty");
	//			return 8;
	//		}

	//		//CSingleLock singleLock(&(pDoc->m_CritSection));
	//		singleLock.Lock();
	//		if (singleLock.IsLocked())  // Resource has been locked
	//		{
	//			pDoc->raw.AddFollow(x,y);
	//			// Now that we are finished, 
	//			// unlock the resource for others.
	//			singleLock.Unlock();
	//		}


	//		runstate=pDoc->ComputeStateData(nowidx,nextidx,outstep,v2a);

	//		TRACE(L"rs=%d,ci=%d,ni=%d\n",runstate,nowidx,nextidx);

	//		if(runstate==3){
	//			CString strerr;
	//			strerr.LoadStringW(IDS_STRING_STEP_ERROR);
	//			//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
	//			mf->pst=stop;
	//			return 1;
	//		}
	//		if(runstate==4){
	//			mf->pst=stop;
	//			return 4;
	//		}

	//		::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
	//		::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
	//		::PostMessage(ol->GetSafeHwnd(),MESSAGE_SHOW_DOL,NULL,(LPARAM)pDoc);
	//		//::SendMessage(ow->GetListCtrl()->GetSafeHwnd(),MESSAGE_SHOW_DOL,NULL,NULL);
	//		Sleep(sleepms);

	//		if(runstate==5){
	//			//::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(v2a),NULL);
	//			//mf->pst=pause;

	//				str.Format(L"add solution %g ml",v2a);

	//			mf->SendMessage(MESSAGE_WAIT_RESPONSE,(WPARAM)(str.GetBuffer()),NULL);
	//			WaitSecond(mf->pst);
	//			/////////////////////////////////////////
	//			{
	//				//TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	//				//CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
	//				//	OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);
	//				//if(fileDlg.DoModal() == IDOK)
	//				//{ 
	//				//	filelist.push_back(fileDlg.GetPathName());
	//				//}
	//				//else{
	//				//	return 9;
	//				//}
	//			}
	//			////////////////////////////////////////

	//			filelist.erase(filelist.begin());
	//			break;
	//		}

	//		if(runstate==0){
	//			::PostMessage(rv->GetSafeHwnd(),MESSAGE_COMPUTE_RESULT,NULL,NULL);
	//			filelist.erase(filelist.begin());
	//			mf->pst=stop;
	//			return 0;
	//			//break;
	//		}
	//	}

	//}
	/////////////////////////////////////////////////////////////////////////////////////



	double v2a;
	BYTE outstep;
	size_t nextidx;
	size_t nowidx;
	CSingleLock singleLock(&(pDoc->m_CritSection));
	lv->pw.bMouseCursor=rv->pw.bMouseCursor=false;
	CSingleLock singleLock1(&(mf->m_CritSection));

	pcct data;
	std::vector<double> x;
	std::vector<double> y;
	size_t rnd;


	if(singleLock.Lock())
	{
		pDoc->raw.Clear();
		// Now that we are finished, 
		// unlock the resource for others.
		singleLock.Unlock();
	}

	mf->SendMessage(MESSAGE_UPDATE_DOL,NULL,NULL);
	//mf->OnMessageUpdateDol(NULL,NULL);

	while(mf->pst!=stop){

		//if(mf->pst!=running){
			//Sleep(sleepms);
		//}

		WaitSecond(mf->pst,-1,50);

		//if(singleLock1.Lock())
		{
			if(pDoc->runstate==0){
				//singleLock1.Unlock();
				mf->pst=stop;
				return 0;
			}


			if(pDoc->runstate==5){
				//singleLock1.Unlock();

				if(filelist.empty()){
					CString strerr;
					strerr.LoadStringW(IDS_STRING_STEP_ERROR);
					//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
					mf->pst=stop;
					return 1;
				}

				/////load data from file////////////
				data.clear();
				data.readFile(filelist.front());
				data.TomA();
				filelist.erase(filelist.begin());

				rnd=data.popData(x,y,nd);

				if(x.empty()||y.empty()){
					TRACE("input empty");
					mf->pst=stop;
					return 8;
				}
				if(singleLock.Lock())
				{
					pDoc->raw.AddNew(x,y);
					// Now that we are finished, 
					// unlock the resource for others.
					singleLock.Unlock();
				}
			}
			else{
				//singleLock1.Unlock();

				rnd=data.popData(x,y,nd);

				if(x.empty()||y.empty()){
					TRACE("input empty");
					mf->pst=stop;
					return 8;
				}

				if(singleLock.Lock())
				{
					pDoc->raw.AddFollow(x,y);
					// Now that we are finished, 
					// unlock the resource for others.
					singleLock.Unlock();
				}
			}

			mf->SendMessage(MESSAGE_UPDATE_DOL,NULL,NULL);
			//mf->OnMessageUpdateDol(NULL,NULL);

			Sleep(sleepms);
		}

	}



	mf->pst=stop;

	return 0;
}


UINT PROCESS1(LPVOID pParam)
{

		//CanalyzerDoc* pDoc=(CanalyzerDoc*)pParam;


	//CanalyzerViewL* lv=((mypara*)pParam)->leftp;
	//CanalyzerViewR* rv=((mypara*)pParam)->rightp;
	//COutputWnd *ow=((mypara*)pParam)->outw;
	//CMFCCaptionBarA *cba=((mypara*)pParam)->cba;

	ProcessState *pst=((mypara1*)pParam)->psta;

	//WaitDlg *wd=((mypara*)pParam)->wd;

	//COutputListA* ol=ow->GetListCtrl();
	//ol->DeleteAllItems();

	//CMainFrame *mf=((mypara*)pParam)->mf;

	//COutputListA* ol=((mypara*)pParam)->ol;



	CanalyzerDoc* pDoc=((mypara1*)pParam)->adoc;

	SAPara p3todo=*(((mypara1*)pParam)->pp3todo);
	

	delete pParam;
	////////////////////////////////////////////////////


	SAPara p3new=pDoc->p3done;
	p3new.AppendData(p3todo);


	CSingleLock singleLock(&(pDoc->m_CritSection));
		while(singleLock.Lock())
		{
			if(*pst!=pause){
				singleLock.Unlock();
				Sleep(10);
			}
			else{
				pDoc->p3=p3new;
				singleLock.Unlock();
				break;
			}
		}


	return 0;
}