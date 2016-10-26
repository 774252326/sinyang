#pragma once
#include "rawdata.hpp"
#include <locale.h>

class RawDataEx :
	public RawData
{

public:
	static BOOL SaveString(CString fp, CString str)
	{
		CStdioFile file;
		BOOL flag;
		flag=file.Open(fp, CFile::modeCreate | CFile::modeWrite /*| CFile::typeText*/);

		if(flag)
		{	
			char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
			setlocale( LC_CTYPE, "chs" );//设定
			file.WriteString(str);
			setlocale( LC_CTYPE, old_locale ); 
			free( old_locale );//还原区域设定

			file.Close();
		}
		return flag;
	};

public:
	std::vector<CString> xlabel;
	std::vector<CString> ylabel;
	std::vector<CString> title;
public:
	RawDataEx(void){};
	virtual ~RawDataEx(void){};
	virtual void Serialize(CArchive& ar){};

	void operator=(const RawDataEx &src)
	{
		xlabel.assign(src.xlabel.begin(),src.xlabel.end());
		ylabel.assign(src.ylabel.begin(),src.ylabel.end());
		title.assign(src.title.begin(),src.title.end());
		RawData::operator=(src);
	};
	RawDataEx(const RawDataEx &src){ operator=(src); };
	RawDataEx(const RawData &src, CString tit=L"curve", CString xla=L"x", CString yla=L"y")
	{
		RawData::operator=(src); 
		xlabel.assign(ll.size(),xla);
		ylabel.assign(ll.size(),yla);
		title.assign(ll.size(),tit);
	};

	bool CheckData(void)
	{
		if(RawData::CheckData()
			&& ll.size()<=xlabel.size()
			&& ll.size()<=ylabel.size()
			&& ll.size()<=title.size())
			return true;

		return false;
	};


	void Clear(void)
	{
		xlabel.clear();
		ylabel.clear();
		title.clear();
		RawData::Clear();
	};

	bool AddNew(const std::vector<double> &x, const std::vector<double> &y, const CString &xla, const CString &yla, const CString &tit )
	{
		bool flg=RawData::AddNew(x,y);
		if(flg){
			xlabel.push_back(xla);
			ylabel.push_back(yla);
			title.push_back(tit);
			return true;
		}
		return false;
	};

	void GetDatai(size_t index, std::vector<double> & x, std::vector<double> & y, CString &xla, CString &yla, CString &tit) const
	{

		if(index>=ll.size())
			xla=yla=tit=L"";
		else{
			xla=xlabel[index];
			yla=ylabel[index];
			tit=title[index];
		}

		RawData::GetDatai(index,x,y);
	};

	void GetDatai(size_t index, RawDataEx &rdei)
	{	
		rdei.Clear();
		if(index<ll.size()){
			rdei.xlabel.push_back(CString());
			rdei.ylabel.push_back(CString());
			rdei.title.push_back(CString());
			GetDatai(index,
				rdei.xll,
				rdei.yll,
				rdei.xlabel[0],
				rdei.ylabel[0],
				rdei.title[0]);
			rdei.ll.push_back(ll[index]);
		}

	};

	bool AppendData(const RawDataEx & pda)
	{
		xll.resize(xll.size()+pda.xll.size());
		std::copy_backward(pda.xll.begin(),pda.xll.end(),xll.end());
		yll.resize(yll.size()+pda.yll.size());
		std::copy_backward(pda.yll.begin(),pda.yll.end(),yll.end());
		ll.resize(ll.size()+pda.ll.size());
		std::copy_backward(pda.ll.begin(),pda.ll.end(),ll.end());

		xlabel.resize(xlabel.size()+pda.xlabel.size());
		std::copy_backward(pda.xlabel.begin(),pda.xlabel.end(),xlabel.end());
		ylabel.resize(ylabel.size()+pda.ylabel.size());
		std::copy_backward(pda.ylabel.begin(),pda.ylabel.end(),ylabel.end());
		title.resize(title.size()+pda.title.size());
		std::copy_backward(pda.title.begin(),pda.title.end(),title.end());

		return true;

	};

	BOOL SaveToText(size_t index)
	{
		RawDataEx rdetmp;
		GetDatai(index,rdetmp);
		CString str=RawData::SaveText(rdetmp.xll,rdetmp.yll,rdetmp.xlabel[0],rdetmp.ylabel[0]);

		CFileFind finder;
		BOOL bWorking = finder.FindFile(rdetmp.title[0]+L".txt");
		if(bWorking==FALSE){
			return SaveString(rdetmp.title[0]+L".txt",str);
		}

		return FALSE;
	};



};

