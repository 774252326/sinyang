#pragma once
#include "editb.hpp"
///
///表格编辑框类EditB的扩展
/// 加入对输入数据的验证
///
template <typename T>
class EditC :
	public EditB
{
public:
	T min;
	T max;
	T x;
public:
	EditC<T>(void)
		:EditB()
	{
		min=0;
		max=0;
		x=0;
	};
	virtual ~EditC<T>(void){};


	void AFXAPI DDV_MinMax(
		CDataExchange* pDX,
		double const& value,
		double minVal,
		double maxVal 
		)
	{
		DDV_MinMaxDouble(pDX,value,minVal,maxVal);
	};

	void AFXAPI DDV_MinMax(
		CDataExchange* pDX,
		DWORD const& value,
		DWORD minVal,
		DWORD maxVal 
		)
	{
		DDV_MinMaxDWord(pDX,value,minVal,maxVal);
	};

	void AFXAPI DDV_MinMax(
		CDataExchange* pDX,
		int const& value,
		int minVal,
		int maxVal 
		)
	{
		DDV_MinMaxInt(pDX,value,minVal,maxVal);
	};

	void AFXAPI DDV_MinMax(
		CDataExchange* pDX,
		UINT const& value,
		UINT minVal,
		UINT maxVal 
		)
	{
		DDV_MinMaxUInt(pDX,value,minVal,maxVal);
	};

	void AFXAPI DDV_MinMax(
		CDataExchange* pDX,
		float const& value,
		float minVal,
		float maxVal 
		)
	{
		DDV_MinMaxFloat(pDX,value,minVal,maxVal);
	};
	void AFXAPI DDV_MinMax(
		CDataExchange* pDX,
		long const& value,
		long minVal,
		long maxVal 
		)
	{
		DDV_MinMaxLong(pDX,value,minVal,maxVal);
	};

	void AFXAPI DDV_MinMax(
		CDataExchange* pDX,
		short const& value,
		short minVal,
		short maxVal 
		)
	{
		DDV_MinMaxShort(pDX,value,minVal,maxVal);
	};

	virtual void DoDataExchange(CDataExchange* pDX)
		//template <typename T>
		//void EditC<T>::DoDataExchange(CDataExchange* pDX)
	{
		// TODO: Add your specialized code here and/or call the base class

		DDX_Text(pDX, GetDlgCtrlID(), x);

		if(min<=max)
			DDV_MinMax(pDX,x,min,max);

		EditB::DoDataExchange(pDX);
	};

};

