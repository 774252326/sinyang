// to-do
// - RunnerTechniqueBase cosmetics? depends on how closely I want to "emulate" EC

// base classes for Runner12A, Runner12A_Options, and their many, inbred siblings

#pragma once
#ifndef RUNNERBASE_H
#define RUNNERBASE_H

#include "standard_definitions_head.h"


//#ifdef _MSC_VER
//#	pragma warning ( disable : 4127 )
//#endif
#include <QFuture> // for worker thread antics
#include <QMutex> // to make SpectatorBase::Look() thread-safe...
#include <QObject>
//#ifdef _MSC_VER
//#	pragma warning ( default : 4127 )
//#endif

#if 0//1
#include <saveTreeNode.h> // for Save() and Load()
#else
#include "saveTreeNode.h" // for Save() and Load()
#endif//0


// forward declarations
class OptionsCenter;
class OptionsDialog; // for integration testing
class QLabel; // JUST for labelWidget()
class QScrollArea;
class Report;
class RunnerBase;
//class RunnerParamsBase;
//class RunnerTechniqueBase;


// ParametersBase includes (move to separate file??)
#include <QPointer>

#if 0//1
#include <optionsCenter.h> // also includes optionsCenterBase.h and option.h
#else
#include "optionsCenter.h" // also includes optionsCenterBase.h and option.h
#endif//0




class RunnerBase : public QObject
{
	Q_OBJECT // required to use QPointer in OptionsCenter subclass

public:
	RunnerBase();
	virtual ~RunnerBase();

	// new: technique then params
	EC_EXPORT bool getExperimentFromUser();
	EC_EXPORT bool getAllParamsAndGlobalsFromUser(); // 

	// non-overridable functions that this base class implements (i.e., shared code!)
	// derived classes SHOULD NOT be overriding these
	// yet they need to be public, so that other objects (like the exe!) can call them.
	EC_EXPORT bool getTechniqueFromUser(); // left public so as not to break my old code
	bool getParamsFromUser();
	void Save(SaveTreeNode &);
	void Load(SaveTreeNode &);	
	
	bool hasValid_iTech() const; // no qualms about making an ACCESSOR public...i guess...
	EC_EXPORT void run();
	EC_EXPORT void runInWorkerThread();
	EC_EXPORT bool isRunningInWorkerThread() const;
	void reset();// exposes resetRun() (for CHI_runExperiment()), but with a more presentable name

	void enableTabbedDialog(bool enable = true);
	bool validate(); // exposed for error-checking in labview. NOT const, cuz it could call silent overrides...
	QString validationErrorString() const; // exposed to allow labview API to query

	// functions that derived classes must implement 
	virtual bool checkCom(); // check that CHI HARDWARE is attached to com port - CAN'T be const since CRun12A::CheckModel() isn't const... (modifies SHITLOAD of member variables)
	virtual QString className() const = 0;
	virtual int iTech() const = 0; // exposed for CHI_getTechnique()
	
	
Q_SIGNALS:
	void iTech_modified(int iTech);
	void gotParamsFromUser();

protected Q_SLOTS: // promoted to Q_SLOT to allow invokation from exe (without EC_EXPORT!). Lets SDK users roll their own UI too...
	void set_iTech(int); // base class emits signal!! also, can't be implemented in this class, without giving CRun12A, CRun12B,... a common ancestor. pointless to make it private? (set_iTech_subclass would break that encapsulation anyway)

private Q_SLOTS:
	void resetParamsViaEcDoc(int iTech);
	void showAboutDialog() const;

public: 
	class SpectatorBase; // public so Recorder classes can access it from outside Runner...but class is abstract, to prevent external instantiation!
	EC_EXPORT SpectatorBase* spectator() const; // interface so Recorders can wire directly to nested class
protected:		// nest those damn classes - but keep them visible as needed to subclasses for "nested inheritance"
	class ParametersBase;
private:
	class RunnerOptionsCenter;
	class Technique; // not polymorphic anymore - need not be visible to subclasses!
	class Cell;
	class RunStatus;
	class SystemSetup;


protected:
	ParametersBase const* parameters() const;	// getter/setter pairs for OptionsCenter subclasses (protected raw data makes me uneasy)
	Technique const* technique() const;
	//void constructTechnique(Technique*); // should get called by subclass::constructOptionsCenters() 
	void constructParameters(ParametersBase*);	// should get called by subclass::constructOptionsCenters()
	void constructSpectator(SpectatorBase*);
	void constructEcDoc(void*); // should get called by subclass::constructRunDefaults()...
	//QString techniqueNameFromID(int) const;
	void setValidationErrorString(const QString&) const; // VIOLATES CONSTNESS to set m_errorValidationString from const functions
	void addToValidationErrorString(const QString&);
	void addToValidationErrorString(const QString&) const;
	virtual void silentlyOverrideParams(); // meant to be overridden, but not called, in derived class...
	
