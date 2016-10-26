#include "StdAfx.h"
#include "ListCtrlPS.h"
#include "func.h"

ListCtrlPS::ListCtrlPS(void)
{
}


ListCtrlPS::~ListCtrlPS(void)
{
}
BEGIN_MESSAGE_MAP(ListCtrlPS, ListCtrlA)
	ON_WM_CREATE()
	//ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ListCtrlPS::OnLvnEndlabeledit)
	//ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlPS::OnNMDblclk)
	//ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ListCtrlPS::OnLvnBeginlabeledit)
END_MESSAGE_MAP()


int ListCtrlPS::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);

	LONG IStyle;
	IStyle=GetWindowLong(GetSafeHwnd(), GWL_STYLE);//获取当前窗口style
	IStyle&= ~LVS_TYPEMASK; //清除显示方式位
	IStyle|= LVS_REPORT; //set style
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, IStyle);//设置style

	DWORD dwStyle1;
	dwStyle1 = GetExtendedStyle();
	//dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	//dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle1 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	SetExtendedStyle(dwStyle1); //设置扩展风格

	CString strTemp;
	std::vector<CString> strl;
	for(int i=IDS_STRING_COLOR_STYLE;i<=IDS_STRING776;i++){
		strTemp.LoadStringW(i);
		strl.push_back(strTemp);
	}
	this->SetHeader(strl);

	typelist[0]=typelist[1]=eCombo;
	typelist[2]=typelist[3]=eEditNum;
	typelist[4]=typelist[5]=typelist[6]=eColor;
	maxi[2]=maxi[3]=50;

	// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_COLOR_TYPE_0-1 ; j <= IDS_STRING_COLOR_TYPE_3 ; j++){
		strTemp.LoadStringW(j);
		cbstr[0].push_back(strTemp);
		AdjustWidth(this,0,strTemp);
	}

	// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_GRID_LINE_SOLID ; j <= IDS_STRING_GRID_LINE_HIDE ; j++){
		strTemp.LoadStringW(j);
		cbstr[1].push_back(strTemp);
		AdjustWidth(this,1,strTemp);
	}

	return 0;
}


//void ListCtrlPS::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//	ListCtrlA::OnLvnEndlabeledit(pNMHDR, pResult);
//
//	if(this->m_iSubItem==0 && this->GetChoice(0,0)!=0){
//		PlotSpec ps0=PlotSpec(GetChoice(0,0));
//		CString str;
//		str.Format(L"%d",ps0.bkgndC);
//		this->SetItemText(0,4,str);
//		str.Format(L"%d",ps0.metricC);
//		this->SetItemText(0,5,str);
//		str.Format(L"%d",ps0.labelC);
//		this->SetItemText(0,6,str);
//		this->Invalidate();
//	}
//
//	*pResult = 0;
//}


//void ListCtrlPS::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//	if(pNMItemActivate->iSubItem>3 && GetChoice(0,0)!=0 ){
//		;
//	}
//	else{
//		ListCtrlA::OnNMDblclk(pNMHDR,pResult);
//	}
//
//
//
//	*pResult = 0;
//}


int ListCtrlPS::SetD(PlotSpec ps)
{
	CString strTemp;

	//strTemp.LoadStringW(IDS_STRING_COLOR_TYPE_0+ps.GetCrType());
	strTemp.LoadStringW(IDS_STRING_COLOR_TYPE_0-1);
	InsertItem( 0, strTemp );

	//strTemp.LoadStringW(IDS_STRING_GRID_LINE_SOLID+ps.gridType);
	//SetItemText(0,1,strTemp);

	//strTemp.Format(L"%d",ps.labelSize);
	//SetItemText(0, 2, strTemp);

	//strTemp.Format(L"%d",ps.metricSize);
	//SetItemText(0, 3, strTemp);

	//strTemp.Format(L"%d",ps.bkgndC);
	//SetItemText(0, 4, strTemp);

	//strTemp.Format(L"%d",ps.metricC);
	//SetItemText(0, 5, strTemp);

	//strTemp.Format(L"%d",ps.labelC);
	//SetItemText(0, 6, strTemp);

	//for(int j=0;j<7;j++){
	//	AdjustWidth(this,j,0);
	//}


	return 0;
}


PlotSpec ListCtrlPS::GetD(void)
{

	PlotSpec ps;

	ps.gridType=this->GetChoice(0,1);
	CString strTemp;
	strTemp=GetItemText(0,2);
	ps.labelSize=_wtoi(strTemp.GetBuffer());

	strTemp=GetItemText(0,3);
	ps.metricSize=_wtoi(strTemp.GetBuffer());


	strTemp=GetItemText(0,4);
	ps.bkgndC=ps.borderC=_wtoi(strTemp.GetBuffer());

	strTemp=GetItemText(0,5);
	ps.metricC=ps.gridC=_wtoi(strTemp.GetBuffer());

	strTemp=GetItemText(0,6);
	ps.labelC=_wtoi(strTemp.GetBuffer());

	return ps;
}


//void ListCtrlPS::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//	ListCtrlA::OnLvnBeginlabeledit(pNMHDR,pResult);
//
//	*pResult = 0;
//}
