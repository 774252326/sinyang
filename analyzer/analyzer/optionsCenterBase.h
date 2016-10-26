// currently the same as OptionsCenter but without widgets/dialog support
//
// to derive from this abstract base class, you must implement the following functions:
	// public:
	//	virtual QString className() const = 0; 
	// protected:
	// 	virtual void readOptionFromParent(Option &) = 0;
	//	virtual void writeOptionToParent(const Option &) const = 0;
	//  virtual void registerAllOptions() = 0; - and call this in the constructor.
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

// wtf this doesn't compile in qt creator?!
//#if T_ARRAY == QVector // uh, keep this out of standard_definitions_head since not everybody needs T_ARRAY??
//#	include <QVector>
//#elif T_ARRAY == QList
//#	include <QList>
//#else
//#	error unknown T_ARRAY type
//#endif

#include <QObject>
#include <QPointer>
#include "option.h"
#include "saveTreeNode.h"
#include "saveTreeObject.h"


#if ARRAY_TYPE == ARRAY_TYPE_VECTOR
	#include <QVector>
	typedef QVector< QPointer<Option> > OptionArray;
#elif ARRAY_TYPE == ARRAY_TYPE_LIST
	#include <QList>
	typedef QList< QPointer<Option> > OptionArray;
#else
	#error unknown ARRAY_TYPE
#endif // ARRAY_TYPE


#ifndef QT_NO_DEBUG
#	define CHI_OPTIONS_CENTER_CHECK_SAVE
#endif


// ------------------------------------------------------------------------------------
class OptionsCenterBase : public QObject// : public SaveTreeObject
{
	//Q_OBJECT // currently not needed - but moc error if it's OptionsCenter that (multiply) inherits QObject...
public:
	OptionsCenterBase();
	virtual ~OptionsCenterBase();

	// Uppercased functions to terminate the tree of Load/Save
	// all OptionsCenters are created equal - they all load/save nodes THE SAME WAY
	//void Load(SaveTreeNode &);
	//void Save(SaveTreeNode &); // try upgrading it to SaveTreeObject? i may regret this...
	virtual void Load(SaveTreeNode &);
	virtual void Save(SaveTreeNode &);
	virtual QString className_STO() const;
	

	// class's name is given by "birthmark" and will be read off by parent class in Load/Save operation
	virtual QString className() const = 0; 

    // exposed for use in libec's RunnerBase
    bool allOptionsAreValid(); // NOT const because it reads from parent before validating
    
	// aha add argument INDEPENDENT checking for functions in class i think? but where will the check be called??
		// nah just set a knowsParent bool flag internally, which gets toggled by setParent




protected:	
	enum Mode
	{
		Mode_Write_Save,
	//	Mode_First = Mode_Write_Save,
		Mode_Read_Save,
		Mode_Write_Parent,
		Mode_Read_Parent,
		Mode_Write_Widget,				
		Mode_Read_Widget,
		Mode_Write_Backup,
		Mode_Read_Backup
	//	Mode_Last = Mode_Read_Widget // hmmmm you have to ignore these modes here...
	};
	

	// main functionality 
	void readWriteAll(Mode);
	void writeAllToParent();
	void readAllFromParent();
	
	// inherited members that OptionsCenter (as a superset with widgets) needs to override
	virtual void readWriteOption(Mode, Option&); // call the base class to leverage functionality!
	virtual bool modeIsSupported(Mode) const;
	virtual void registerOption(const Option &);

	// inherited members that concrete derived classes must implement (need to know Parent's type, interface)
	virtual void readOptionFromParent(Option &) = 0;
	virtual void writeOptionToParent(const Option &) const = 0;
	virtual void registerAllOptions() = 0;

	// INTERNAL comparison of Options within OptionsCenter and hence not Options::compare.
	// for use in readOptionFromParent to case Option to base class function.
	// base implementation just checks that the object pointers are the same
	// derived class could override in a number of ways - e.g. comparing id tags
	virtual bool compare(const Option&, const Option&) const;

	// Options interface - for use by child abstract class OptionsCenter as well as concrete grandchildren
	Option& getOption(int);	
	const Option& getOption(int) const;
	bool isValidOptionIndex(int) const;	
	int numOptions() const;
	bool optionCanBeRegistered(const Option &) const;
	void unregisterAllOptions();


private:
	// main functionality (base class)
	void readOptionFromSaveTree(Option &);
	void writeOptionToSaveTree(const Option &);
	
	// Options interface - expose as protected ONLY AS NEEDED, since grandchildren would see them too...		
	Option& nullOption(); 
	const Option& nullOption() const;
    const Option& getOption_shared(int) const;

	// Option/SaveTreeNode accounting and conversion
	void saveLoadExternalNode(SaveTreeNode&, Mode);
	void storeInputSaveNode(SaveTreeNode&);
	SaveTreeNode& inputSaveNode();
	void storeOutputSaveNode(SaveTreeNode&);
	SaveTreeNode& outputSaveNode();	
	Option& findOptionFromSaveNode(const SaveTreeNode&, bool *success);
	SaveTreeNode findSaveNodeFromOption(const SaveTreeNode&, const Option&, bool *success);