	// override in your derived class if desired - with its dependencies listed immediately after...
	#ifndef QT_NO_DEBUG
	public: 
		EC_EXPORT virtual int singleChannelStorageRequired() const;
	protected:
	#else
		virtual int singleChannelStorageRequired() const;
	#endif	
	virtual float eh() const = 0; // NOT consolidated using runPointer(), because subclass use them too, and idiosyncratically..
	virtual float el() const = 0;
	virtual float inpcl() const = 0;
	virtual float inpsi() const = 0;
	virtual float st() const = 0;
	virtual float cl() const;
	virtual float pw() const; // avoid pure virtual, to save work in subclasses that don't use it!

	// these must be implemented in the base class (maybe would have been better off using a templated version for the trivial ones??)
	virtual bool checkCom_subclass() = 0;
	virtual void set_iTech_subclass(int i) = 0; // m_run.m_iTech = i - access CRun12A, etc. tolerate the kludgy name, since base class will emit signal so derived classes need not worry...
	virtual void run_subclass() = 0; // must call m_run.Run()
	virtual void resetRun() = 0; // reset m_run.m_iNN, etc.
	virtual bool validateParams() const = 0;
	virtual void disableBipot() = 0;
	virtual void silentlyOverrideParams_CParam_Input() = 0; // CParam::Input()
	virtual void silentlyOverrideParams_CParam_Check() = 0; // CParam::Check()
	//virtual bool isUsingSecondChannel() const = 0; // not used in the base class... just implement in subclasses that need it??
	//virtual unsigned int singleChannelStorageRequired() const = 0; // only declare this if you ACTUALLY NEED THIS in the base class! duh
	
	
	// don't forget to call these in subclass constructor!
	//virtual void constructOptionsCenters() = 0; 
	void constructRunDefaults(void* runPointer);

	// ugh, raw CRun access...
	virtual const void* constRunPointer() const = 0;
	void* runPointer() const; // ugh, NOT private, since it's needed by constructRunDefaults() in subclasses...
	//const void* runPointer() const;
	



// "validations" for run()

#ifndef QT_NO_DEBUG
//private: // meh, making these private would slightly obscure the intent of overriding in subclass (then again, they're pure virtual...)
	// already in the parade - but for validation purposes, base class needs to be made aware of these
//	virtual bool bParamE2() const = 0;
//	virtual bool bScanE2() const = 0; 
//	virtual bool bScanE2Diff() const = 0;
//	virtual bool bParamI2() const = 0;

#endif



private:
	void saveOptionsCenter(OptionsCenter*, SaveTreeNode &); // unfortunately, OptionsCenterBase::Save() isn't const
	void loadOptionsCenter(OptionsCenter*, SaveTreeNode &);
	QList<OptionsCenter*> allParamsAndGlobals() const;
	QWidget* widgetWrappedWithScrollArea(QWidget*);
	
	QWidget* aboutWidget();
	bool allParamsAndGlobalsAreValid() const;
	bool isUsingTabbedDialog() const;
	void prepareSpectator();
	void resetGlobals();
	bool validateParamsFromUser();
	bool validateParams_superclass();
	
	// hmmmmm do derived classes REALLY need direct access?? i guess write getter/setter pairs
	ParametersBase *m_parameters; // this will be polymorphic and wired into m_run...
	Technique *m_technique; 
	Cell *m_cell;
	RunStatus *m_runStatus;
	SystemSetup *m_systemSetup;
	SpectatorBase *m_spectator;
	bool m_bUseTabbedDialog;
	QString m_validationErrorString; // so that "const" validation functions can modify this
	QFuture<void> m_future;


	// this variable should ONLY be used in 4 places: constructEcDoc(), and resetParamsViaEcDoc(), constructor(null), destructor!!!!
	class CEcDoc; 
	CEcDoc *m_pEcDoc; // oh boy...well at least keep it as a pointer, to avoid dragging ecdoc.h into this header file

	// dry-run functionality to allow SDK users to test UI without instrument attached
	Q_PROPERTY(bool m_bDryRun READ bDryRun WRITE set_bDryRun)
	bool bDryRun() const;
	void set_bDryRun(bool);
	bool m_bDryRun;

