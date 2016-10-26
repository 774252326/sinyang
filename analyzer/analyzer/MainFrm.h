
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutputWnd.h"
//#include "MFCCaptionBarA.h"
//#include "MFCToolBarA.h"
//#include "analyzerViewR.h"
//#include "analyzerViewL.h"
//#include "func.h"

//#include "UserAccount.h"
//#include "AccountList.h"
#include "struct\AccountList.hpp"

#include "filefunc.h"

#include "WaitDlg.h"


class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	//CSplitterWnd m_wndSplitter;
public:
	CSplitterWnd m_wndSplitter;
// Operations
public:

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
	//CMFCCaptionBarA    m_wndCaptionBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	//afx_msg void OnViewCaptionBar();
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
//	BOOL CreateCaptionBar();

	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI *pCmdUI);

	afx_msg void OnViewAnalysisProgress();
	afx_msg void OnUpdateViewAnalysisProgress(CCmdUI *pCmdUI);
	BOOL m_bSplitterCreated;

	afx_msg void OnAnalysisStartanalysis();
	CWinThread *pWriteA;
	afx_msg void OnAnalysisAbortanalysis();
	afx_msg void OnAnalysisPause();
	afx_msg void OnUpdateAnalysisMethodsetup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisStartanalysis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisPause(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisAbortanalysis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisReport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsPlotsettings(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewFitwindow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewDatacursor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisCompute(CCmdUI *pCmdUI);
	afx_msg void OnAnalysisCompute();
	afx_msg void OnSecurityLogin();
	afx_msg void OnSecurityUseraccounts();
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilePrintSetup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);

	afx_msg void OnHelpHelptopics();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:

	COutputWnd* GetOutputWnd(){return &m_wndOutput;};
	//CMFCCaptionBarA* GetCaptionBar(){ return &m_wndCaptionBar;};


	//bool bWaiting;
	ProcessState pst;
	//authority au;
	AccountList al;
	int userIndex;

	//afx_msg LRESULT OnMessageUpdateDol(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	WaitDlg *wd;
protected:
	afx_msg LRESULT OnMessageWaitResponse(WPARAM wParam, LPARAM lParam);
};


