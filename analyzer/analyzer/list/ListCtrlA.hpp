#pragma once

#include <vector>
// ListCtrlA

///
/// \brief The ListCtrlA class
///�����

class ListCtrlA : public CListCtrl
{
	//DECLARE_DYNAMIC(ListCtrlA)

public:
	ListCtrlA(){};
	virtual ~ListCtrlA(){};
    ///
    /// \brief AdjustWidth
    /// �������ֵ����п�
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
    /// ����ָ����Ԫ������п�
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
    /// ���ñ�ͷ
    /// \param headerstrl
    /// ��ͷ����
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
    ///���ñ����
	void SetStyle()
	{
		ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS);

		LONG IStyle;
		IStyle=GetWindowLong(GetSafeHwnd(), GWL_STYLE);//��ȡ��ǰ����style
		IStyle&= ~LVS_TYPEMASK; //�����ʾ��ʽλ
		IStyle|= LVS_REPORT; //set style
		SetWindowLong(GetSafeHwnd(), GWL_STYLE, IStyle);//����style

		DWORD dwStyle1;
		dwStyle1 = GetExtendedStyle();
		dwStyle1 |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
		dwStyle1 |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
		//dwStyle1 |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
		SetExtendedStyle(dwStyle1); //������չ���

	};
	//protected:
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//DECLARE_MESSAGE_MAP()
};


