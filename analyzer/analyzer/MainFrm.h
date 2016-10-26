
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutputWnd.h"
#include "user\AccountList.hpp"
#include "WaitDlg.h"
#include "property\PropertySheetA1ML.h"

enum ProcessState{
	stop,
	running,
	pause
};

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
	int userIndex;
	AccountList al;
	BOOL m_bSplitterCreated;
	PropertySheetA1ML *psheetml;
	WaitDlg *wd;
	CWinThread *pWriteA;
	LANGID LangID;
// Operations
public:
	CWnd * LeftPane(void) const { return m_wndSplitter.GetPane(0,0);};
	CWnd * RightPane(void) const { return m_wndSplitter.GetPane(0,1);};
	void ShowWaitDlg(CString tips);
	void HideWaitDlg(void);
	COutputWnd * GetOutputWnd(void){ return &m_wndOutput; };
	void ChangeLang(void);
	UserAccount::authority GetCurAuth(void){ return al.ual[userIndex].au; };
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
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

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSecurityLogin();
	afx_msg void OnSecurityUseraccounts();

	afx_msg void OnAnalysisMethodsetup();	
	afx_msg void OnAnalysisAbortanalysis();
	afx_msg void OnAnalysisPause();
	afx_msg void OnAnalysisStartanalysis();

	afx_msg LRESULT OnMessageUpdateDol(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageCloseSapSheet(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageChangeLang(WPARAM wParam, LPARAM lParam);

	afx_msg void OnViewAnalysisProgress();
	afx_msg void OnViewToolbarA();
	afx_msg void OnUpdateViewAnalysisProgress(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewToolbarA(CCmdUI *pCmdUI);


	afx_msg void OnUpdateAnalysisMethodsetup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisStartanalysis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisPause(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisAbortanalysis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisReport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisCompute(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisExportdata(CCmdUI *pCmdUI);


	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrintSetup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);

	afx_msg void OnUpdateViewFitwindow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewDatacursor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsPlotsettings(CCmdUI *pCmdUI);

	afx_msg void OnUpdateSecurityLogin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSecurityUseraccounts(CCmdUI *pCmdUI);

	LANGID nID2LangID(UINT nID);
	afx_msg void OnLanguage(UINT id);
	afx_msg void OnUpdateLanguage(CCmdUI *pCmdUI);

public:
	HANDLE m_hStop;
	
};


