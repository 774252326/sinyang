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
	BROWSEINFO bi;                     //BROWSEINFO�ṹ�кܶ��Ա���������Լ�����
	ZeroMemory(&bi,sizeof(BROWSEINFO));     //����&biΪ��ʼ��ַ����СΪsizeof(BROWSEINFO)���ڴ�������0���  

	//bi.ulFlags=BIF_BROWSEINCLUDEFILES;        //���õ����ĶԻ������ԡ��˴���˼Ϊ�����ļ�������������ֵ��Ĭ�ϵ���ֻ���ļ��С�ulFlags����   �Զ���ܶ�������  

	bi.ulFlags=BIF_NEWDIALOGSTYLE;            //���ڿ��Ե�����С�����½��ļ��а�ť  

	//bi.lpszTitle=L"ָ��Ҫ������ļ���";       //�ڴ�������ʾ��ʾ�û������ 
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

