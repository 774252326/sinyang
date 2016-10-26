#include "StdAfx.h"
#include "ListCtrlC.h"
#include "ComboBoxA.hpp"
#include "ColorButtonA.hpp"

IMPLEMENT_DYNAMIC(ListCtrlC, ListCtrlB)

	ListCtrlC::ListCtrlC(void)
{
}


ListCtrlC::~ListCtrlC(void)
{
}

BEGIN_MESSAGE_MAP(ListCtrlC, ListCtrlB)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ListCtrlC::OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ListCtrlC::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlC::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &ListCtrlC::OnNMCustomdraw)
END_MESSAGE_MAP()



CComboBox * ListCtrlC::ComboItem(CPoint coord)
{
#define IDC_COMBOBOXINLISTVIEW 0x1235

	CStringList m_strList;
	for(size_t i=0;i<cbstr[coord.y].size();i++){
		m_strList.AddTail(cbstr[coord.y][i]);
	}
	CComboBox *pList;
	pList = new ComboBoxA(coord, &m_strList);

	CRect rect=GetBoundRect(coord.x, coord.y);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all
	pList->ShowDropDown();

	CString strFind = GetItemText(coord.x, coord.y);
	pList->SelectString(-1, strFind.GetBuffer(1));

	// The returned pointer should not be saved
	return pList;
}

CMFCColorButton * ListCtrlC::ColorItem(CPoint coord)
{
#define IDC_COLORINLISTVIEW 0x1236

	COLORREF color=_wtoi(GetItemText(coord.x, coord.y));

	CMFCColorButton *pList;
	pList = new ColorButtonA(coord);

	pList->SetColor(color);
	CRect rect;
	GetSubItemRect(coord.x, coord.y, LVIR_BOUNDS, rect);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | BS_FLAT |BS_PUSHBUTTON;
	pList->Create(L"",dwStyle, rect, this, IDC_COLORINLISTVIEW);

	// The returned pointer should not be saved
	return pList;
}


void ListCtrlC::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here



	switch(typelist[m_itemEdit.coord.y]){
	case eEdit:
		{
			ListCtrlB::BeginEdit(m_itemEdit);
		}
		break;
	case eEditReal:
		{
			m_itemEditReal.coord=m_itemEdit.coord;
			m_itemEditReal.min=mind[m_itemEdit.coord.y];
			m_itemEditReal.max=maxd[m_itemEdit.coord.y];
			ListCtrlB::BeginEdit(m_itemEditReal);
		}
		break;
	case eEditInt:
		{
			m_itemEditInt.coord=m_itemEdit.coord;
			m_itemEditInt.min=mini[m_itemEdit.coord.y];
			m_itemEditInt.max=maxi[m_itemEdit.coord.y];
			ListCtrlB::BeginEdit(m_itemEditInt);
		}
		break;
	case eCombo:
		break;
	case eColor:
		break;
	default:
		break;
	}



	*pResult = 0;
}


void ListCtrlC::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here


	switch(typelist[m_itemEdit.coord.y]){

	case eStatic:
		break;

	case eEdit:
		{
			EndEdit(m_itemEdit,pDispInfo->item.pszText);	
		}
		break;

	case eEditReal:
		{
			BOOL flg=UpdateData();
			if(flg==TRUE){						
				EndEdit(m_itemEditReal,pDispInfo->item.pszText);
				CString str;
				str.Format(L"%g",m_itemEditReal.x);
				SetItemText(m_itemEditReal.coord.x,m_itemEditReal.coord.y,str);
			}
		}
		break;

	case eEditInt:
		{
			BOOL flg=UpdateData();
			if(flg==TRUE){	
				EndEdit(m_itemEditInt,pDispInfo->item.pszText);
				CString str;
				str.Format(L"%d",m_itemEditInt.x);
				SetItemText(m_itemEditInt.coord.x,m_itemEditInt.coord.y,str);
			}
		}
		break;
	case eCombo:
		{
			if(pDispInfo->item.pszText!=NULL){
				SetItemText(pDispInfo->item.iItem, m_itemEdit.coord.y, pDispInfo->item.pszText);		
			}
		}
		break;
	case eColor:
		{
			if(pDispInfo->item.pszText!=NULL){
				SetItemText(pDispInfo->item.iItem, m_itemEdit.coord.y, pDispInfo->item.pszText);		
			}
		}
		break;
	default:
		break;
	}


	this->ModifyStyle(LVS_EDITLABELS,0);		
	*pResult = 0;
}


