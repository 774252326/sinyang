#include "StdAfx.h"
#include "ListCtrlDOA.h"
#include "func.h"

ListCtrlDOA::ListCtrlDOA(void)
{
}


ListCtrlDOA::~ListCtrlDOA(void)
{
}
BEGIN_MESSAGE_MAP(ListCtrlDOA, ListCtrlA)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int ListCtrlDOA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ListCtrlA::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	CString strTemp;


	CString hl[]={L"file",
		L"base vol.(ml)", 
		L"add vol.(ml)",
		L"additive vol.(ml)",
		L"total vol.(ml)",
		L"S conc.(ml/L)",
		L"A conc.(ml/L)",
		L"L conc.(ml/L)",
		L"Q(mC)",
		L"Q0(mC)",
		L"nQ"};

	std::vector<CString> strl(hl,hl+11);

	this->SetHeader(strl);

	return 0;
}


int ListCtrlDOA::InsertItemDOA(int i, const DataOutA & doa)
{

	CString strTemp;

	int ri=InsertItem( i, doa.stepName );
	AdjustWidth(this,0,doa.stepName);

	strTemp.Format(L"%g",doa.VMSVolume);
	SetItemText(i,1,strTemp);
	AdjustWidth(this,1,strTemp);

	strTemp.Format(L"%g",doa.addVolume);
	SetItemText(i,2,strTemp);
	AdjustWidth(this,2,strTemp);

	strTemp.Format(L"%g",doa.additiveVolume);
	SetItemText(i,3,strTemp);
	AdjustWidth(this,3,strTemp);

	strTemp.Format(L"%g",doa.TotalVolume());
	SetItemText(i,4,strTemp);
	AdjustWidth(this,4,strTemp);

	strTemp.Format(L"%g",doa.SConc());
	SetItemText(i,5,strTemp);
	AdjustWidth(this,5,strTemp);

	strTemp.Format(L"%g",doa.AConc());
	SetItemText(i,6,strTemp);
	AdjustWidth(this,6,strTemp);

	strTemp.Format(L"%g",doa.LConc());
	SetItemText(i,7,strTemp);
	AdjustWidth(this,7,strTemp);

	strTemp.Format(L"%g",doa.ArUse());
	SetItemText(i,8,strTemp);
	AdjustWidth(this,8,strTemp);

	strTemp.Format(L"%g",doa.Ar0);
	SetItemText(i,9,strTemp);
	AdjustWidth(this,9,strTemp);

	strTemp.Format(L"%g",doa.ArUse()/doa.Ar0);
	SetItemText(i,10,strTemp);
	AdjustWidth(this,10,strTemp);


	return ri;

	return 0;
}
