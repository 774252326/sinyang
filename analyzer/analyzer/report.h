#pragma once

#include "standard_definitions_head.h" 

#include <QList>
#include <QHash>
//#include <QPair>

// ok, there's NO NEED to nest this here.
// Report is accessed externally by Recorder, so it actually doesn't really make SENSE to nest this deep!
// yep, doubly nested class was kind of a giveaway smell
//class RunnerBase::SpectatorBase::Report
class Report
{
public:
	Report();
	~Report();
	void clear();
	void addSeries(const QString& name, const QString& xName, float const volatile* x, const QString& yName, float const volatile* y, int const volatile* n);

	// called by Spectator
	void update(); 

	// read-only interface for Recorder
	struct Series // convention: structs should be pure DATA. constructor added as an exception for default values
	{
		Series(const QString& name_, const QString& xName, float const volatile* x_, const QString& yName, float const volatile* y_, int n_);
		QString name, xTitle, yTitle;
		float const volatile* x;
		float const volatile* y;
		int n;	
	};	
	QList<Series> allSeries() const;
	int numSeries() const;
	
private:	
	void addSeriesLengthAddress(float const volatile*, int const volatile*);
	bool hasSeriesLengthAddress(float const volatile*) const;
	int seriesLength(int const volatile*) const;
	
	bool isNull(float const volatile*, const QString&) const;
	bool isNull(int const volatile*, const QString&) const;
	bool isNull(void const volatile*, const QString&) const;
	int const volatile* vectorSizeAddress(float const volatile*) const;

	QHash<float const volatile*, int const volatile*> m_yDataLengthAddresses; // [ y-address => n-address ]; hash use inspired by Ruby
	QList<Series> m_seriesList;

	enum { INITIAL_LENGTH = 0 };//static int const INITIAL_LENGTH = 0;// hmmm uglier and uglier still
};


