#include "StdAfx.h"
#include "ListCtrlLS.h"
#include "func.h"
#include "resource.h"

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


	//ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);

	//LONG IStyle;
	//IStyle=GetWindowLong(GetSafeHwnd(), GWL_STYLE);//获取当前窗口style
	//IStyle&= ~LVS_TYPEMASK; //清除显示方式位
	//IStyle|= LVS_REPORT; //set style
	//SetWindowLong(GetSafeHwnd(), GWL_STYLE, IStyle);//设置style

	//DWORD dwStyle1;
	//dwStyle1 = GetExtendedStyle();
	//dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	//dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	////dwStyle1 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	//SetExtendedStyle(dwStyle1); //设置扩展风格

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
		AdjustWidth(this,4,strTemp);
	}

		// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_NO_SMOOTH ; j <= IDS_STRING_BEZIER ; j++){
		strTemp.LoadStringW(j);
		cbstr[5].push_back(strTemp);
		AdjustWidth(this,5,strTemp);
	}

		// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_YES ; j <= IDS_STRING_NO ; j++){
		strTemp.LoadStringW(j);
		cbstr[6].push_back(strTemp);
		AdjustWidth(this,6,strTemp);
	}





	return 0;
}
