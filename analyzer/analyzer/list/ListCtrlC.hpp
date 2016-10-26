#pragma once
#include "listctrlb.hpp"
#include "EditC.hpp"
#include "ComboBoxA.hpp"
#include "ColorButtonA.hpp"

#include "../messagemaphpp.h"
///
/// \brief The ListCtrlC class
///增加下拉选框和颜色选框和不可编辑项的可编辑表格
///
class ListCtrlC :
	public ListCtrlB
{
	//DECLARE_DYNAMIC(ListCtrlC)

public:
	enum eType{
        eStatic,//不可编辑项
        eEdit,//文字编辑框
        eEditReal,//实数编辑框
        eEditInt,//整数编辑框
        eCombo,//下拉选框
        eColor	//颜色选框
	};

public:
    ///
    /// \brief typelist
    ///指定各列的单元格类型
	std::vector<eType> typelist;
    ///
    /// \brief cbstr
    ///各列的下拉选框内容
    /// 若某列的类型不是下拉选框时则不考虑
	std::vector< std::vector<CString> > cbstr;
    ///
    /// \brief mind
    ///各列的实数编辑框的输入数据下限
    /// 若该列的类型不是实数编辑框时则不考虑
	std::vector<double> mind;
    ///
    /// \brief maxd
    ///各列的实数编辑框的输入数据上限
    /// 若该列的类型不是实数编辑框时则不考虑
	std::vector<double> maxd;
    ///
    /// \brief mini
    /// 各列的整数编辑框的输入数据下限
    ///若该列的类型不是整数编辑框时则不考虑
	std::vector<int> mini;
    ///
    /// \brief maxi
    ///各列的整数编辑框的输入数据上限
    /// 若该列的类型不是整数编辑框时则不考虑
	std::vector<int> maxi;

    ///
    /// \brief m_itemEditInt
    ///整数编辑框
	EditC<int> m_itemEditInt;
    ///
    /// \brief m_itemEditReal
    ///实数编辑框
	EditC<double> m_itemEditReal;

public:
	ListCtrlC(void){};
	virtual ~ListCtrlC(void){};

    ///
    /// \brief ComboItem
    /// 由单元格产生下拉选框
    /// \param coord
    /// 单元格坐标
    /// \return
    ///
	CComboBox * ComboItem(CPoint coord)
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
	};

    ///
    /// \brief ColorItem
    /// 有单元格产生颜色选框
    /// \param coord
    /// 单元格坐标
    /// \return
    ///
	CMFCColorButton * ColorItem(CPoint coord)
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
	};

    ///
    /// \brief GetBoundRect
    /// 计算单元格位置和对应下拉选框的大小
    /// \param nItem
    ///
    /// \param nSubItem
    /// \return
    ///
	CRect GetBoundRect(int nItem, int nSubItem)
	{
		//basic code start
		CRect rect;

		// Make sure that the item is visible
		if( EnsureVisible(nItem, TRUE) )
		{
			GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
			// Now scroll if we need to expose the column
			CRect rcClient;
			GetClientRect(rcClient);
			int offset = 0;
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
		}
		return rect;

	};

    ///
    /// \brief GetChoice
    /// 求下拉选框当前选中项
    /// \param nItem
    /// \param nSubItem
    /// \return
    ///
	int GetChoice(int nItem, int nSubItem)
	{
		CString strTemp, strTemp2;
		strTemp=GetItemText(nItem,nSubItem);
		for(size_t i=0;i<cbstr[nSubItem].size();i++){
			if(cbstr[nSubItem][i]==strTemp){
				return i;
			}
		}
		return -1;
	};

    ///
    /// \brief SetChoice
    /// 设置单元格内容为下拉选框中的指定选项
    /// \param nItem
    /// \param nSubItem
    /// \param choiceIndex
    /// \return
    ///
	BOOL SetChoice(int nItem, int nSubItem, int choiceIndex)
	{
		if( nItem<0 || nSubItem<0 
			|| nSubItem>=cbstr.size() 
			|| choiceIndex<0 
			|| choiceIndex>=cbstr[nSubItem].size() ){
				return FALSE;
		}
		return SetItemText(nItem,nSubItem,cbstr[nSubItem][choiceIndex]);
	};

    ///
    /// \brief SetHeader
    /// 设置表头
    /// \param headerstrl
    /// \return
    ///
	BOOL SetHeader(const std::vector<CString> & headerstrl)
	{
		BOOL flg=ListCtrlB::SetHeader(headerstrl);
		if(flg==TRUE){
			size_t totaln=headerstrl.size();

			typelist.resize(totaln,eStatic);
			std::vector<CString> strl;
			cbstr.resize(totaln,strl);
			mind.resize(totaln,0);
			mini.resize(totaln,0);
			maxd.resize(totaln,-1);
			maxi.resize(totaln,-1);
		}
		return flg;
	};

protected:
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
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
	};


	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
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
            //实数值以字符形式保存
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
            //整数值以字符形式保存
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
            //颜色数值以字符形式保存
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
	};


	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
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
	};


	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult){
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
            ///将单元格字体和背景设为所指定颜色
            ///
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

	};



	virtual void DoDataExchange(CDataExchange* pDX)
	{
		// TODO: Add your specialized code here and/or call the base class
		if(m_itemEditInt.GetSafeHwnd())
		{
			m_itemEditInt.DoDataExchange(pDX);
		}
		if(m_itemEditReal.GetSafeHwnd())
		{
			m_itemEditReal.DoDataExchange(pDX);
		}

		ListCtrlB::DoDataExchange(pDX);
	};


	BEGIN_MESSAGE_MAP_HPP(ListCtrlC, ListCtrlB)
		ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ListCtrlC::OnLvnBeginlabeledit)
		ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ListCtrlC::OnLvnEndlabeledit)
		ON_NOTIFY_REFLECT(NM_DBLCLK, &ListCtrlC::OnNMDblclk)
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &ListCtrlC::OnNMCustomdraw)
		END_MESSAGE_MAP_HPP()

};

