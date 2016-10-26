#if ((defined _MSC_VER) && (_MSC_VER > 1000)) || ((defined __GNUC__) && (__GNUC__ > 3))
#	pragma once
#else
#	error "Custom header guard needed - have fun editing all N files in this project...just HOW portable is pragma once??"
#endif

// NOTE: this file should only be included in .h files

// this file centralizes shared definitions required by headers (e.g., typedefs)


// typedefs
typedef double FData;		// actually float is DISALLOWED by qwtplot, which requires double data??


// "templated typedefs"... http://marcmutz.wordpress.com/2010/07/29/sneak-preview-qlist-considered-harmful/

// "QList considered harmful" despite Nokia documentation... 
// omfg how horrible - i am NOT proud of this idea...// template<class T> typedef QList<T> Array<T>; direct approach fails it in C++ pre-0x// template <class T> class Array<T> : public QList <T> {public: private:}; my idea alsofails it
//#define T_ARRAY QVector // candidates: {QVector, QList} - QVarLengthArray doesn't like SaveTreeNode (needs = operator?)
// - compile error in qt creator?! better to catch this shit now...


#define ARRAY_TYPE_LIST 1
#define ARRAY_TYPE_VECTOR 2
#define ARRAY_TYPE ARRAY_TYPE_VECTOR


#ifdef QT_NO_DEBUG
#	define QT_NO_DEBUG_OUTPUT					
#	define QT_NO_WARNING_OUTPUT			
#else
//#	define QT_FATAL_WARNINGS // no, this is an ENVIRONMENT VARIABLE??
#endif


// "temporary" code for qwt's autoscale + inverse + log axes bug
// still not fixed as of Qwt 6.0.1 2011-10-17
// "development branch" Qwt 6.1.0 svn1196 has tons of breaking changes
// FIXED in stable branch Qwt 6.0.2 svn1225 + Qt 4.7.3!! works for me!!! Qt 4.8 not required!
//#define CHI_PLOT_H_PATCH_AUTO_INV_LOG





// 2011-08-02 reasons not to turn on /Wall 
// - not supported in VC8 which is my main development compiler anyway
// - can't #pragma warning disable qrc, other qt-generated files?
//#pragma warning ( disable : 4365 4571 4619 4625 4626 4711 4820 )
// these have been moved to the .pro file? (would have to regenerate sln file if changed...)
	// fuck that i think? reloading .pro file is annoying


// muzzle warnings for all includes, esp. library; to re-enable for OUR code, "override" in standard_definitions_body.h
#ifdef _MSC_VER
#	pragma warning ( disable : 4127 ) // conditional expression is constant
#	pragma warning ( disable : 4189 ) // local variable is initialized but not referenced
#	pragma warning ( disable : 4265 ) // class has virtual functions, but destructor is not virtual (CTest12A)
#	pragma warning ( disable : 4365 ) // signed/unsigned mismatch
#	pragma warning ( disable : 4505 ) // unreferenced local function has been removed (for qwt) wait BODY only?!
#	pragma warning ( disable : 4512 ) // assignment operator could not be generated
#	pragma warning ( disable : 4548 ) // expression before comma has no side effect
#	pragma warning ( disable : 4571 ) // deprecated catch semantics
#	pragma warning ( disable : 4619 ) // no warning number 'number'
#	pragma warning ( disable : 4625 ) // no base class copy constructor
#	pragma warning ( disable : 4626 ) // no base class assignment operator
#	pragma warning ( disable : 4711 ) // function selected for inline expansion
#	pragma warning ( disable : 4820 ) // padding added after construct
#	pragma warning ( disable : 4826 ) // conversion is sign-extended
#	pragma warning ( disable : 4996 ) // secure string
//dbg
#	pragma warning ( disable : 4628 ) 
#	pragma warning ( disable : 4189 ) 

#	if _MSC_VER >= 1600
#		pragma warning ( disable : 4189 ) // local variable is initialized but not referenced
#		pragma warning ( disable : 4986 ) // operator new[], delete[]: exception specification does not match previous declaration
#	endif

#	ifdef QT_NO_DEBUG // release only?!

//#		pragma warning ( disable : 4946 ) // huh? not needed anymore? reinterpret_cast used (via qobject_cast)

#	endif


// extra stuff required to build on command line without complaints? did my reinstallation of platform sdk break something?
#	pragma warning ( disable : 4061 ) // enumerator in switch is not explicitly handled by a case label (mainly qwt)
//#	pragma warning ( disable : 4619 ) // warning: there is no warning number xxxx - ugh has to go in .pro file since it's for qrc...
#	pragma warning ( disable : 4668 ) // win32_winnt not defined - no this has to go in the qmake file
#	pragma warning ( disable : 4946 ) // reinterpret_cast used (via qobject_cast)
#	ifndef QT_NO_DEBUG
#		pragma warning ( disable : 4710 ) // function not inlined - ugh has to go in .pro file?
#		pragma warning ( disable : 4738 ) // storing 32-bit float result in memory, possible loss of performance
#	endif


#endif // defined _MSC_VER


// dll shit...
#include <QtCore/QtGlobal> // for Q_DECL_EXPORT and Q_DECL_IMPORT
#if defined(EC_BUILD_SHARED)
#	define EC_EXPORT Q_DECL_EXPORT
#elif defined(EC_USE_SHARED)
#	define EC_EXPORT Q_DECL_IMPORT
#else
#	define EC_EXPORT // does nothing
#endif


// comm simulator - if you want to use it outside of integration testing...
#if !defined (QT_NO_DEBUG) && !defined(LIBEC_COMM_SIMULATOR)
#   define LIBEC_COMM_SIMULATOR
#endif

#if defined(LIBEC_COMM_SIMULATOR) && defined(QT_NO_DEBUG)
#   error simulator + release build!? no, no, no
#endif




