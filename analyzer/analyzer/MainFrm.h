
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutputWnd.h"
#include "pcct.h"
#include "pcctA.h"
//#include "typedefine.h"
#include "MFCCaptionBarA.h"
#include "ANPara.h"
#include "SAPara.h"
#include "VPara.h"
#include "PlotData.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

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
	int timer1;
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	std::vector<pcct> dat;
	bool finishflag;
	bool finishflag2;
	double Ar0;
	pcctA datt;
	int stepCount;
	int rowCount;
	double totalVolume;
	afx_msg void OnViewToolbara();
	afx_msg void OnUpdateViewToolbara(CCmdUI *pCmdUI);
	void plot1(const std::vector<double> & x, const std::vector<double> & y, const CString & xlabel, const CString & ylabel);
	void plot2(const std::vector<double> & x, const std::vector<double> & y, const CString & xlabel, const CString & ylabel);
	int timer2;
	afx_msg void OnAnalysisMethodsetup();

	//anp p1;
	//cvp p2;
	//sap p3;
	ANPara p1;
	CVPara p2;
	SAPara p3;

	afx_msg void OnFileSave();
	CString AnalysisSetupINI;
	
	afx_msg void OnAnalysisStartanalysis();
	CString Output1(PlotData & pdat);
	CString Output2(PlotData & pdat);
	CString Output3(PlotData & pdat);
};


