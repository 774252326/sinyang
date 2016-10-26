
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutputWnd.h"
#include "user\AccountList.hpp"
#include "WaitDlg.h"
#include "property\PropertySheetA1ML.h"

class CMainFrame : public CFrameWndEx
{
	public:
	static UINT PROCESS(LPVOID pParam);

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:
	ProcessState pst;
// Operations
public:
	CWnd * LeftPane(void) const { return m_wndSplitter.GetPane(0,0);};
	CWnd * RightPane(void) const { return m_wndSplitter.GetPane(0,1);};
	
// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	COutputWnd        m_wndOutput;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	int userIndex;
	AccountList al;
	BOOL m_bSplitterCreated;
	PropertySheetA1ML *psheetml;
	WaitDlg *wd;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSecurityLogin();
	afx_msg void OnSecurityUseraccounts();
	afx_msg void OnAnalysisMethodsetup();
	COutputWnd * GetOutputWnd(void){ return &m_wndOutput; };
	afx_msg LRESULT OnMessageUpdateDol(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageCloseSapSheet(WPARAM wParam, LPARAM lParam);
	
};


