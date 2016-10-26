#pragma once

#include <vector>
// ListCtrlA

///
/// \brief The ListCtrlA class
///表格类

class ListCtrlA : public CListCtrl
{
	//DECLARE_DYNAMIC(ListCtrlA)

public:
	ListCtrlA(){};
	virtual ~ListCtrlA(){};
    ///
    /// \brief AdjustWidth
    /// 根据文字调整列宽
    /// \param nCol
    /// \param str
    /// \param gap
    /// \return
    ///
    BOOL AdjustWidth(int nCol, CString str, int gap=15)
    {
        int widthc,widtht;
        widtht=GetStringWidth(str)+gap;
        widthc=GetColumnWidth(nCol);
        if(widthc<widtht)
            return SetColumnWidth(nCol,widtht);
        return FALSE;
    };

    ///
    /// \brief AdjustWidth
    /// 根据指定单元格调整列宽
    /// \param nCol
    /// \param nRow
    /// \param gap
    /// \return
    ///
    BOOL AdjustWidth(int nCol, int nRow, int gap=15)
    {
        CString	str=GetItemText(nRow,nCol);
        return AdjustWidth(nCol, str, gap);
    };


    ///
    /// \brief SetHeader
    /// 设置表头
    /// \param headerstrl
    /// 表头内容
    /// \return
    ///
	BOOL SetHeader(const std::vector<CString> & headerstrl)
	{
		int totaln=headerstrl.size();

		if(totaln<1)
			return FALSE;

		int nColumnCount = GetHeaderCtrl()->GetItemCount();

		for(int i=0;i<totaln;i++){

			if(i<nColumnCount){
				LVCOLUMN col;
				col.mask = LVCF_TEXT;
				col.cchTextMax=MAX_PATH;
				col.pszText=new TCHAR[col.cchTextMax];						
				CString str=headerstrl[i];
				GetColumn(i,&col);
				CString::CopyChars(col.pszText,str.GetBuffer(),str.GetLength()+1);
				SetColumn(i,&col);					
				delete [] col.pszText;
			}
			else{
				InsertColumn(i, headerstrl[i], LVCFMT_LEFT);
			}
			AdjustWidth(i,headerstrl[i]);
		}

		while(GetHeaderCtrl()->GetItemCount()>totaln){
			DeleteColumn(totaln);

		}

		return TRUE;
	};


    ///
    /// \brief SetStyle
    ///设置表格风格
	void SetStyle()
	{
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

	};
	//protected:
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//DECLARE_MESSAGE_MAP()
};


