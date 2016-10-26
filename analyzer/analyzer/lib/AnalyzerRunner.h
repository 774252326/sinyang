// analyzerrunner.h : header file for the analyzer runner
//
#pragma once




#include "recorder.h"
#include "exports.h" // for Libec namespace
#include "ECDEF.H"

#include "../struct/VPara.hpp"
#include <vector>


#pragma comment( lib, "libec.lib" )
#pragma comment( lib, "QtCore4.lib" )
#pragma comment( lib, "QtSvg4.lib" )
#pragma comment( lib, "QtGui4.lib" )
#pragma comment( lib, "QtXml4.lib" )



class AnalyzerRunner 
{
public:
	RunnerBase * runner;
	LabviewRecorder * arrayRecorder;
	bool m_bRunning;
	int m_nCnt;

	static const int NN=65535;
	float* fx;
	float* fy;

public:

	void Release()
	{			
		//delete arrayRecorder;
		delete []fx;
		delete []fy;
		delete runner;
	};

	void Initialize()
	{
		/////open com for the electrochemical analyzer
		Libec::initializeSystemSettings(); 
		runner = Libec::instantiateRunner();
		if (!Libec::openCom(runner))
		{
			qWarning() << __FUNCTION__ << "could not open com port";
			//	return FALSE;
		}

		arrayRecorder = new LabviewRecorder(runner->spectator());


	};

	AnalyzerRunner()		
	{
		//Initialize();
	};
	~AnalyzerRunner()
	{
		//Release();
	};

	void RecordData(std::vector<double> &x, std::vector<double> &y)
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
			y[i-ncnt2]=-fy[i]*1000;
		}
	};

	void SetAndStart(const CVPara &p2, int seg)
	{
		m_nCnt=0;

		//int const NN=65535;
		//float fx[NN],fy[NN];
		Q_ASSERT(NN >= runner->singleChannelStorageRequired());
		// change technique. must do runner before changing any Q_PROPERTYs...
		bool bResult1 = QMetaObject::invokeMethod(
			runner, 
			"set_iTech", // invoke (protected!) slot
			Qt::DirectConnection,
			Q_ARG(int, M_IT) // ECDEF.H
			);
		Q_ASSERT(bResult1);
		// during a "dry run", run() does nothing - so instrument need not be attached.
		// don't forget to omit the fatal-error-checking on Libec::openCom()!

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
		runner->setProperty("m_qt", p2.quiettime);//quiet time
		runner->setProperty("m_iSens",2);

		delete []fx;
		delete []fy;

		fx=new float[NN];
		fy=new float[NN];

		arrayRecorder->reset();
		arrayRecorder->setStorageTarget(fx,fy,NN); // a quick of my api	

		//bool haveExperiment = runner->getExperimentFromUser();  //removing runner line code will not show the setting dialog 
		//if(!haveExperiment) return;

		runner->runInWorkerThread();


	};

};


class AnalyzerRecorder : public LabviewRecorder 
{
public:
	RunnerBase * runner;

	int m_nCnt;

	static const int NN=65535;
	float* fx;
	float* fy;

public:

	void Release()
	{			
		delete []fx;
		delete []fy;
		delete runner;
	};


	AnalyzerRecorder(RunnerBase * r=Libec::instantiateRunner())
		: LabviewRecorder(r->spectator())
	{
		Libec::initializeSystemSettings(); 

		runner=r;
		if (!Libec::openCom(runner))
		{
			qWarning() << __FUNCTION__ << "could not open com port";
		}

	};

	~AnalyzerRecorder()
	{
		Release();
	};

	void RecordData(std::vector<double> &x, std::vector<double> &y)
	{
		int ncnt2=m_nCnt;
		recordAllSeries();
		m_nCnt = numRecorded();
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
			y[i-ncnt2]=-1000*fy[i];
		}
	};

	void SetAndStart(const CVPara &p2, int seg)
	{


		Q_ASSERT(NN >= runner->singleChannelStorageRequired());
		// change technique. must do runner before changing any Q_PROPERTYs...
		bool bResult1 = QMetaObject::invokeMethod(
			runner, 
			"set_iTech", // invoke (protected!) slot
			Qt::DirectConnection,
			Q_ARG(int, M_IT) // ECDEF.H
			);
		Q_ASSERT(bResult1);
		// during a "dry run", run() does nothing - so instrument need not be attached.
		// don't forget to omit the fatal-error-checking on Libec::openCom()!

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
		runner->setProperty("m_qt", p2.quiettime);//quiet time
		runner->setProperty("m_iSens",2);


        m_nCnt=0;

		delete []fx;
		delete []fy;

		fx=new float[NN];
		fy=new float[NN];

		reset();
		setStorageTarget(fx,fy,NN); // a quick of my api	

		//bool haveExperiment = runner->getExperimentFromUser();  //removing runner line code will not show the setting dialog 
		//if(!haveExperiment) return;

		runner->runInWorkerThread();
	};


};