void ListCtrlC::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	switch(typelist[pNMItemActivate->iSubItem]){
	case eStatic:
		break;
	case eEdit:
	case eEditReal:
	case eEditInt:
		{
			m_itemEdit.coord.SetPoint(pNMItemActivate->iItem,pNMItemActivate->iSubItem);
			ModifyStyle(0,LVS_EDITLABELS);
			EditLabel(pNMItemActivate->iItem);
		}
		break;
	case eCombo:
		m_itemEdit.coord.SetPoint(pNMItemActivate->iItem,pNMItemActivate->iSubItem);
		ModifyStyle(0,LVS_EDITLABELS);
		//EditLabel(pNMItemActivate->iItem);
		ComboItem(m_itemEdit.coord);
		break;
	case eColor:
		{
			//COLORREF color=_wtoi(GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem));
			//CColorDialog colorDlg(color);  
			//if(IDOK == colorDlg.DoModal()){   
			//	color = colorDlg.GetColor();
			//	CString str;
			//	str.Format(L"%d",color);
			//	SetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem,str);
			//}
			m_itemEdit.coord.SetPoint(pNMItemActivate->iItem,pNMItemActivate->iSubItem);
			ModifyStyle(0,LVS_EDITLABELS);
			//EditLabel(pNMItemActivate->iItem);
			ColorItem(m_itemEdit.coord);
		}
		break;
	default:
		break;
	}

	*pResult = 0;
}

void ListCtrlC::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	////// TODO: Add your control notification handler code here

	//switch(pNMCD->dwDrawStage){
	//case CDDS_PREPAINT:
	//	*pResult = CDRF_NOTIFYITEMDRAW;
	//	break;
	//case CDDS_ITEMPREPAINT:
	//	*pResult = CDRF_NOTIFYSUBITEMDRAW;
	//	break;
	//case (CDDS_ITEMPREPAINT|CDDS_SUBITEM):
	//	{

	//	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	//	int nItem=static_cast<int>(pLVCD->nmcd.dwItemSpec );
	//	int a=pNMCD->dwItemSpec;

	//	ASSERT(nItem==a);

	//	if(typelist[pLVCD->iSubItem] == eColor){
	//		//COLORREF clrNewTextColor=_wtoi(GetItemText(nItem, pLVCD->iSubItem));
	//		//pLVCD->clrText =clrNewTextColor;

	//		COLORREF clrNewBkColor=_wtoi(GetItemText(nItem, pLVCD->iSubItem));
	//		pLVCD->clrTextBk =clrNewBkColor;
	//	}
	//	else{
	//		pLVCD->clrTextBk=this->GetBkColor();
	//	}
	//	
	//		//pNMCD->dwItemSpec

	//	*pResult = CDRF_DODEFAULT;
	//	}
	//	break;
	//default:		
	//	*pResult = 0;
	//	break;
	//}


	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	switch(pLVCD->nmcd.dwDrawStage){
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_SUBITEM|CDDS_ITEMPREPAINT:
		{
			int nItem=static_cast<int>(pLVCD->nmcd.dwItemSpec );
			if(typelist[pLVCD->iSubItem] == eColor){
				//COLORREF clrNewTextColor=_wtoi(GetItemText(nItem, pLVCD->iSubItem));
				//pLVCD->clrText =clrNewTextColor;

				COLORREF clrNewBkColor=_wtoi(GetItemText(nItem, pLVCD->iSubItem));
				pLVCD->clrTextBk =clrNewBkColor;
			}
			else{
				pLVCD->clrTextBk=this->GetBkColor();
			}		
			*pResult = CDRF_DODEFAULT;
		}
		break;
	default:
		*pResult = 0;
		break;
	}


}
