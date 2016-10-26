#pragma once
#include "standard_definitions_head.h" // for EC_EXPORT

#include <QHash>
#include "report.h" // to allow function with Report::Series argument
#include "runnerBase.h" // ugh, nested class can't use forward declaration, despite usage of pointer only
//class RunnerBase::SpectatorBase::Report;

#define RECORDER_H__MAX_LABVIEW_CHANNELS 8



class EC_EXPORT BaseRecorder // exported so that PlotOptions can subclass from it. Good luck to sdk users trying to subclass from an undocumented class...
{
public:
	BaseRecorder();
	BaseRecorder(RunnerBase::SpectatorBase*, unsigned int delayInMs);
	int numRecorded() const;
	virtual void recordAllSeries(); // polymorphic output via recordSeries_subclass()
	void reset();
	void setSpectator(RunnerBase::SpectatorBase*);

protected:
	unsigned int delayInMs() const;
	RunnerBase::SpectatorBase* spectator() const;
	bool hasSpectator() const; // if recorder is going to "live" in its medium, then it needs to set/query spectator AFTER instantiation
	int numRecordedY(float const volatile* y) const;
	virtual void recordSeries(const QList<Report::Series>&, int start, int n) = 0;
	virtual void reset_subclass() = 0;
	
private:
	void initialize();
	
	void recordAllSeries_naive();
	void recordSeriesBase(const Report::Series&, int start, int recordCount);
	
	void recordAllSeries_commonX();
	void recordSeriesBase(const QList<Report::Series>&, int start, int recordCount);

	RunnerBase::SpectatorBase* m_spectator; // not instantiated here; no need to delete
	unsigned int m_delayInMs;
//	int m_numSeries;
	QHash<float const volatile*, int> m_recordedXLengths; // cached state variables for "smart" recording
	QHash<float const volatile*, int> m_recordedYLengths;

	// i want to store a state variable that keeps track of how many data points have already been recorded
};



// basically a crappy little class to test the rest of this pipeline
class EC_EXPORT ConsoleRecorder : public BaseRecorder
{
public:
	ConsoleRecorder(RunnerBase::SpectatorBase*, unsigned int delayInMs);
protected:
	virtual void recordSeries(const QList<Report::Series>&, int start, int n);
	virtual void reset_subclass();
};



class EC_EXPORT LabviewRecorder : public BaseRecorder
{
public:
	LabviewRecorder(RunnerBase::SpectatorBase*);
	bool hasStorageTarget() const;
	void setStorageTarget(float *x, float *y, int n);
	void setStorageTarget(float *x, float *y, float *y2, int n);
	void setStorageTarget(
		float* xTargets[RECORDER_H__MAX_LABVIEW_CHANNELS], 
		float* yTargets[RECORDER_H__MAX_LABVIEW_CHANNELS], 
		int xySizes[RECORDER_H__MAX_LABVIEW_CHANNELS], 
		int xyRecorded[RECORDER_H__MAX_LABVIEW_CHANNELS]);

	#ifndef QT_NO_DEBUG
	int integrationTest_numRecorded() const; // meh, kept for legacy compatibility
	//private:
	//	int m_integrationTest_n; // deprecated by BaseRecorder::numRecorded()
	//public:
	#endif

protected:
	virtual void recordSeries(const QList<Report::Series>&, int start, int n);
	virtual void reset_subclass();
private:
	bool channelHasStorageTarget(int) const;
	void errorMessage(const QString&) const;
	
	enum { MAX_CHANNELS = RECORDER_H__MAX_LABVIEW_CHANNELS };

	QList<float const volatile*> m_rawYpointers; // JUST for keeping track of which channel to write to

	float *m_x[RECORDER_H__MAX_LABVIEW_CHANNELS];
	float *m_y[RECORDER_H__MAX_LABVIEW_CHANNELS];
	int m_xySizes[RECORDER_H__MAX_LABVIEW_CHANNELS]; // array size for internal bookkeeping
	int *m_xyNumRecorded; // OUTPUT array for reporting # recorded to user
	
};