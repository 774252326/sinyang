// option.h
// my custom replacement for QtProperty (i didn't name it Property, in order to avoid any FURTHER confusion with the Qt Property system, which is NOT the same as the QtProperty class in the Qt Solutions archive)
//
// this is UGLY and not object-oriented at all. but polymorphic Options would require casting in OptionsCenter to access unique options (e.g., addEnum). get the ugly code OUT OF THE WAY so that future code will be cleaner.
// it's KIND of hard to add new types(shouldn't be often), but here's what i had to modify for Enum:
//	- enum Type (WATCH out for custom data types)
//	- isSupportedType()
//	- internalType() - if you are using a custom data type
//	- range validation (NUMERICAL only - decided against it for Enum)
//		- hasRange()
//		- liesInRange() - since enums have range checking of sorts (available options ONLY)
//  - typeName() - for non-nonsensical name for user data type (dom will save this!)

#if ((defined _MSC_VER) && (_MSC_VER > 1000)) || ((defined __GNUC__) && (__GNUC__ > 3))
#	pragma once
#else
#	error "Custom header guard needed - have fun editing all N files in this project...just HOW portable is pragma once??"
#endif

#include "standard_definitions_head.h"

// currently just for EnumDataArray
#if ARRAY_TYPE == ARRAY_TYPE_VECTOR 
	#include <QVector>
#elif ARRAY_TYPE == ARRAY_TYPE_LIST
	#include <QList>
#else
	#error unknown ARRAY_TYPE
#endif // ARRAY_TYPE

#include <QPointer>
#include <QString>
#include <QVariant>
#include "optionWidgets.h" // ugh forward declaration is INSUFFICIENT for QPointer member variable

class QIcon; // wheee, LOOKS like forward declaration suffices for const ref argument AND return values?!
//class QWidget;
class SaveTreeNode;


// class Option is not meant to be subclassed...couldn't quite figure out how to lock it down in code from http://www.parashift.com/c++-faq-lite/strange-inheritance.html#faq-23.11
class Option : public QObject
{
	Q_OBJECT	// for QPointer<Option> support in other classes
public:
	enum Type	// a SUBSET of QVariant's types...
	{
		Type_Invalid = QVariant::Invalid,

		Type_Bool = QVariant::Bool,
		Type_Color = QVariant::Color,
		Type_Double = QVariant::Double,
		Type_Enum = QMetaType::User, // QtProperty uses Q_DECLARE_METATYPE...deal with it if a SECOND user-defined type is needed...no # overlap if there's just one...this enum is only used internally anyways
		Type_Float = QMetaType::Float, // this CAN be returned by QVariant::type??
		Type_Font = QVariant::Font,
		Type_Int = QVariant::Int,
		Type_String = QVariant::String,
	};

	enum WidgetType // hmm simply not USED in OptionsCenterBase...
	{
		Widget_Invalid,

		// absolute #'s are only used internally, so order shouldn't matter
		Widget_Bool_CheckBox,
		Widget_Color_Edit,
		Widget_Double_Knob,
		Widget_Double_Slider, 
			Widget_Double_SliderReverse, Widget_Double_SliderVertical, Widget_Double_SliderUpsideDown,
		Widget_Double_SpinBox,
		Widget_Double_TextCompass,
		Widget_Enum_ComboBox,
		Widget_Enum_RadioBox,
		Widget_Enum_ListBox,
		Widget_Font_Edit,
		Widget_Int_Dial,
		Widget_Int_ScrollBar, 
			Widget_Int_ScrollBarReverse, Widget_Int_ScrollBarVertical, Widget_Int_ScrollBarUpsideDown,
		Widget_Int_Slider, 
			Widget_Int_SliderReverse, Widget_Int_SliderVertical, Widget_Int_SliderUpsideDown,
		Widget_Int_SpinBox,		
		Widget_String_LineEdit,


