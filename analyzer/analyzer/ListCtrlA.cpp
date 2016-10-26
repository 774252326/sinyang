// ListCtrlA.cpp : implementation file
//

#include "stdafx.h"
//#include "edls.h"
#include "ListCtrlA.h"
#include "func.h"

// ListCtrlA

IMPLEMENT_DYNAMIC(ListCtrlA, CListCtrl)

	ListCtrlA::ListCtrlA()
	: m_iSubItem(0)
{

}

ListCtrlA::~ListCtrlA()
{
}


BEGIN_MESSAGE_MAP(ListCtrlA, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ListCtrlA::OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ListCtrlA::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlA::OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &ListCtrlA::OnNMCustomdraw)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// ListCtrlA message handlers




void ListCtrlA::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here


	if (m_iSubItem >= 0)
	{	
		//get edit control and subclass
		HWND hWnd= (HWND)SendMessage(LVM_GETEDITCONTROL);
		ASSERT(hWnd != NULL);

		//CRect  rcSubItem;
		//GetSubItemRect( pDispInfo->item.iItem, m_iSubItem, LVIR_LABEL, rcSubItem);
		//CString str=GetItemText(pDispInfo->item.iItem, m_iSubItem);		

		switch(typelist[m_iSubItem]){
		case eEdit:
			{
				VERIFY(m_itemEdit.SubclassWindow(hWnd));
				GetSubItemRect( pDispInfo->item.iItem, m_iSubItem, LVIR_LABEL, m_itemEdit.rect);
				//m_itemEdit.rect=rcSubItem;	
				//CString str=GetItemText(pDispInfo->item.iItem, m_iSubItem);							
				m_itemEdit.SetWindowText( GetItemText(pDispInfo->item.iItem, m_iSubItem) );
			}
			break;
		case eEditReal:
			{
				VERIFY(m_itemEditReal.SubclassWindow(hWnd));
				//m_itemEditReal.rect=rcSubItem;
				//m_itemEditReal.SetWindowText(str);
				m_itemEditReal.min=mind[m_iSubItem];
				m_itemEditReal.max=maxd[m_iSubItem];
				GetSubItemRect( pDispInfo->item.iItem, m_iSubItem, LVIR_LABEL, m_itemEditReal.rect);
				m_itemEditReal.SetWindowText( GetItemText(pDispInfo->item.iItem, m_iSubItem) );
			}
			break;
		case eEditInt:
			{
				VERIFY(m_itemEditInt.SubclassWindow(hWnd));
				//m_itemEditInt.rect=rcSubItem;
				//m_itemEditInt.SetWindowText(str);
				m_itemEditInt.min=mini[m_iSubItem];
				m_itemEditInt.max=maxi[m_iSubItem];
				GetSubItemRect( pDispInfo->item.iItem, m_iSubItem, LVIR_LABEL, m_itemEditInt.rect);
				m_itemEditInt.SetWindowText( GetItemText(pDispInfo->item.iItem, m_iSubItem) );
			}
			break;
		case eEditNum:
			{
				VERIFY(m_itemEditNum.SubclassWindow(hWnd));
				//m_itemEditNum.rect=rcSubItem;
				//m_itemEditNum.SetWindowText(str);
				m_itemEditNum.min=mini[m_iSubItem];
				m_itemEditNum.max=maxi[m_iSubItem];
				GetSubItemRect( pDispInfo->item.iItem, m_iSubItem, LVIR_LABEL, m_itemEditNum.rect);
				m_itemEditNum.SetWindowText( GetItemText(pDispInfo->item.iItem, m_iSubItem) );
			}
			break;
		default:
			break;
		}


	}



	*pResult = 0;
}


void ListCtrlA::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here

	if (m_iSubItem >= 0)
	{
		switch(typelist[m_iSubItem]){

		case eStatic:
			break;

		case eEdit:
			{
				if(pDispInfo->item.pszText!=NULL		
					){						
						SetItemText(pDispInfo->item.iItem, m_iSubItem, pDispInfo->item.pszText);	
				}
				VERIFY(m_itemEdit.UnsubclassWindow()!=NULL);	
				this->ModifyStyle(LVS_EDITLABELS,0);
			}
			break;

		case eEditReal:
			{
				if(pDispInfo->item.pszText!=NULL 
					//&& flg==TRUE
					){

						BOOL flg=UpdateData();
						if(flg==TRUE){	
							CString str;							
							str.Format(L"%g",aa);		

							//m_itemEditReal.GetWindowTextW(str);
							SetItemText(pDispInfo->item.iItem, m_iSubItem, str);
							//SetItemText(pDispInfo->item.iItem, m_iSubItem, pDispInfo->item.pszText);	
						}	

				}
				VERIFY(m_itemEditReal.UnsubclassWindow()!=NULL);	
				this->ModifyStyle(LVS_EDITLABELS,0);			
			}
			break;

		case eEditInt:
			{
				if(pDispInfo->item.pszText!=NULL 
					//&& flg==TRUE
					){

						BOOL flg=UpdateData();
						if(flg==TRUE){							

							CString str;
							str.Format(L"%d",bb);						
							//m_itemEditReal.GetWindowTextW(str);
							SetItemText(pDispInfo->item.iItem, m_iSubItem, str);
							//SetItemText(pDispInfo->item.iItem, m_iSubItem, pDispInfo->item.pszText);	
						}	

				}
				VERIFY(m_itemEditInt.UnsubclassWindow()!=NULL);	
				this->ModifyStyle(LVS_EDITLABELS,0);			
			}
			break;
		case eEditNum:
			{
				if(pDispInfo->item.pszText!=NULL 
					//&& flg==TRUE
					){

						BOOL flg=UpdateData();
						if(flg==TRUE){							
							//str.Format(L"%g",aa);		
							//CString str;					
							//m_itemEditReal.GetWindowTextW(str);
							//SetItemText(pDispInfo->item.iItem, m_iSubItem, str);
							SetItemText(pDispInfo->item.iItem, m_iSubItem, pDispInfo->item.pszText);	
						}	

				}
				VERIFY(m_itemEditNum.UnsubclassWindow()!=NULL);	
				this->ModifyStyle(LVS_EDITLABELS,0);			
			}
			break;

		case eCombo:
			{
				if(pDispInfo->item.pszText!=NULL){
					SetItemText(pDispInfo->item.iItem, m_iSubItem, pDispInfo->item.pszText);		
				}
			}
			break;
		case eColor:
			break;
		default:
			break;
		}
	}

	*pResult = 0;
}


