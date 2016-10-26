
// analyzer.h : main header file for the analyzer application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


//#define QTT

#ifdef QTT
#include "lib/AnalyzerRunner.h"
#endif



// CanalyzerApp:
// See analyzer.cpp for the implementation of this class
//

class CanalyzerApp : public CWinAppEx
{
public:
	CanalyzerApp();

#ifdef QTT

	
	AnalyzerRecorder ar;


#endif

	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CanalyzerApp theApp;