	// another parade? oh boy...note that RunBase wouldn't help, since m_bDummy would be declared in EACH SUBCLASS
	// these parameters are DANGEROUSLY accessed via runPointer()... because it's so tedious to wire up EVERY subclass just to add a new base-class feature
	// these parameters are ONLY also accessed through RunnerBase's UNDIFFERENTIATED OPTIONSCENTERS: Cell, RunStatus
	// NOT including m_iTech, since that one has special signals/slots
	Q_PROPERTY(bool m_bCathodicPositive READ bCathodicPositive WRITE set_bCathodicPositive)
	bool bCathodicPositive() const;
	void set_bCathodicPositive(bool);
	Q_PROPERTY(float m_bDummy READ bDummy WRITE set_bDummy) // n.b. this DOES work with pure virtuals!
	bool bDummy() const;
	void set_bDummy(bool);
	Q_PROPERTY(int m_iStopExp READ iStopExp WRITE set_iStopExp)
	int iStopExp() const;
	void set_iStopExp(int);
	Q_PROPERTY(float m_stopi READ stopi WRITE set_stopi)
	float stopi() const;
	void set_stopi(float);
	Q_PROPERTY(float m_stopi2 READ stopi2 WRITE set_stopi2)
	float stopi2() const;
	void set_stopi2(float);
	Q_PROPERTY(float m_stopq READ stopq WRITE set_stopq)
	float stopq() const;
	void set_stopq(float);
	Q_PROPERTY(int g_iBaud READ iBaud WRITE set_iBaud)
	int iBaud() const;
	void set_iBaud(int i) const; // writes to global variable...
	Q_PROPERTY(int m_iHz READ iHz WRITE set_iHz)
	int iHz() const;
	void set_iHz(int i);

// allow RunnerTechniqueBase to call the pure virtual function set_iTech  in RunnerTechniqueBase::registerAllOptions()
//friend class RunnerBase::Technique;


// integration testing stuff
#ifndef QT_NO_DEBUG
Q_SIGNALS:
	EC_EXPORT void integrationTest_acceptDialogSignal();
public:
	EC_EXPORT void integrationTest_nextTechnique();
	EC_EXPORT int integrationTest_numTechniques() const;
#endif
private:
	void integrationTest_connect(OptionsDialog&);



	// todo: enum shared amongst Runners - here or in RunnerOptionsBase?	


};




// modifications to OptionsCenter that I would like to share with ParametersBase, Cell, RunStatus, SystemSetup, etc.
// hey, lookit, don't need to mention all the pure virtuals i'm not making, since this is an abstract class anyway!
class RunnerBase::RunnerOptionsCenter : public OptionsCenter
{
public:
	RunnerOptionsCenter(RunnerBase*);
protected:
	RunnerBase* baseParent() const; // accessor to m_parent; parent() used extensively in Parameters subclasses (avoid name collision/confusion)
	virtual void registerOption(const Option&); // override to document Q_PROPERTY for SDK users
private:
	RunnerBase* m_parent; // concrete Parameter classes need to recast this to implement "parent()", and I don't want TWO parent() accessors floating around
};

// ---------------------------------------------------------------------------
// other tedious boilerplate tasks to do (but can't be moved to OptionsCenterBase) - e.g. in class Child
	// get TYPE names right downstairs (quick search and replace)
	// Parent::Parent() : m_childOptions(this)
	// interface to parent class (i.e. flesh out readOptionFromParent, writeOptionToParent)



class RunnerBase::ParametersBase : public RunnerBase::RunnerOptionsCenter
{
	Q_OBJECT // required for tr(.)!
public:
	// no default constructor! ensures Parent will initialize m_parent
	ParametersBase(RunnerBase*);
	//virtual ~RunnerParamsBase(); // unnecessary? the child should NEVER delete the parent!!
	bool isEmpty() const;
	virtual QWidget* masterWidget(); // ugly but workable default for all models, all techniques. oops this is public!

protected Q_SLOTS:;
	virtual void userChangedElectrode2CV(int) = 0; // to be implemented in derived classes

private Q_SLOTS:; // http://qt-project.org/faq/answer/intellisense_does_not_work_for_my_qt_application._whats_wrong
	void set_iTech(int iTech);  // common error-checking; used to be pure virtual - gettin fancy! only invoked via signal/slot, so forget using ugly _subclass() naming

protected:
	virtual void registerAllOptions(); // do not override! shouldn't get called anyway
	int iTech() const;
	virtual void registerParamSubset(int iTech) = 0; // subset depends on both Model (subclass) and Technique (argument). 
	
	virtual QString masterWidgetTitle() const;
	void pullWidgetOffForm(QWidget *form, QWidget *widget);
	QLabel* labelWidget(const Option&) const;
	virtual bool needsCustomMasterWidget() const = 0;
	