		// legacy name support (for old code - namely, OptionsCenter - refactor later...)
		Widget_CheckBox = Widget_Bool_CheckBox,
		Widget_SpinBoxDouble = Widget_Double_SpinBox,
		Widget_SpinBoxInt = Widget_Int_SpinBox,
		Widget_LineEdit = Widget_String_LineEdit,
		Widget_ComboBox = Widget_Enum_ComboBox,
		Widget_RadioButtonBox = Widget_Enum_RadioBox,
		Widget_FontEdit = Widget_Font_Edit,
		Widget_ColorEdit = Widget_Color_Edit,
		Widget_TextCompass = Widget_Double_TextCompass,
		Widget_SliderInt = Widget_Int_Slider,
		Widget_ScrollBarInt = Widget_Int_ScrollBar,
		Widget_DialInt = Widget_Int_Dial,
		Widget_SliderDouble = Widget_Double_Slider,
		Widget_KnobDouble = Widget_Double_Knob
	};

	// default constructor - chosen to allow cleaner syntax in OptionsCenter
	Option();
	~Option();

	void declare(Type, const QString &);
	void declareAsNull();
	bool isDeclared() const;
    void undeclare();

	Type type() const; // read-only! void setType(Type);	// erases current value if incompatible. but you'll set types FIRST before values, right?
	QString typeName() const;
	QString ID() const;	

	// templated functions in a non-template class requires 1 of 2 undesirable options (http://www.parashift.com/c++-faq-lite/templates.html#faq-35.13)
	// - inlining IN the header file, but my setValue function has non-trivial logic
	// - manually declaring the concretetemplates in the cpp file - which loses the genericity i wanted	
	//template<typename T> void setValue(const T &inputValue); 
	void setValue (const QVariant &v);
	QVariant value() const;
	bool isValid() const;

	// SaveTreeNode interface
	bool operator==(const SaveTreeNode &) const;
	void readFromSaveNode(const SaveTreeNode &inputNode);
	void writeToSaveNode(SaveTreeNode &outputNode) const;

	// Backup interface
	void readFromBackup();
	void writeToBackup();

	// Widget main interface
	void readFromWidget(); 
	void writeToWidget();
	void setWidgetType(WidgetType);
	QWidget* widget();
	QWidget* widget() const;

	// Widget settings and helpers
	bool widgetTypeIsValid(WidgetType) const;	
	void setLabel(const QString &);
	QString label() const;
	void setStatusTip(const QString &);
	QString statusTip() const;
	void setToolTip(const QString &);
	QString toolTip() const;
	void setWhatsThis(const QString &);
	QString whatsThis() const;
	

	// Widget interface: type-specific
	QVariant maximum() const;
	void setMaximum(const QVariant &); // numeric types ONLY
	void clearMaximum();

	QVariant minimum() const;
	void setMinimum(const QVariant &);
	void clearMinimum();

	void setRange(const QVariant &min, const QVariant &max);
	void clearRange();
	bool hasRange() const;

	QVariant step() const;
	void setStep(const QVariant &);
	void clearStep();
	bool hasStep() const;

	void addEnum(int, const QString &);
	void addEnum(int, const QString &, const QIcon &);
	int enumCount() const;
	QIcon enumIconByIndex(int) const;
	QString enumTextByIndex(int) const;
	int enumValueByIndex(int) const;
	int enumIndexByValue(int) const;
	bool enumHasValue(int) const;

	// Widget-specific convenience functions (basically patched onto default QFormLayout
	QWidget* labeledCheckBox();
	QWidget* labeledRadioBox();

Q_SIGNALS:
	void widgetModified();

protected: // Option is not meant to be subclassed and thus protected members are meaningless
private:

// tiny private data type declared in cpp file; JUST for stupid enums...
	class EnumData; // helper 
	#if ARRAY_TYPE == ARRAY_TYPE_VECTOR // uglier and uglier still
		typedef QVector<EnumData> EnumDataArray;
	#elif ARRAY_TYPE == ARRAY_TYPE_LIST
		typedef QList<EnumData> EnumDataArray;
	#else
		#error unknown ARRAY_TYPE
	#endif // ARRAY_TYPE

// helper functions
	//Option(); // private constructor doesn't work to prevent subclassing - only instantiation - see also http://www.parashift.com/c++-faq-lite/strange-inheritance.html#faq-23.11
    void reset();
    
