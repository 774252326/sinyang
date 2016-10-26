#include "StdAfx.h"
#include "ListCtrlSAP.h"
#include "func.h"
#include "resource.h"

ListCtrlSAP::ListCtrlSAP(void)
{
}


ListCtrlSAP::~ListCtrlSAP(void)
{
}
BEGIN_MESSAGE_MAP(ListCtrlSAP, ListCtrlA)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, &ListCtrlSAP::OnLvnKeydown)
END_MESSAGE_MAP()


int ListCtrlSAP::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	dwStyle1 |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle1 |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle1 |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	SetExtendedStyle(dwStyle1); //设置扩展风格

	CString strTemp;
	std::vector<CString> strl;
	for(int i=IDS_STRING520;i<=IDS_STRING526;i++){
		strTemp.LoadStringW(i);
		strl.push_back(strTemp);
	}
	this->SetHeader(strl);
	typelist[1]=eCombo;
	typelist[2]=typelist[3]=typelist[4]=typelist[5]=typelist[6]=eEditReal;
	maxd[2]=maxd[3]=maxd[4]=maxd[5]=maxd[6]=1000;

	// insert string elements  for the ComboBox : 
	for ( int j=IDS_STRING_VOL_ONCE ; j <= IDS_STRING_VMS ; j++){
		strTemp.LoadStringW(j);
		cbstr[1].push_back(strTemp);
		AdjustWidth(this,1,strTemp);
	}

	return 0;
}


int ListCtrlSAP::Inserti(int iSelect)
{

	iSelect++;
	CString str;
	str.Format(L"%d",iSelect+1);
	iSelect=InsertItem( iSelect, str );
	EnsureVisible(iSelect, FALSE);
	for(iSelect++;iSelect<GetItemCount();iSelect++){
		str.Format(L"%d",iSelect+1);
		SetItemText(iSelect,0,str);
	}


	return 0;
}


int ListCtrlSAP::Deletei(int iSelect)
{

	if(iSelect!=-1) // valid item 	
	{
		DeleteItem( iSelect );
		EnsureVisible(iSelect, FALSE);
		for(;iSelect<GetItemCount();iSelect++){
			CString str;
			str.Format(L"%d",iSelect+1);
			SetItemText(iSelect,0,str);
		}
	}

	return 0;
}


void ListCtrlSAP::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here


	switch(pLVKeyDow->wVKey){
	case VK_DELETE:
		Deletei(GetSelectionMark());
		break;
	case VK_INSERT: 
		Inserti(GetSelectionMark());
		break;
	default:
		break;
	}


	*pResult = 0;
}
