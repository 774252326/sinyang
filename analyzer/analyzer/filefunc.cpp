#include "stdafx.h"
#include "filefunc.h"
#include "calfunc.h"
#include "struct1\pcct.hpp"

#include "windowsversion.hpp"

sapitemA sapitemdummy;
BYTE bytedummy;
double doubledummy;




const DWORD sleepms=100;

const size_t nd=500;
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
	//,int second=-1
	//,int second=3
	,int second=0
	)
{
	int interval=1000;
	while( //second<0||
		(waitflg!=running && second--!=0)
		){
			Sleep(interval);
	}
	waitflg=running;
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

	ProcessState *pst=((mypara*)pParam)->psta;

	//COutputListA* ol=ow->GetListCtrl();
	//ol->DeleteAllItems();

	COutputListA* ol=((mypara*)pParam)->ol;

	CanalyzerDoc* pDoc=lv->GetDocument();

	delete pParam;

	std::vector<CString> filelist;
	LoadFileList(flistlist[pDoc->p1.analysistype],filelist);

	UINT runstate=2;

	double v2a;
	sapitemA outitem;
	BYTE outstep;

	CSingleLock singleLock(&(pDoc->m_CritSection));
	singleLock.Lock();
	if (singleLock.IsLocked())  // Resource has been locked
	{
		pDoc->raw.Clear();
		// Now that we are finished, 
		// unlock the resource for others.
		singleLock.Unlock();
	}

	//runstate=ComputeStateData(pDoc->p1.analysistype,pDoc->p2,pDoc->p3,pDoc->raw,pDoc->dol,outitem,outstep,v2a);

	runstate=pDoc->ComputeStateData(outitem,outstep,v2a);

	TRACE(L"%d\n",runstate);
	if(runstate==3){
		CString strerr;
		strerr.LoadStringW(IDS_STRING_STEP_ERROR);
		//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
		*pst=stop;
		return 1;
	}
	//::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(v2a),NULL);

	*pst=pause;
	WaitSecond(*pst);
	/////////////////////////////////////////
	{		
		//TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		//CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
		//	OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);
		//if(fileDlg.DoModal() == IDOK)
		//{ 
		//	filelist.assign(1,fileDlg.GetPathName());
		//}
		//else{
		//	return 9;
		//}
	}
	////////////////////////////////////////
	//::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);

	lv->pw.bMouseCursor=rv->pw.bMouseCursor=false;

	while(true){

		if(filelist.empty()){

			CString strerr;
			strerr.LoadStringW(IDS_STRING_STEP_ERROR);
			//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);

			*pst=stop;
			return 1;

		}



		/////load data from file////////////
		//data.clear();
		pcct data;
		data.readFile(filelist.front());
		//data.readFile1(filelist.front());
		data.TomA();

		std::vector<double> x;
		std::vector<double> y;
		size_t rnd;
		pDoc->raw.ll.push_back(0);

		while(true){
			rnd=data.popData(x,y,nd);
			//pDoc->raw.xll.resize(pDoc->raw.xll.size()+x.size());
			//std::copy_backward(x.begin(),x.end(),pDoc->raw.xll.end());

			//pDoc->raw.yll.resize(pDoc->raw.yll.size()+y.size());
			//std::copy_backward(y.begin(),y.end(),pDoc->raw.yll.end());

			//pDoc->raw.ll.back()+=x.size();
			if(x.empty()||y.empty()){
				TRACE("input empty");
				return 8;
			}

			//CSingleLock singleLock(&(pDoc->m_CritSection));
			singleLock.Lock();

			if (singleLock.IsLocked())  // Resource has been locked
			{

				pDoc->raw.AddFollow(x,y);

				pDoc->dol.clear();

				// Now that we are finished, 
				// unlock the resource for others.
				singleLock.Unlock();
			}


			//runstate=ComputeStateData(pDoc->p1.analysistype,pDoc->p2,pDoc->p3,pDoc->raw,pDoc->dol,outitem,outstep,v2a);	
			runstate=pDoc->ComputeStateData(outitem,outstep,v2a);



			TRACE(L"%d\n",runstate);

			if(runstate==3){
				CString strerr;
				strerr.LoadStringW(IDS_STRING_STEP_ERROR);
				//::SendMessage(cba->GetSafeHwnd(),MESSAGE_OVER,(WPARAM)(strerr.GetBuffer()),NULL);
				*pst=stop;
				return 1;
			}

			//pDoc->Show();
			::PostMessage(rv->GetSafeHwnd(),MESSAGE_UPDATE_TEST,NULL,NULL);
			::PostMessage(lv->GetSafeHwnd(),MESSAGE_UPDATE_RAW,NULL,NULL);
			::PostMessage(ol->GetSafeHwnd(),MESSAGE_SHOW_DOL,NULL,(LPARAM)pDoc);
			//::SendMessage(ow->GetListCtrl()->GetSafeHwnd(),MESSAGE_SHOW_DOL,NULL,NULL);
			Sleep(sleepms);

			if(runstate==5){
				//::SendMessage(cba->GetSafeHwnd(),MESSAGE_WAIT_RESPONSE,(WPARAM)&(v2a),NULL);
				*pst=pause;
				WaitSecond(*pst);
				/////////////////////////////////////////
				{
					//TCHAR szFilters[]= _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
					//CFileDialog fileDlg(TRUE, _T("txt"), _T("*.txt"),
					//	OFN_FILEMUSTEXIST | OFN_HIDEREADONLY /*| OFN_ALLOWMULTISELECT*/ , szFilters);
					//if(fileDlg.DoModal() == IDOK)
					//{ 
					//	filelist.push_back(fileDlg.GetPathName());
					//}
					//else{
					//	return 9;
					//}
				}
				////////////////////////////////////////
				//::SendMessage(cba->GetSafeHwnd(),MESSAGE_BUSY,NULL,NULL);
				filelist.erase(filelist.begin());
				break;
			}

			if(runstate==0){
				::PostMessage(rv->GetSafeHwnd(),MESSAGE_COMPUTE_RESULT,NULL,NULL);
				filelist.erase(filelist.begin());
				*pst=stop;
				return 0;
				//break;
			}
		}

	}





	*pst=stop;

	return 0;
}