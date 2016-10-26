#pragma once
#include "listctrlc.hpp"
//#include "linespec.h"
#include "../struct1/LineSpec.hpp"
#include "../resource.h"

#include "../messagemaphpp.h"

class ListCtrlLS :
	public ListCtrlC
{
public:
	ListCtrlLS(void){};
	virtual ~ListCtrlLS(void){};
	int InsertItemLS(int i, const LineSpec & ls)
	{
		CString strTemp=L"";
		int ri=InsertItem( i, strTemp );

		strTemp.Format(L"%d",i+1);
		SetItemText(i, 1, strTemp);
		AdjustWidth(1,strTemp);

		SetItemText(i,2,ls.name);
		AdjustWidth(2,ls.name);

		strTemp.Format(L"%d",ls.colour);
		SetItemText(i, 3, strTemp);
		AdjustWidth(3,strTemp);

		strTemp.Format(L"%d",ls.dotSize);
		SetItemText(i,4,strTemp);
		AdjustWidth(4,strTemp);

		SetChoice(i,5,ls.lineType);

		SetChoice(i,6,ls.smoothLine);

		SetChoice(i,7,(ls.traceLast?0:1));

		return ri;
	};
	bool GetItemLS(int i, LineSpec & ls)
	{

		ls.name=GetItemText(i,2);

		CString strTemp;
		strTemp=GetItemText(i,3);
		ls.colour=_wtoi(strTemp.GetBuffer());

		strTemp=GetItemText(i,4);
		ls.dotSize=_wtoi(strTemp.GetBuffer());

		ls.lineType=GetChoice(i,5);
		ls.smoothLine=GetChoice(i,6);
		ls.traceLast=(GetChoice(i,7)==0);


		return true;
	};
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (ListCtrlC::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here


		CString strTemp;
		std::vector<CString> strl(1,CString());
		for(int i=IDS_STRING720;i<=IDS_STRING726;i++){
			strTemp.LoadStringW(i);
			strl.push_back(strTemp);
		}
		this->SetHeader(strl);

		typelist[2]=eEdit;
		typelist[3]=eColor;
		typelist[4]=eEditInt;
		typelist[7]=typelist[5]=typelist[6]=eCombo;

		maxi[4]=10;

		// insert string elements  for the ComboBox : 
		for ( int j=IDS_STRING_GRID_LINE_SOLID ; j <= IDS_STRING_GRID_LINE_HIDE ; j++){
			strTemp.LoadStringW(j);
			cbstr[5].push_back(strTemp);
			AdjustWidth(5,strTemp);
		}

		// insert string elements  for the ComboBox : 
		for ( int j=IDS_STRING_NO_SMOOTH ; j <= IDS_STRING_BEZIER ; j++){
			strTemp.LoadStringW(j);
			cbstr[6].push_back(strTemp);
			AdjustWidth(6,strTemp);
		}

		// insert string elements  for the ComboBox : 
		for ( int j=IDS_STRING_YES ; j <= IDS_STRING_NO ; j++){
			strTemp.LoadStringW(j);
			cbstr[7].push_back(strTemp);
			AdjustWidth(7,strTemp);
		}
		return 0;
	};

//	DECLARE_MESSAGE_MAP()
//};
//
//BEGIN_MESSAGE_MAP(ListCtrlLS, ListCtrlC)
//	ON_WM_CREATE()
//END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP_HPP(ListCtrlLS, ListCtrlC)
	ON_WM_CREATE()
END_MESSAGE_MAP_HPP()

};