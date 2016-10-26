#pragma once
#include "afxwin.h"
#include <vector>
#include "../struct\Value.hpp"
#include "../list/EditC.hpp"

#include "../messagemaphpp.h"

// ParaList
///
/// \brief The ParaList class
///公式变量的可编辑表格
///
class ParaList : public CListCtrl
{
	//DECLARE_DYNAMIC(ParaList)

public:
    ///
    /// \brief ScaleBitmap
    /// 缩放位图
    /// \param pSrc
    /// 原图像
    /// \param pDst
    /// 输出图像
    /// \param dstW
    /// 输出图像宽度
    /// \param dstH
    /// 输出图像高度
    ///
	static void ScaleBitmap(CBitmap *pSrc, CBitmap *pDst, int dstW, int dstH)
	{
		CDC srcDC;
		srcDC.CreateCompatibleDC(NULL); 
		srcDC.SelectObject(pSrc);

		pDst->CreateCompatibleBitmap(&srcDC,dstW,dstH);

		CDC dstDC ;
		dstDC.CreateCompatibleDC(NULL);
		dstDC.SelectObject(pDst);

		BITMAP bm;
		pSrc->GetBitmap(&bm); 
		dstDC.StretchBlt(0,0,dstW,dstH,&srcDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	};
    ///
    /// \brief AdjustWidth
    /// 根据文字调整表格列宽
    /// \param ls
    /// 表格
    /// \param nCol
    /// 列号
    /// \param str
    /// 文字
    /// \param gap
    /// 单元格边沿留空宽度
    ///
	static void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap=42)
	{
		int widthc,widtht;
		widtht=ls->GetStringWidth(str)+gap;
		widthc=ls->GetColumnWidth(nCol);
		if(widthc<widtht)
			ls->SetColumnWidth(nCol,widtht);
	};

protected:
    ///
    /// \brief imList
    ///图像列表
	CImageList imList;

    ///
    /// \brief m_iSubItem
    ///列号
	int m_iSubItem;
    ///
    /// \brief m_edtItemEdit
    ///数值编辑框
	EditC<double> m_edtItemEdit;
public:
    ///
    /// \brief vl
    ///参数列表
	std::vector<Value> vl;
    ///
    /// \brief x
    ///编辑数值时用的临时变量
	double x;
    ///
    /// \brief rowH
    ///行高
	int rowH;
    ///
    /// \brief firstColW
    ///第一列列宽
	int firstColW;
public:
	ParaList()
		: rowH(18)
		, firstColW(39)
	{
	};
	virtual ~ParaList(){};


    ///
    /// \brief Refresh
    ///根据数据vl自动填充表格
	void Refresh(void)
	{
		this->DeleteAllItems();
		imList.DeleteImageList();

		//二、添加图像列表
		//1、在类中添加成员变量	CImageList m_imList;
		//2、为图像列表添加图片

		//Bitmap图片(应该为48X48大小)
		imList.Create(firstColW,rowH,ILC_COLOR,0,20);

		CBitmap * pBmp = NULL;
		CBitmap * dst=NULL;

		for(size_t i=0;i<vl.size();i++){
			pBmp = new CBitmap();
			pBmp->LoadBitmapW(vl[i].LogoId);
			dst=new CBitmap();
			ScaleBitmap(pBmp,dst,firstColW,rowH);
			imList.Add(dst,RGB(0,0,0));
			delete dst;
			delete pBmp;
		}

		////ICON模式（方便一点）
		//HICON hIcon;

		//m_imList.Create(32,32,ILC_COLOR,2,20);

		//hIcon = AfxGetApp()->LoadIconW(IDI_ICON_GREE);
		//m_imList.Add(hIcon);

		//hIcon = AfxGetApp()->LoadIconW(IDI_ICON_READ);
		//m_imList.Add(hIcon);

		//三、设置图像列表与CListCtrl控件关联，要使用LVSIL_SMALL风格
		SetImageList(&imList,LVSIL_SMALL ); //这里要使用，LVSIL_SMALL风格


		//五、添加数据

		int nRow;  //记录行号
		LVITEM lvItem={0};

		for(size_t i=0;i<vl.size();i++){
			//第一行数据
			lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;	//文字、图片、状态
			lvItem.iItem = i;		//行号(第一行)
			lvItem.iImage = i;	//图片索引号(第一幅图片)
			lvItem.iSubItem = 0;	//子列号

			nRow = InsertItem(&lvItem);		//第一列为图片
			CString str;
			str.Format(L"%g",vl[i].raw);
			SetItemText(nRow,1,str);  //第二列为文字
			AdjustWidth(this,1,str);
			str.Format(L"%g",vl[i].correction);
			SetItemText(nRow,2,str);  //第二列为文字
			AdjustWidth(this,2,str);
			str.Format(L"%g",vl[i].Output());
			SetItemText(nRow,3,str);  //第二列为文字
			AdjustWidth(this,3,str);
		}

	};

protected:
    ///布局控件并显示数据

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CListCtrl::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		//CListCtrl控件，Report风格列表中添加图片

