// EditList.cpp : implementation file
//

#include "stdafx.h"
#include "ComboItem.h"
#include "EditItem.h"
#include "EditList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditList

CEditList::CEditList()
{
	m_fGetType = NULL;
}

CEditList::~CEditList()
{
}


BEGIN_MESSAGE_MAP(CEditList, CListCtrl)
	//{{AFX_MSG_MAP(CEditList)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CEditList::OnNMCustomdraw)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditList message handlers

CEdit *CEditList::EditItem(int nItem, int nSubItem)
{
	CRect rect;
	int offset = 0;
	if(!EnsureVisible(nItem, TRUE))
	{ 
		return NULL;	
	}

	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size(offset + rect.left,0);		
		Scroll(size);
		rect.left -= size.cx;
	}
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nSubItem);
	if(rect.right > rcClient.right) 
		rect.right = rcClient.right;

	// Get Column alignment	
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nSubItem, &lvcol);

	DWORD dwStyle;
	if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else 
		dwStyle = ES_CENTER;	


	dwStyle |=WS_VISIBLE 
		| WS_BORDER 
		| WS_CHILD 
		//| ES_NUMBER
		| ES_AUTOHSCROLL;

	//if(nSubItem==0){
	//	dwStyle |=ES_READONLY;
	//}

	CEdit *pEdit = new CEditItem(nItem, nSubItem, GetItemText(nItem, nSubItem));

	////////////////add//////////////////
	((CEditItem*)pEdit)->typelimit=typelimit[nSubItem];
	////////////////add//////////////////

#define IDC_EDITCTRL 0x1234
	pEdit->Create(dwStyle, rect, this, IDC_EDITCTRL);	
	//	pEdit->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	//funny thing happend here, uncomment this, 
	//and then edit an item, 
	//enter a long text so that the ES_AUTOHSCROLL comes to rescue
	//yes that's look funny, ???.
	return pEdit;
}

void CEditList::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM *plvItem = &plvDispInfo->item;

	if( plvItem->iItem != -1 &&  // valid item
		plvItem->pszText )		// valid text
	{
		SetItemText( plvItem->iItem, plvItem->iSubItem, plvItem->pszText);				

		// this will invoke an ItemChanged handler in parent
		if ( plvItem->iSubItem != 0 )
		{
			wchar_t szText[0x10+1];
			GetItemText( plvItem->iItem, 0, szText, 0x10);				
			SetItemText( plvItem->iItem, 0, szText);				
		}	
	}

	*pResult = 0;
}


void genstrlist(int i1, int i2, const CStringList &source, CStringList &dst)
{
	if(dst.IsEmpty()==FALSE)
		dst.RemoveAll();

	for(int i=i1;i<i2;i++){
		dst.AddTail(source.GetAt(source.FindIndex(i)));
	}
}

void CEditList::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if( GetFocus() != this) 
		SetFocus();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iSubItem > 0)
	{
		if ( m_fGetType && m_fGetType( pNMListView->iSubItem ) == eCombo ){

			if(pNMListView->iSubItem==0){
				genstrlist(0,cols[0],allComboStr,m_strList);
			}
			else{
				genstrlist(cols[pNMListView->iSubItem-1],cols[pNMListView->iSubItem],allComboStr,m_strList);
			}
			ComboItem(pNMListView->iItem, pNMListView->iSubItem);
		}
		else{
			if ( m_fGetType && m_fGetType( pNMListView->iSubItem ) == eEdit ){
				EditItem (pNMListView->iItem, pNMListView->iSubItem);
			}
			else{
				COLORREF color=_wtoi(GetItemText(pNMListView->iItem, pNMListView->iSubItem));
				CColorDialog colorDlg(color);  
				if(IDOK == colorDlg.DoModal()){   
					color = colorDlg.GetColor();
					CString str;
					str.Format(L"%d",color);
					SetItemText(pNMListView->iItem, pNMListView->iSubItem,str);
				}
			}
		}
	}

	*pResult = 0;
}

BOOL CEditList::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CHILD |
		LVS_REPORT |
		LVS_SINGLESEL |
		LVS_SHOWSELALWAYS
		;	
	return CListCtrl::PreCreateWindow(cs);
}



void CEditList::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	switch(pLVKeyDow->wVKey)
	{
	case VK_DELETE: 
		{
			int nItem = GetSelectionMark();
			if(nItem!=-1) // valid item 	
				//if( nItem > 0 )
			{
				DeleteItem( nItem );
				EnsureVisible(nItem, FALSE);
				for(;nItem<GetItemCount();nItem++){
					CString str;
					str.Format(L"%d",nItem+1);
					SetItemText(nItem,0,str);
				}
			}
		}	break;

	case VK_INSERT: 
		{
			int nItem;
			//nItem=GetItemCount();
			//if(nItem<1){
			//	CString str;
			//	str.Format(L"%d",nItem+1);
			//	InsertItem( nItem, str );
			//	EnsureVisible(nItem, FALSE);
			//}
			//else{


			nItem = GetSelectionMark();
			//if(nItem!=-1) // valid item 					
			{
				//DeleteItem( nItem );
				//nItem=GetItemCount();

				nItem++;

				CString str;
				str.Format(L"%d",nItem+1);
				InsertItem( nItem, str );
				EnsureVisible(nItem, FALSE);


				for(nItem++;nItem<GetItemCount();nItem++){
									//CString str;
				str.Format(L"%d",nItem+1);
					this->SetItemText(nItem,0,str);
				}
			//}
			}

		}	break;

	default:
		break;
	}
	*pResult = 0;

}

int CEditList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here	
	SetExtendedStyle( GetExtendedStyle() /*| LVS_EX_CHECKBOXES*/ );

	return 0;
}

CComboBox * CEditList::ComboItem(int nItem, int nSubItem)
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

	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | /*WS_VSCROLL | WS_HSCROLL|*/CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;

	CComboBox *pList;

	pList = new CComboItem(nItem, nSubItem, &m_strList);
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all

	pList->ShowDropDown();
	pList->SelectString(-1, strFind.GetBuffer(1));

	// The returned pointer should not be saved
	return pList;
}



int CEditList::GetChoice(int nItem, int nSubItem)
{
	CString strTemp, strTemp2;
	strTemp=GetItemText(nItem,nSubItem);

	int firsti= (nSubItem>0) ? cols[nSubItem-1] : 0;	

	for(int i=firsti;i<cols[nSubItem];i++){
		strTemp2=allComboStr.GetAt(allComboStr.FindIndex(i));
		if(strTemp2==strTemp)
			return i-firsti;
	}
	return -1;
}


void CEditList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	if ( CDDS_PREPAINT ==pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the notification message for an item. We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		int nItem=static_cast<int>(pLVCD->nmcd.dwItemSpec );

		if(m_fGetType( pLVCD->iSubItem ) == eLast){
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

}


void CEditList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	//this->Invalidate();

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CEditList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	//this->Invalidate();

	//HWND hWnd= (HWND)SendMessage(LVN_ENDLABELEDIT);

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