	// TYPE validity
	bool hasValidType(const QVariant &) const;
	bool isSupportedType(Type) const;	
	void makeValidType(const QVariant &in, QVariant &out) const;
	void makeValidType(QVariant &);
	QVariant::Type internalType() const;
	
	// type-dependent functions, MOSTLY intended for use with widgets
	bool greaterThan(const QVariant&, const QVariant&) const;
	bool greaterThanOrEquals(const QVariant&, const QVariant&) const;
	bool lessThan(const QVariant&, const QVariant&) const;
	bool lessThanOrEquals(const QVariant&, const QVariant&) const;
	bool hasValidValue(const QVariant &) const; // superset of hasValidType
	bool liesInRange(const QVariant &min, const QVariant &val, const QVariant &max) const;
	void setMinMax(const QVariant &, bool wantMax);
	QVariant systemMin() const;
	QVariant systemMax() const;
	QVariant systemMinMax(bool) const;	

	// pure widget functions
	WidgetType widgetType() const;
	WidgetType defaultWidgetType(Type) const;	
	OptionWidget* widgetWrapper();
	void manufactureWidget();




// data
	// FUNDAMENTAL data
	bool m_declared;
	QString m_ID;
	QVariant m_value; // default QVariant constructor uses QVariant::Invalid
	QVariant m_backupValue;
	Type m_type;

	// constants: SaveTreeNode
	static QString const M_ATTR_ID;
	static QString const M_ATTR_TYPE;
	static QString const M_ATTR_TYPE_ENUM;

	// constants: Widgets
	// ??

	// "mandatory" (widgetType-independent) widget data like tooltip, statustip, ...
	WidgetType m_widgetType;
//	QPointer<QWidget> m_widget; 
	QPointer<OptionWidget> m_widgetWrapper;
	QString m_label;
	QString m_statusTip;
	QString m_toolTip;
	QString m_whatsThis;

	// OPTIONAL widget data like min/max that gets ignored for unsupported types. eat redundant storage?
	QVariant m_maximum, m_minimum;
	QVariant m_step; // for SpinBoxes...
	EnumDataArray m_enums; // for QComboBox. radio buttons?



	


};

// todo
	// eliminate legacy enum WidgetType's
	// enums: upgrade storage to QHash or QMap?
		// esp for enumIndexByValue
	// backwards compatibility: what if value from file is out of range/invalid?
		// e.g. enum change in program
		// e.g. program changes to stricter max/min
		// current behavior: ignore out-of-range value, assign out-of-date enum...
			// just HOPE that parent classes extend their enums in ORDER! 
				// UI order via addEnum() is arbitrary and adjustable ANY time


// ---------------------------------------------------


// QtProperty shit that ain't useful right now

// QtProperty stuff that is NOT in my current design but might be useful someday
	//void 	setEnabled ( bool enable )
	//bool 	isEnabled () const
	//bool 	hasValue () const

	//QVariant 	attributeValue ( const QString & attribute ) const
	//void 	setAttribute ( const QString & attribute, const QVariant & value )
	//int 	attributeType ( int propertyType, const QString & attribute ) const
	//QStringList 	attributes ( int propertyType ) const

	//void 	addSubProperty ( QtProperty * property )
	//void 	insertSubProperty ( QtProperty * property, QtProperty * precedingProperty )
	//void 	removeSubProperty ( QtProperty * property )
	//QList<QtProperty *> 	subProperties () const


// QtProperty stuff that relies on Trolltech's QtPropertyManager framework and will probably NEVER be implemented in my design
	//QIcon 	valueIcon () const 
	//QString 	valueText () const

	//void 	setModified ( bool modified ) // smells like signals/slots, which i'm trying to AVOID here in a generic framework
	//bool 	isModified () const

	//bool 	isPropertyTypeSupported ( int propertyType ) const
	//int 	propertyType ( const QtProperty * property ) const
	//QtVariantProperty * 	variantProperty ( const QtProperty * property ) const


