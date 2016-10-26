#pragma once
#include "afxwin.h"
#include <vector>
#include "../struct\Value.hpp"
#include "../list/EditC.hpp"

// ParaList

class ParaList : public CListCtrl
{
	//DECLARE_DYNAMIC(ParaList)

public:
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

	static void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap=42)
	{
		int widthc,widtht;
		widtht=ls->GetStringWidth(str)+gap;
		widthc=ls->GetColumnWidth(nCol);
		if(widthc<widtht)
			ls->SetColumnWidth(nCol,widtht);
	};

protected:
	CImageList imList;

	int m_iSubItem;
	EditC<double> m_edtItemEdit;
public:
	std::vector<Value> vl;
	double x;
	int rowH;
	int firstColW;
public:
	ParaList()
		: rowH(18)
		, firstColW(39)
	{
	};
	virtual ~ParaList(){};


	void Refresh(void)
	{
		this->DeleteAllItems();
		imList.DeleteImageList();

		//�������ͼ���б�
		//1����������ӳ�Ա����	CImageList m_imList;
		//2��Ϊͼ���б����ͼƬ

		//BitmapͼƬ(Ӧ��Ϊ48X48��С)
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

		////ICONģʽ������һ�㣩
		//HICON hIcon;

		//m_imList.Create(32,32,ILC_COLOR,2,20);

		//hIcon = AfxGetApp()->LoadIconW(IDI_ICON_GREE);
		//m_imList.Add(hIcon);

		//hIcon = AfxGetApp()->LoadIconW(IDI_ICON_READ);
		//m_imList.Add(hIcon);

		//��������ͼ���б���CListCtrl�ؼ�������Ҫʹ��LVSIL_SMALL���
		SetImageList(&imList,LVSIL_SMALL ); //����Ҫʹ�ã�LVSIL_SMALL���


		//�塢�������

		int nRow;  //��¼�к�
		LVITEM lvItem={0};

		for(size_t i=0;i<vl.size();i++){
			//��һ������
			lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;	//���֡�ͼƬ��״̬
			lvItem.iItem = i;		//�к�(��һ��)
			lvItem.iImage = i;	//ͼƬ������(��һ��ͼƬ)
			lvItem.iSubItem = 0;	//���к�

			nRow = InsertItem(&lvItem);		//��һ��ΪͼƬ
			CString str;
			str.Format(L"%g",vl[i].raw);
			SetItemText(nRow,1,str);  //�ڶ���Ϊ����
			AdjustWidth(this,1,str);
			str.Format(L"%g",vl[i].correction);
			SetItemText(nRow,2,str);  //�ڶ���Ϊ����
			AdjustWidth(this,2,str);
			str.Format(L"%g",vl[i].Output());
			SetItemText(nRow,3,str);  //�ڶ���Ϊ����
			AdjustWidth(this,3,str);
		}

	};

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CListCtrl::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  Add your specialized creation code here

		//CListCtrl�ؼ���Report����б������ͼƬ

		ModifyStyle(0,LVS_EDITLABELS|
			LVS_REPORT
			//|LVS_SHOWSELALWAYS
			);

		//һ������CListCtrl�ؼ���չ���(�����ߡ�����ѡ�С�ͼƬ)
		DWORD dwStyle;

		dwStyle = GetExtendedStyle();
		dwStyle = dwStyle
			|LVS_EX_FULLROWSELECT
			|LVS_EX_GRIDLINES
			|LVS_EX_SUBITEMIMAGES ;
		SetExtendedStyle(dwStyle);


		//�ġ���CListCtrl�ؼ���Ӷ���
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
		//�塢�������

		Refresh();


		return 0;
	};

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

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(ParaList, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &ParaList::OnLvnBeginlabeledit)
	ON_NOTIFY_REFLECT(NM_CLICK, &ParaList::OnNMClick)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &ParaList::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &ParaList::OnNMDblclk)
END_MESSAGE_MAP()