		ModifyStyle(0,LVS_EDITLABELS|
			LVS_REPORT
			//|LVS_SHOWSELALWAYS
			);

		//一、设置CListCtrl控件扩展风格(网络线、整行选中、图片)
		DWORD dwStyle;

		dwStyle = GetExtendedStyle();
		dwStyle = dwStyle
			|LVS_EX_FULLROWSELECT
			|LVS_EX_GRIDLINES
			|LVS_EX_SUBITEMIMAGES ;
		SetExtendedStyle(dwStyle);


		//四、给CListCtrl控件添加二列
		InsertColumn(0,_T(""),LVCFMT_CENTER,firstColW+4);

		CString str;
		str.LoadStringW(IDS_STRING_RAW);
		InsertColumn(1,str,LVCFMT_CENTER);
		AdjustWidth(this,1,str);

		str.LoadStringW(IDS_STRING_CORRECTION);
		InsertColumn(2,str,LVCFMT_CENTER);
		AdjustWidth(this,2,str);

		str.LoadStringW(IDS_STRING_OUTPUT);
		InsertColumn(3,str,LVCFMT_CENTER);
		AdjustWidth(this,3,str);
		//五、添加数据

		Refresh();


		return 0;
	};

//    开始编辑
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
	{
		NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
		// TODO: Add your control notification handler code here



		if (m_iSubItem >= 0)
			//if(pDispInfo->item.iSubItem>=0)
		{			
			//get edit control and subclass
			HWND hWnd= (HWND)SendMessage(LVM_GETEDITCONTROL);
			ASSERT(hWnd != NULL);
			VERIFY(m_edtItemEdit.SubclassWindow(hWnd));

			CRect  rcSubItem;
			GetSubItemRect( pDispInfo->item.iItem, m_iSubItem, LVIR_LABEL, rcSubItem);
			m_edtItemEdit.rect=rcSubItem;
			m_edtItemEdit.SetWindowText( GetItemText(pDispInfo->item.iItem, m_iSubItem) );

			m_edtItemEdit.ModifyStyle(NULL,ES_CENTER);
		}



		*pResult = 0;
	};

//    单击表格
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		// TODO: Add your control notification handler code here

		if(pNMItemActivate->iSubItem==2){

			m_iSubItem=pNMItemActivate->iSubItem;
			EditLabel(pNMItemActivate->iItem);  
		}


		*pResult = 0;
	};

//    结束编辑
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
	{
		NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
		// TODO: Add your control notification handler code here


		if (m_iSubItem >= 0)
		{
			if(pDispInfo->item.pszText!=NULL){

				if(UpdateData())
				{
					SetItemText(pDispInfo->item.iItem, m_iSubItem, pDispInfo->item.pszText);
					vl[pDispInfo->item.iItem].correction=x;
					CString str;
					str.Format(L"%g",vl[pDispInfo->item.iItem].Output());
					SetItemText(pDispInfo->item.iItem, 3, str);
				}
			}
			VERIFY(m_edtItemEdit.UnsubclassWindow()!=NULL);	
		}
		*pResult = 0;
	};

//    双击单元格
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		// TODO: Add your control notification handler code here

		//m_iSubItem=pNMItemActivate->iSubItem;
		//EditLabel(pNMItemActivate->iItem);  

		*pResult = 0;
	};

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		// TODO: Add your specialized code here and/or call the base class

		DDX_Text(pDX, m_edtItemEdit.GetDlgCtrlID(), x);


		CListCtrl::DoDataExchange(pDX);
	};

	
	//DECLARE_MESSAGE_MAP()
//};
//
//BEGIN_MESSAGE_MAP(ParaList, CListCtrl)
//	ON_WM_CREATE()
//	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ParaList::OnLvnBeginlabeledit)
//	ON_NOTIFY_REFLECT(NM_CLICK, &ParaList::OnNMClick)
//	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ParaList::OnLvnEndlabeledit)
//	ON_NOTIFY_REFLECT(NM_DBLCLK, &ParaList::OnNMDblclk)
//END_MESSAGE_MAP()

	BEGIN_MESSAGE_MAP_HPP(ParaList, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ParaList::OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT(NM_CLICK, &ParaList::OnNMClick)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ParaList::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ParaList::OnNMDblclk)
	END_MESSAGE_MAP_HPP()
};
