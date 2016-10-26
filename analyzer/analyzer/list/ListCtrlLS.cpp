#include "StdAfx.h"
#include "ListCtrlLS.h"
//#include "func.h"
#include "../resource.h"

ListCtrlLS::ListCtrlLS(void)
{
}


ListCtrlLS::~ListCtrlLS(void)
{
}
BEGIN_MESSAGE_MAP(ListCtrlLS, ListCtrlA)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int ListCtrlLS::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	CString strTemp;
	std::vector<CString> strl;
	for(int i=IDS_STRING720;i<=IDS_STRING726;i++){
		strTemp.LoadStringW(i);
		strl.push_back(strTemp);
	}
	this->SetHeader(strl);

	typelist[1]=eEdit;
	typelist[2]=eColor;
	typelist[3]=eEditNum;
	typelist[4]=typelist[5]=typelist[6]=eCombo;

	maxi[3]=10;

	// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_GRID_LINE_SOLID ; j <= IDS_STRING_GRID_LINE_HIDE ; j++){
		strTemp.LoadStringW(j);
		cbstr[4].push_back(strTemp);
		AdjustWidth(4,strTemp);
	}

	// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_NO_SMOOTH ; j <= IDS_STRING_BEZIER ; j++){
		strTemp.LoadStringW(j);
		cbstr[5].push_back(strTemp);
		AdjustWidth(5,strTemp);
	}

	// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_YES ; j <= IDS_STRING_NO ; j++){
		strTemp.LoadStringW(j);
		cbstr[6].push_back(strTemp);
		AdjustWidth(6,strTemp);
	}





	return 0;
}


int ListCtrlLS::InsertItemLS(int i, const LineSpec & ls)
{
	CString strTemp;

	strTemp.Format(L"%d",i+1);
	int ri=InsertItem( i, strTemp );
	AdjustWidth(0,strTemp);

	SetItemText(i,1,ls.name);
	AdjustWidth(1,ls.name);

	strTemp.Format(L"%d",ls.colour);
	SetItemText(i, 2, strTemp);
	AdjustWidth(2,strTemp);

	strTemp.Format(L"%d",ls.dotSize);
	SetItemText(i,3,strTemp);
	AdjustWidth(3,strTemp);

	//strTemp.LoadStringW(IDS_STRING_GRID_LINE_SOLID+ls.lineType);
	//SetItemText(i,4,strTemp);
	//AdjustWidth(this,4,strTemp);

	SetChoice(i,4,ls.lineType);

	//strTemp.LoadStringW(IDS_STRING_NO_SMOOTH+ls.smoothLine);
	//SetItemText(i,5,strTemp);
	//AdjustWidth(this,5,strTemp);

	SetChoice(i,5,ls.smoothLine);

	//strTemp.LoadStringW( (ls.traceLast?IDS_STRING_YES:IDS_STRING_NO) );
	//SetItemText(i,6,strTemp);
	//AdjustWidth(this,6,strTemp);

	SetChoice(i,6,(ls.traceLast?0:1));

	return ri;
}


bool ListCtrlLS::GetItemLS(int i, LineSpec & ls)
{

	ls.name=GetItemText(i,1);

	CString strTemp;
	strTemp=GetItemText(i,2);
	ls.colour=_wtoi(strTemp.GetBuffer());

	strTemp=GetItemText(i,3);
	ls.dotSize=_wtoi(strTemp.GetBuffer());

	ls.lineType=GetChoice(i,4);
	ls.smoothLine=GetChoice(i,5);
	ls.traceLast=(GetChoice(i,6)==0);


	return true;
}