	// the following functions can be overridden in your derived class if desired
	virtual bool currentTechniqueCanUseBipot() const;
	virtual QWidget* customMasterWidget();	
	virtual QWidget* makeChannel2Widget(QWidget*) = 0; // got a bit confused during Runner6C, so made this pure virtual as it should be...
	virtual QWidget* makeChannel2WidgetCV(QWidget *, Option& o_e_1, Option& o_e2d, Option& o_iSens2, Option& o_iE2);
	virtual QWidget* makeChannel2WidgetIT(QWidget *channel1, Option& o_e_1, Option& o_iSens2, Option& o_bParamE2, Option& o_bParamI2);

private:
	bool isInitialized() const;
	int m_iTech; // needed this state variable after all, in case Technique changes after initialization...

friend void RunnerBase::constructParameters(RunnerBase::ParametersBase*); // requires either visual studio 2012 or inbound friendship to work on private functions?	
//friend bool RunnerBase::allParamsAndGlobalsAreValid() const; // doesn't grant friendship to OptionsCenterBase

/// BOILERPLATE, UNIMPLEMENTED STUFF - consider deleting: stuff that the derived class must implement - here as a reminder
public:
	virtual QString className() const = 0; 

protected:
	// need to know WHAT options i have, before implementing these.
	virtual void readOptionFromParent(Option &) = 0;
	virtual void writeOptionToParent(const Option &) const = 0;
///


// Q_PROPERTY allows all subclasses to share the same check-in/check-out code! only YOU can prevent code duplication
// can't move to cpp file...(just try it, punk!) - COULD move it to hpp file, but how annoying is that??
// http://stackoverflow.com/questions/972152/how-to-create-a-template-function-within-a-class-c
// template functions are really feature-limited: no overloading, no inheritance (virtual functions)
protected:	
	template <class Runner> void readOptionFromParentProperty(Runner const *parent, Option &o) // can't overload...
	{
		// error-checking: parent exists
		if (parent == NULL)
		{
			qWarning("%s null parent", __FUNCTION__);
			return;
		}

		QVariant value = parent->property(o.ID().toAscii().data());
		Q_ASSERT(value.isValid());
		o.setValue(value);
	}

	template <class Runner> void writeOptionToParentProperty(Runner *parent, const Option &o) const
	{
		// error-checking: parent exists
		if (parent == NULL)
		{
			qWarning("%s null parent", __FUNCTION__);
			return;
		}

		// hey, no need to do any type-casting! QObject::setProperty() handles it fine, for built-in data types!?
		bool success; 
		success = parent->setProperty(o.ID().toAscii().data(), o.value());
		Q_ASSERT(success);
	}


// stupid SFINAE antics
#ifdef CHI_OPTIONS_CENTER_CHECK_SAVE
protected:
	virtual void checkParentHasLoad(); //- e.g.,  bool loadFunctionFound = TypeHasSave<Foo>::hasLoad;
	virtual void checkParentHasSave(); //- e.g.,  bool saveFunctionFound = TypeHasSave<Foo>::hasSave;
#endif
	
};



// Recorders will poll into this as THEY see fit
class RunnerBase::SpectatorBase : public QObject
{
	Q_OBJECT // required by connect() and slots
public:
	SpectatorBase(RunnerBase*);
	virtual ~SpectatorBase();
	//void Look(); 
	const Report& Look(); // external harness should use only const interfaces on Report
	virtual bool requiresNaiveRecording() const;
	EC_EXPORT virtual bool runnerIsFinished() const; // override for 1600

protected Q_SLOTS:;
	void prepareToLookBase();

private Q_SLOTS:;
	void set_iTech(int);
	

protected:
	RunnerBase* baseParent() const;
	int technique() const; // will be used heavily by concrete subclasses
	void warnUserAboutPostRunData() const;
	virtual void prepareToLook() = 0; // (1) depends on concrete Runner, and (2) prevents direct instantiation of base class!

	//class Report;

	// helper functions to be used by subclasses - meh, Report API is in too much flux...
	//void watchInt(const QString &name, int const volatile* address);
	//void watchVector(const QString &name, float const volatile* startAddress, int const volatile* size);
	//void watchSeries(const QString &name, float const volatile* x, float const volatile* y, int const volatile* n);
	Report *m_report;
	
friend void RunnerBase::constructSpectator(RunnerBase::SpectatorBase*); // for access to set_iTech

private:
	RunnerBase *m_baseParent; // doesn't really need to be QPointer, does it?
	int m_iTech;
	QMutex m_mutex;
	
};


















#endif // !defined RUNNERBASE_H








