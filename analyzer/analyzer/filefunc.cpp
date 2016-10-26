#include "stdafx.h"
#include "filefunc.h"


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


CString ChooseFolderDlg()
{
	BROWSEINFO bi;                     //BROWSEINFO结构有很多成员参数可以自己设置
	ZeroMemory(&bi,sizeof(BROWSEINFO));     //将以&bi为起始地址，大小为sizeof(BROWSEINFO)的内存区域用0填充  

	//bi.ulFlags=BIF_BROWSEINCLUDEFILES;        //设置弹出的对话框属性。此处意思为包含文件。如果不设这个值，默认的是只有文件夹。ulFlags还可   以定义很多别的属性  

	bi.ulFlags=BIF_NEWDIALOGSTYLE;            //窗口可以调整大小，有新建文件夹按钮  

	//bi.lpszTitle=L"指定要处理的文件夹";       //在窗口内显示提示用户的语句 
	//bi.hwndOwner = m_hWnd;   
	//bi.pidlRoot = NULL;   
	//bi.pszDisplayName = szPath;   
	//bi.lpfn = NULL;   
	//bi.lParam = 0;   
	//bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);   //Displays a dialog box enabling the user to select a Shell folder.  
	
	if(pidl != NULL){ 
		TCHAR * path = new TCHAR[MAX_PATH];  
		BOOL flg=SHGetPathFromIDList(pidl,path);      //Converts an item identifier list to a file system path  
		if(flg==TRUE){
			return CString(path);
		}
	}  
	return CString();
}