void ListCtrlA::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	m_iSubItem=pNMItemActivate->iSubItem;

	switch(typelist[m_iSubItem]){
	case eStatic:
		break;
	case eEdit:
	case eEditReal:
	case eEditInt:
	case eEditNum:
		{
			this->ModifyStyle(0,LVS_EDITLABELS);
			this->EditLabel(pNMItemActivate->iItem);
		}
		break;
	case eCombo:
		ComboItem(pNMItemActivate->iItem, pNMItemActivate->iSubItem);
		break;
	case eColor:
		{
			COLORREF color=_wtoi(GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem));
			CColorDialog colorDlg(color);  
			if(IDOK == colorDlg.DoModal()){   
				color = colorDlg.GetColor();
				CString str;
				str.Format(L"%d",color);
				SetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem,str);
			}
		}
		break;
	default:
		break;
	}

	*pResult = 0;
}


CComboBox * ListCtrlA::ComboItem(int nItem, int nSubItem)
{
#define IDC_COMBOBOXINLISTVIEW 0x1235

	CString strFind = GetItemText(nItem, nSubItem);

	//basic code start
	CRect rect;
	int offset = 0;
	// Make sure that the item is visible
	if( !EnsureVisible(nItem, TRUE)) 
		return NULL;

	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nSubItem);
	if(rect.right > rcClient.right) 
		rect.right = rcClient.right;
	//basic code end

	rect.bottom += 30 * rect.Height();//dropdown area

	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;

	CComboBox *pList;

	CStringList m_strList;
	for(size_t i=0;i<cbstr[nSubItem].size();i++){
		m_strList.AddTail(cbstr[nSubItem][i]);
	}

	pList = new CComboItem(nItem, nSubItem, &m_strList);
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all

	pList->ShowDropDown();
	pList->SelectString(-1, strFind.GetBuffer(1));

	// The returned pointer should not be saved
	return pList;
}



int ListCtrlA::GetChoice(int nItem, int nSubItem)
{
	CString strTemp, strTemp2;
	strTemp=GetItemText(nItem,nSubItem);


	//if(!cbstr[nSubItem].empty()){
	for(size_t i=0;i<cbstr[nSubItem].size();i++){
		if(cbstr[nSubItem][i]==strTemp){
			return i;
		}
	}



	return -1;
}


void ListCtrlA::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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


void ListCtrlA::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_itemEditReal.GetSafeHwnd()!= NULL){
		DDX_Text(pDX, m_itemEditReal.GetDlgCtrlID(), aa);
		if(m_itemEditReal.min<=m_itemEditReal.max)
			DDV_MinMaxDouble(pDX,aa,m_itemEditReal.min,m_itemEditReal.max);
	}


	if(m_itemEditInt.GetSafeHwnd()!= NULL){
		DDX_Text(pDX, m_itemEditInt.GetDlgCtrlID(), bb);
		if(m_itemEditInt.min<=m_itemEditInt.max)
			DDV_MinMaxInt(pDX,bb,m_itemEditInt.min,m_itemEditInt.max);
	}


	if(m_itemEditNum.GetSafeHwnd()!= NULL){
		DDX_Text(pDX, m_itemEditNum.GetDlgCtrlID(), bb);
		if(m_itemEditNum.min<=m_itemEditNum.max)
			DDV_MinMaxInt(pDX,bb,m_itemEditNum.min,m_itemEditNum.max);
	}


	CListCtrl::DoDataExchange(pDX);
}


BOOL ListCtrlA::SetHeader(const std::vector<CString> & headerstrl)
{

	size_t totaln=headerstrl.size();

	if(totaln<1)
		return FALSE;


	//this->DeleteAllItems();

	int nColumnCount = GetHeaderCtrl()->GetItemCount();

	// Delete all of the columns.

	for (int i=0; i < nColumnCount; i++)
	{
		DeleteColumn(0);
	}


	typelist.assign(totaln,eStatic);

	std::vector<CString> strl;
	this->cbstr.assign(totaln,strl);

	this->mind.assign(totaln,0);
	this->mini.assign(totaln,0);
	this->maxd.assign(totaln,-1);
	this->maxi.assign(totaln,-1);


	for(int i=0;i<totaln;i++){		
		InsertColumn(i, headerstrl[i], LVCFMT_LEFT);
		AdjustWidth(this,i,headerstrl[i]);
	}


	return TRUE;
}


int ListCtrlA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
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


	return 0;
}


BOOL ListCtrlA::SetChoice(int nItem, int nSubItem, int choiceIndex)
{

	if( nItem<0 || nSubItem<0 || nSubItem>=cbstr.size() || choiceIndex<0 || choiceIndex>=cbstr[nSubItem].size() ){
		return FALSE;
	}

	return SetItemText(nItem,nSubItem,cbstr[nSubItem][choiceIndex]);

	return 0;
}
