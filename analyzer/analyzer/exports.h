#ifndef EXPORTS_H
#define EXPORTS_H

#include "standard_definitions_head.h" // for EC_EXPORT

#if 0//1
#include "stdafx2.h" // for BOOL
#else
#include "stdafx.h" // for BOOL
#endif//0

#include <QList> // for noComPortsFound()

// functions to be exposed outside the dll (e.g., to labview)
// - actually, labview-visible functions have been moved to labview.cpp.
// - these are exposed only in debug mode.
#ifdef MY_EXPORT
#	error uh oh name collision // won't preprocessor catch redefinition anyway??
#endif

#ifndef QT_NO_DEBUG
#	define MY_EXPORT //EC_EXPORT
#else
#	define MY_EXPORT // hide most of these, which wound up being testing-only...
#endif


// functions used in my testing VI's - CV.vi and quick-dialog.vi
#define MY_LV_EXPORT MY_EXPORT //EC_EXPORT meh, don't expose those to end users for now



// --------------------------------------------------------------------------------
// "C" functions that i want (or once wanted) to be accessible from labview
// --------------------------------------------------------------------------------

// system initialization tasks (esp. for labview!!)
extern "C" MY_LV_EXPORT void export_initializeSystemSettings();
extern "C" MY_EXPORT void export_finalizeSystemSettings();

// CruD for serial port
extern "C" MY_LV_EXPORT BOOL export_openCom();
extern "C" MY_LV_EXPORT void export_closeCom();

// most rudimentary run: specify # data points (via array size)
extern "C" MY_EXPORT void export_run(float outputArray[], int outputLength);

// adjust i-t potential
extern "C" MY_EXPORT void export_run2(float outputArray[], int outputLength, float potential);

// adjust i-t potential + unmodified class CRun12A
extern "C" MY_EXPORT void export_run3(float outputArray[], int outputLength, float potential);

// an experiment in "real-time" data acquisition
extern "C" MY_LV_EXPORT int export_run3_numDataPts();
extern "C" MY_EXPORT int export_run3_getData(float*, int);

// toward the full instrument line!
extern "C" MY_LV_EXPORT void export_run_CV(float output_x[], float output_y[], int, float vmax);
extern "C" MY_LV_EXPORT int export_run3_getDataXY(float*, float*, int);

// About CHI
extern "C" MY_LV_EXPORT void export_about();



// also maybe some shit in testing.h...
//extern "C" MY_EXPORT int export_testDefaultArg(int a, int b);
extern "C" MY_EXPORT int export_sum(int N, int a, ...);
extern "C" MY_LV_EXPORT int export_messageBox(); // hmm, don't want to break quick-dialog.vi...

// unfortunately, extern "C" and Qt's moc seem to be INCOMPATIBLE...
//extern "C" MY_EXPORT void test();

// proof of principle: can move code from Exe to Dll ANYTIME
extern "C" MY_EXPORT void export_runFromDll();





// --------------------------------------------------------------------------------
// C++ functions for the "real" SDK
// --------------------------------------------------------------------------------

// forward declarations
class RunnerBase;

#if !defined QT_NO_DEBUG && defined LIBEC_COMM_SIMULATOR
#	define EXPORTS_H_EXPOSE_SIMULATOR_DELAY
#endif



// "pure SDK" functions that shouldn't be visible from labview(windows)
	// - confirmed as "invisible" in labview(windows)
	// - confirmed as "visible" in labview(linux). unavoidable? 
	// PROBABLY visible in labview(mac), god rest their souls
namespace Libec
{
	// expose these to exe cuz they're just too damn useful
	//EC_EXPORT void hello();	
	EC_EXPORT void initializeSystemSettings();
	EC_EXPORT bool openCom(RunnerBase*);
	EC_EXPORT void closeCom();
	EC_EXPORT RunnerBase* instantiateRunner();
	EC_EXPORT int dataLength();
	
	enum DataLength { DataLen256K, DataLen512K, DataLen1024K, DataLen2048K, DataLen4096K, DataLen8192K, DataLen16384K };
	EC_EXPORT void setDataLength(DataLength); // only takes effect if called before instantiateRunner()	

	#ifndef QT_NO_DEBUG
	EC_EXPORT
	#endif
	bool openComFromAutoScan_silent(RunnerBase*);

	#ifdef EXPORTS_H_EXPOSE_SIMULATOR_DELAY
	EC_EXPORT void setSimulatorDelayInMs(unsigned int);
	#endif

	// used by labview.cpp and CANNOT be limited to file scope. 
	// but don't have to expose this to exe, not yet anyway...
	bool checkCom(RunnerBase*);
}

#endif // defined EXPORTS_H

