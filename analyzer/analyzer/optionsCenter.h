// optionsCenter.h
// This class provides centralized management for user-accessible settings.
// The guiding principle is that anything the user can modify, should also be saved to disk.
//
// to derive from this abstract base class, you must implement the following functions:
	// public:
	//	virtual QString className() const = 0; 
	// protected:
	// 	virtual void readOptionFromParent(Option &) = 0;
	//	virtual void writeOptionToParent(const Option &) const = 0;
	//#ifdef CHI_OPTIONS_CENTER_CHECK_SAVE
	// protected:
	//	virtual void checkParentHasLoad() = 0; - e.g.,  bool loadFunctionFound = TypeHasSave<Foo>::hasLoad;
	//	virtual void checkParentHasSave() = 0; - e.g.,  bool saveFunctionFound = TypeHasSave<Foo>::hasSave;
	//#endif
// other tedious boilerplate tasks to do (but can't be moved to OptionsCenterBase) - e.g. in class Child
	// add member variable Parent *const m_parent
	// Child constructor
		// call checkParentHasLoad()!!
		// Child::Child(Parent *p) : m_parent(p) - no default constructor! ensures Parent will initialize parent
		// Parent::Parent() : m_childOptions(this)
	// interface to parent class
		// basic option: manually populate read/writeOptionFromParent with Parent's accessors/modifiers
		// fancy option: Parent: Q_OBJECT Q_PROPERTIES(...) - and call 
			// + awesomely elegant, uncased Child code: o.setValue(parent()->property(o.ID().toAscii().data()));
			// - have to match ID property tags between Child and Parent
			// - you have to write the modifiers/accessors independently of the Q_PROPERTIES macro
				// basically "conservation of work", so might as well isolate all the messiness to Child (stick with basic)
			// - the SPIRIT of OptionsCenter is to keep Parent as CLEAN as possible, and this does not do that.

// this is NOT quite that easy to use - rather temperamental to add to an existing class

#if ((defined _MSC_VER) && (_MSC_VER > 1000)) || ((defined __GNUC__) && (__GNUC__ > 3))
#	pragma once
#else
#	error "Custom header guard needed - have fun editing all N files in this project...just HOW portable is pragma once??"
#endif

#include "standard_definitions_head.h"

#include <QFormLayout> // ugh forward declaration fails for QPointer...safety first!
//#include <QObject>
#include <QPointer>
#include "option.h"
#include "optionsCenterBase.h"



class OptionsCenter : public OptionsCenterBase//, public QObject <-- sadly, moc error unless BASE class inherits QObject?
{
	Q_OBJECT // needed by signals/slots here!! (but not in OptionsCenterBase?)
public:
	OptionsCenter();
	virtual ~OptionsCenter();

	// inherited from OptionsCenterBase - here as a reminder to implement in subclasses of OptionsCenter ("bequeathed immediately")
	virtual QString className() const = 0;

	// new function! override to make a better looking default dialog box.
	// exposed so that you COULD in principle assemble widgets from multiple OptionsCenters into a single dialog
	virtual QWidget* masterWidget();

	// inherited by derived class. what the heck, if even the WIDGET is public, no harm in making its title public (and i need it...)
	virtual QString masterWidgetTitle() const = 0; // i'd make this static if i could, but static pure virtual is a PARADOX	
	
	QWidget* initialFocusWidget() const; // WHY did i have this as private but the WRITE as protected?? just because i could??
	static QString const M_ICON_FILE; // meh keep here for now...what could lack of accessor possibly hurt? it's READ ONLY

public Q_SLOTS:
	bool getOptionsFromUser(); // convenience function - gets values immediately from dialog. must be exposed as a slot to be called by QAction's
	void commitAllFromWidget(); // public in case parent class needs to call them
	void checkOutAllToWidget(); // slots because they're used by Apply button
	void restoreAllFromBackup();

//protected Q_SLOTS: // in case derived classes need to call these programmatically

Q_SIGNALS:
	void widgetsModified();

protected:
//	typedef OptionsCenterBase::Mode Mode;

	// inherited from OptionsCenterBase and overridden here for widget support
	virtual void readWriteOption(Mode, Option&); // call the base class to leverage functionality!
	virtual bool modeIsSupported(Mode) const;
	virtual void registerOption(const Option&);

	// inherited from OptionsCenterBase - here as a reminder
	virtual void readOptionFromParent(Option &) = 0;
	virtual void writeOptionToParent(const Option &) const = 0;

	// helper functions accessible to derived classes	
	void getOptionsFromUser(QWidget*, const QString&, QWidget*); // for exposing sub-widgets to user, in subclasses
	void writeAllToWidget();
	void readAllFromWidget();
	void writeAllToBackup(); // mainly for hitting cancel or reset on a widget
	void readAllFromBackup();
	QWidget* defaultWidget();
	bool defaultWidgetExists() const;
	bool defaultWidgetIsEmpty() const;	
	void setInitialFocusWidget(QWidget*); // customize in derived classes...
	//void createWidgetRelays();
	void enableFancyDialogButtons(bool = true);
	bool fancyDialogButtons() const;
	void hideCancelButton();
	bool cancelButtonIsHidden() const;

	// data accessible to derived classes
	QPointer<QWidget> m_masterWidget; // hmmmmm
	
private:
	// basic, elemental functionality
	void readOptionFromWidget(Option &);
	void writeOptionToWidget(Option &);
	void readOptionFromBackup(Option &);
	void writeOptionToBackup(Option &);

	// widget functions
	void constructDefaultWidget();

	// data hidden to derived classes
	bool m_bFancyDialogButtons;
	bool m_bHideCancelButton;
	QPointer<QFormLayout> m_defaultLayout;
	QPointer<QWidget> m_defaultWidget;
	QPointer<QWidget> m_initialFocusWidget; // kludge to get initial tab order in default widget...



	// inherited from OptionsCenterBase
	// ------------------------------------------------------------------------------------
	#ifdef CHI_OPTIONS_CENTER_CHECK_SAVE
	protected:
		virtual void checkParentHasLoad() = 0;
		virtual void checkParentHasSave() = 0;
	#endif

		// sample subclass implementation
		//#ifdef CHI_OPTIONS_CENTER_CHECK_SAVE
		//void FooOptions::checkParentHasLoad() { // n.b. can't inline this shit because of circular dependency otherwise
		//	bool loadFunctionFound = TypeHasLoad<Foo>::hasLoad;
		//	if (!loadFunctionFound) {
		//		qWarning("FATAL ERROR: %s parent object has no load function", className().toAscii().data());
		//		exit(1);
		//	}
		//}

		//void FooOptions::checkParentHasSave() { 
		//	bool saveFunctionFound = TypeHasSave<Foo>::hasSave;
		//	if (!saveFunctionFound) {
		//		qWarning("FATAL ERROR: %s parent object has no save function",  className().toAscii().data());
		//		exit(1);
		//	}
		//}
		//#endif // defined CHI_OPTIONS_CENTER_CHECK_SAVE


};




