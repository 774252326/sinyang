
// analyzer.h : main header file for the analyzer application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


#include "recorder.h"
#include "runnerBase.h"
#include "standard_definitions_head.h"//
//#include <iostream>
////using std::cout;
////using std::endl;
//#include <QApplication>
#include <QMessageBox>
//#include <QTimer>
#include "exports.h" // for Libec namespace
#include "ECDEF.H"

#include "struct/VPara.hpp"

#include <vector>

// CanalyzerApp:
// See analyzer.cpp for the implementation of this class
//

class CanalyzerApp : public CWinAppEx
{
public:
	CanalyzerApp();

	RunnerBase* runner;
	LabviewRecorder* arrayRecorder;
	BOOL m_bRunning;
	int m_nCnt;

	static const int NN=65535;
	float* fx;
	float* fy;


	void dosomething(std::vector<double> &x, std::vector<double> &y)
	{

		int ncnt2=m_nCnt;
		arrayRecorder->recordAllSeries();
		m_nCnt = arrayRecorder->numRecorded();
		if(m_nCnt>=ncnt2){
			x.resize(m_nCnt-ncnt2,0);
			y.resize(m_nCnt-ncnt2,0);
		}
		else{
			x.resize(m_nCnt,0);
			y.resize(m_nCnt,0);
		}
		for(int i=ncnt2;i<m_nCnt;i++)
		{
			x[i-ncnt2]=fx[i];
			y[i-ncnt2]=fy[i];
		}
	};

	void setsomething(const CVPara &p2, int seg)
	{
		m_nCnt=0;

		//int const NN=65535;
		//float fx[NN],fy[NN];
		Q_ASSERT(NN >= runner->singleChannelStorageRequired());
		// change technique. must do this before changing any Q_PROPERTYs...
		bool bResult1 = QMetaObject::invokeMethod(
			runner, 
			"set_iTech", // invoke (protected!) slot
			Qt::DirectConnection,
			Q_ARG(int, M_IT) // ECDEF.H
			);
		Q_ASSERT(bResult1);
		// during a "dry run", run() does nothing - so instrument need not be attached.
		// don't forget to omit the fatal-error-checking on Libec::openCom()!
		////theApp.runner->setProperty("m_bDryRun", true); 
		//theApp.runner->setProperty("m_ei", 1.57);//initial E
		//theApp.runner->setProperty("m_eh", 1.57);//high E
		//theApp.runner->setProperty("m_el", -0.23);	//low E
		//theApp.runner->setProperty("m_ef", 0.8);//final E
		//theApp.runner->setProperty("m_pn", 1);//intial scan polarity
		//theApp.runner->setProperty("m_vv",0.1);//scan rate
		//theApp.runner->setProperty("m_inpcl", 3);//sweep segment
		//theApp.runner->setProperty("m_inpsi",0.001);//sample interval
		//theApp.runner->setProperty("m_qt", 1);//quiet time
		//theApp.runner->setProperty("m_iSens",2);

		int polar=(seg==3?1:0);
		double ie=(seg==3?0.8:p2.highelimit);
		runner->setProperty("m_ei", ie);//initial E
		runner->setProperty("m_eh", p2.highelimit);//high E
		runner->setProperty("m_el", p2.lowelimit);	//low E
		runner->setProperty("m_ef", 0.8);//final E
		runner->setProperty("m_pn", polar);//intial scan polarity
		runner->setProperty("m_vv",p2.scanrate);//scan rate
		runner->setProperty("m_inpcl", seg);//sweep segment
		runner->setProperty("m_inpsi",p2.EInterval());//sample interval
		runner->setProperty("m_qt", 1);//quiet time
		runner->setProperty("m_iSens",2);

		delete []fx;
		delete []fy;
				
		fx=new float[NN];
		fy=new float[NN];

		arrayRecorder->reset();
		arrayRecorder->setStorageTarget(fx,fy,NN); // a quirk of my api	

		//bool haveExperiment = runner->getExperimentFromUser();  //removing this line code will not show the setting dialog 
		//if(!haveExperiment) return;

		runner->runInWorkerThread();
	};



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
