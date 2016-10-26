
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutputWnd.h"
//#include "pcct.h"
//#include "pcctA.h"
//#include "pcctB.h"
//#include "typedefine.h"
#include "MFCCaptionBarA.h"
//#include "ANPara.h"
//#include "SAPara.h"
//#include "VPara.h"
//#include "PlotData.h"
#include "dlg1.h"
//#include "afxmt.h"

//#define MESSAGE_BUSY WM_USER+1
//#define MESSAGE_WAIT_RESPONSE WM_USER+2
//#define MESSAGE_OVER WM_USER+3

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:
	CSplitterWnd* GetSplitter(){return &m_wndSplitter;};
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
	//CMFCCaptionBar    m_wndCaptionBar;
	CMFCCaptionBarA    m_wndCaptionBar;
public:
	CMFCCaptionBarA* GetCaptionBar(){ return &m_wndCaptionBar;};



// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	BOOL CreateCaptionBar();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL m_bSplitterCreated;
	afx_msg void OnFileOpen();

	COutputWnd* GetOutputWnd(){return &m_wndOutput;}
	afx_msg void OnViewFitwindow();
	afx_msg void OnViewAnalysisProgress();
	afx_msg void OnUpdateViewAnalysisProgress(CCmdUI *pCmdUI);
//	int timer1;
//	afx_msg void OnTimer(UINT_PTR nIDEvent);


//	std::vector<pcct> dat;
//	bool finishflag;
//	bool finishflag2;
//	double Ar0;
//	pcctA datt;
//	int stepCount;
//	int rowCount;
//	double totalVolume;
	afx_msg void OnViewToolbara();
	afx_msg void OnUpdateViewToolbara(CCmdUI *pCmdUI);
//	void plot1(const std::vector<double> & x, const std::vector<double> & y, const CString & xlabel, const CString & ylabel);
//	void plot2(const std::vector<double> & x, const std::vector<double> & y, const CString & xlabel, const CString & ylabel, const CString & lineName);
//	int timer2;
	afx_msg void OnAnalysisMethodsetup();

	//anp p1;
	//cvp p2;
	//sap p3;
	//ANPara p1;
	//CVPara p2;
	//SAPara p3;

	afx_msg void OnFileSave();
//	CString AnalysisSetupINI;
	
	afx_msg void OnAnalysisStartanalysis();
//	CString Output1(PlotData & pdat);
//	CString Output2(PlotData & pdat);
//	CString Output3(PlotData & pdat);
//	double Aml;
//	CString Output4(PlotData & pdat);
//	double Qintercept;
	dlg1 * LeftPlotPointer(void);
	dlg1 * RightPlotPointer(void);
	bool waiting;
	afx_msg void OnAnalysisAbortanalysis();
protected:
//	afx_msg LRESULT OnMessagebusy(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnMessageWaitResponse(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnCaptionbarMessage(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnMessageOver(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnOptionsPlotsettings();
	afx_msg void OnUpdateOptionsPlotsettings(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalysisStartanalysis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewFitwindow(CCmdUI *pCmdUI);
private:
	HANDLE hThread;
public:
	afx_msg void OnUpdateAnalysisAbortanalysis(CCmdUI *pCmdUI);
	afx_msg void OnAnalysisPause();
	afx_msg void OnUpdateAnalysisPause(CCmdUI *pCmdUI);
//	int processState;
	afx_msg void OnUpdateAnalysisMethodsetup(CCmdUI *pCmdUI);
	ProcessState pst;
};