	// data hidden from derived classes
	OptionArray m_optionPtrArray; // internal ALIAS REGISTRY for Options in derived classes - previously T_ARRAY<Option*> m_optionPtrs; 
	Option m_nullOption;
	SaveTreeNode m_inputSaveNode;
	SaveTreeNode m_outputSaveNode;



	// ------------------------------------------------------------------------------------
	// the block below is intended to check the parent class (e.g., Plot, which owns a PlotOptionsCenter) for correct Save() and Load()
	// design rationale:
	// - the typical approach would be to give Plot's base class a pure virtual Save function, but
	//		- i do NOT want to have to subclass EVERY class that owns an OptionsCenter from another class
	//		- easy to forget, esp for other coders
	//		- things might get messy? dunno how well that would play with QObject inheritance, etc.
	// - put ALL the fanciness in OptionsCenter - the parent class can be plain vanilla - UNTIL it adds a member OptionsCenter...
	// - pure virtual functions remind the coder of PlotOptionsCenter to include this check...lame...
	// limitations
	// - only checks the class declaration, not whether the function is implemented
	// - you have to remember to call checkParentHasLoad() yourself in the subclass, e.g. PlotOptionsCenter	

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

	#ifdef CHI_OPTIONS_CENTER_CHECK_SAVE
	protected:

		virtual void checkParentHasLoad() = 0;
		virtual void checkParentHasSave() = 0;

		// FireAphis' nice portable SFINAE code! http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
		template <class Type>
		class TypeHasSave
		{
		private:
			virtual void killInstances() = 0; // this class is never meant to be instantiated

			// This type won't compile if the second template parameter isn't of type T,
			// so I can put a function pointer type in the first parameter and the function
			// itself in the second thus checking that the function has a specific signature.
			template <typename T, T> struct TypeCheck;

			typedef char Yes;
			struct No { char c[2]; };

			// A helper struct to hold the declaration of the function pointer.
			// Change it if the function signature changes.
			template <typename T> struct Signature
			{
				typedef void (T::*fptr)(SaveTreeNode&);					
			};

			template <typename T> static Yes HasFunction(
				TypeCheck< typename Signature<T>::fptr, &T::Save >*);			
			template <typename T> static No  HasFunction(...);

		public:
			static bool const hasSave = (sizeof(HasFunction<Type>(0)) == sizeof(Yes));
		};
		
		template <class Type>
		class TypeHasLoad
		{
		private:
			virtual void killInstances() = 0; // this class is never meant to be instantiated
			template <typename T, T> struct TypeCheck;
			typedef char Yes;
			struct No { char c[2]; };
			template <typename T> struct Signature
			{
				typedef void (T::*fptr)(SaveTreeNode&);							// <=== change function argument types here
			};

			template <typename T> static Yes HasFunction(
				TypeCheck< typename Signature<T>::fptr, &T::Load >*);			// <=== change function name here
			template <typename T> static No  HasFunction(...);

		public:
			static bool const hasLoad = (sizeof(HasFunction<Type>(0)) == sizeof(Yes));
		};
	#endif // !defined QT_NO_DEBUG
	// ------------------------------------------------------------------------------------
}; // class OptionsCenterWidgetless













// Remarks ==================================================
// Originally proposed: Option::Option(type, name) to be the ONLY constructor, + const Option::m_name, Option::m_type
// + m_type, m_name get initialized EXACTLY once for each instantiated Option
// - This results in nothing but AWKWARD design options for OptionsCenter and her subclasses:
//  > Option m_o; ... OptionsCenter() : m_o(type, title)
//		+ cleanest implementation
//		+ no call to delete (or other pointer bullshit)
//		- (type, title) init is separated from other initialization code, which potentially will be written a LOT
//	> Option *m_po; ... m_po = new Option(type, title); m_po->setTooltip(...)
//		+ (type, title) init can be grouped with other init code
//		- have to call delete in ~OptionsCenter() anyway, so all related code is NOT centralized
//		- type/title reassignments are still possible if a careless developer reassigns a pointer
//	> QScopedPointer<Option> m_spo; ... m_spo.reset(new Option (type, title));
//		+ (type, title) init can be grouped with other init code
//		+ no call to delete required			
//		- later reassignments (via calls to reset) are still possible
//		- awkward, non-intuitive syntax
//	> subclass QScopedPointer<Option> to OptionHandle 
//		+ (type, title) init can be grouped with other init code
//		+ no call to delete required	
//		+ can make the OptionsCenter syntax as clean as I want
//		+ later reassignments? can PROBABLY have OptionHandle::init() set a flag so it is callable only once...
//		- ...so why not just modify Option::initName() to set a flag, instead of creating an entire new class (well, reusable? if i ever need this approach i'll read my notes here maybe)
//		- a BIT indirect, don't you think? to learn Option API you also must read OptionHandle header...




