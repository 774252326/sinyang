// RawData.cpp : implementation file
//

#include "stdafx.h"
//#include "analyzer.h"
#include "RawData.h"
//#include "func.h"
#include "pcct.h"


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


// RawData

RawData::RawData()
{
}

RawData::~RawData()
{
}


void RawData::operator=(const RawData &src)
{
	ll.assign(src.ll.begin(),src.ll.end());
	xll.assign(src.xll.begin(),src.xll.end());
	yll.assign(src.yll.begin(),src.yll.end());
}


// RawData member functions


void RawData::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar<<ll.size();		
		size_t si=0;
		size_t ei=0;
		for(size_t i=0;i<ll.size();i++){
			ar<<ll[i];
			ei=si+ll[i];
			for(size_t j=si;j<ei;j++){
				ar<<xll[j]
				<<yll[j];
			}
			si=ei;
		}
	}
	else
	{	// loading code
		size_t l;

		ar>>l;
		ll.assign(l,0);
		size_t si=0;
		size_t ei=0;
		for(size_t i=0;i<ll.size();i++){
			ar>>ll[i];
			xll.resize(si+ll[i],0);
			yll.resize(si+ll[i],0);
			ei=si+ll[i];
			for(size_t j=si;j<ei;j++){
				ar>>xll[j]
				>>yll[j];
			}
			si=ei;
		}

	}
}


void RawData::GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y) const
{
	if(index>=ll.size()){
		x.clear();
		y.clear();
		return;
	}

	size_t starti=0;
	for(size_t i=0;i<index;i++){
		starti+=ll[i];
	}
	size_t endi=starti+ll[index];

	x.assign(xll.begin()+starti, xll.begin()+endi);
	y.assign(yll.begin()+starti, yll.begin()+endi);
}

void RawData::Clear(void)
{
	xll.clear();
	yll.clear();
	ll.clear();
}


int RawData::LoadFromFileList(CString fp)
{
	Clear();

	std::vector<CString> filelist;
	LoadFileList(fp,filelist);
	pcct data;

	while(!filelist.empty()){
		/////load data from file////////////
		data.clear();
		data.readFile(filelist.front());
		data.TomA();

		xll.resize(xll.size()+data.potential.size());
		std::copy_backward(data.potential.begin(),data.potential.end(),xll.end());

		yll.resize(yll.size()+data.current.size());
		std::copy_backward(data.current.begin(),data.current.end(),yll.end());

		ll.push_back(data.potential.size());

		filelist.erase(filelist.begin());

	}
	
	return 0;
}
