// optionWidgets.h
// polymorphic class that wraps a QWidget for use with Option
// warning: the QWidgets are ALWAYS initialized with Qt parent NULL
// the "owner" is the Option in which this wrapper is embedded
// ALL this convolutedness JUST to have pure virtual functions...

// +: an incomplete new widget type will be a COMPILE error (pure virtual functions). trying to minimize breaking changes when adding a new widget
// -: an incomplete new data type will be a RUNTIME debug error - but this should be RARER, right?

#if ((defined _MSC_VER) && (_MSC_VER > 1000)) || ((defined __GNUC__) && (__GNUC__ > 3))
#	pragma once
#else
#	error "Custom header guard needed - have fun editing all N files in this project...just HOW portable is pragma once??"
#endif

#include "standard_definitions_head.h"

//#include <QBoxLayout> // for stupid Direction enum...wait this is included in sliders.h, right?
#include <QObject>
#include <QPointer>
#include <QVariant>
#include <QWidget>
//#include "option.h" - CANNOT have this! option.h needs optionWidgets.h for stupid QPointer
#include "sliders.h" // for IntSlider::Type enum

class QCheckBox;
class QComboBox;
class QLineEdit;
class QListWidget;
class QSlider;
class QWidget;
class Option;
class ColorEdit;
class DoubleSlider;
class FontEdit;
class IntSlider;
class RadioButtonBox;
class SpinBoxDouble;
class SpinBoxInt;
class TextCompass;




// contents of this header -----------------------------------------------------------------------------------
class OptionWidget; // abstract base class
class OptionWidgetCheckBox; // bool
class OptionWidgetSpinBoxDouble;
class OptionWidgetSpinBoxInt; 
class OptionWidgetLineEdit; // QString
class OptionWidgetComboBox; // enum (!= int!)
class OptionWidgetRadioButtonBox; // enum
class OptionWidgetListBox; // enum
class OptionWidgetFontEdit; // QFont
class OptionWidgetColorEdit; // QColor
class OptionWidgetTextCompass; // double - for label rotation
class OptionWidgetSliderInt; // QSlider + QSpinBox





// base class -----------------------------------------------------------------------------------
class OptionWidget : public QObject
{
	Q_OBJECT

public:
	OptionWidget(Option *owner);
	~OptionWidget();
	void initializeWidget();
	bool ownerTypeIsValid() const;
	void setValue(const QVariant &);
	QVariant value() const; 
	bool verifyManufacturedWidget() const;
	QWidget* widget();
	
protected:
	Option* owner();
	Option* owner() const;

	// pure virtual functions that will make the COMPILER enforce new widget requirements as much as possible
	virtual void initializeWidget_Subclass() = 0;
	virtual bool ownerTypeIsValid_Subclass() const = 0;
	virtual void setValue_Subclass(const QVariant &) = 0;
	virtual QVariant value_Subclass() const = 0;
	virtual bool verifyManufacturedWidget_Subclass() const = 0;
	
	// protected data - so that derived classes can access directly (dangerous? meh)
	QPointer<QWidget> m_widget;
	//const Option::WidgetType m_type; meh

Q_SIGNALS:
	void widgetModified();
	
private:
	QPointer<Option> m_owner;	
};



// checkbox -----------------------------------------------------------------------------------
class OptionWidgetCheckBox : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetCheckBox(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<QCheckBox> m_checkBox;
};



// spin box (double) -----------------------------------------------------------------------------------
class OptionWidgetSpinBoxDouble : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetSpinBoxDouble(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<SpinBoxDouble> m_spinBox;
};



// spin box (int) -----------------------------------------------------------------------------------
class OptionWidgetSpinBoxInt : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetSpinBoxInt(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<SpinBoxInt> m_spinBox;
};



// line editor -----------------------------------------------------------------------------------
class OptionWidgetLineEdit : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetLineEdit(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<QLineEdit> m_lineEdit;
};



// combo box (enum) -----------------------------------------------------------------------------------
class OptionWidgetComboBox : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetComboBox(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<QComboBox> m_comboBox;
	static int const M_MAX_VISIBLE_ITEMS;
};



// radio button box (enum) ----------------------------------------------------------------------------
class OptionWidgetRadioButtonBox : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetRadioButtonBox(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<RadioButtonBox> m_radioBox;
};



// list box (enum) -----------------------------------------------------------------------------------
class OptionWidgetListBox : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetListBox(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<QListWidget> m_listWidget;
};



// QFont editor ----------------------------------------------------------------------------
class OptionWidgetFontEdit : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetFontEdit(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<FontEdit> m_fontEdit;
};



// QColor editor ----------------------------------------------------------------------------
class OptionWidgetColorEdit : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetColorEdit(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<ColorEdit> m_colorEdit;
};



// TextCompass (for label rotation angle) -------------------------------------------------------------
class OptionWidgetTextCompass : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetTextCompass(Option *owner);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<TextCompass> m_compass;
};



// QSlider + QSpinBox -------------------------------------------------------------
class OptionWidgetSliderInt : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetSliderInt(Option *owner, IntSlider::Type, QBoxLayout::Direction = QBoxLayout::LeftToRight);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<IntSlider> m_slider;
};



// QwtSlider + QDoubleSpinBox -------------------------------------------------------------
class OptionWidgetSliderDouble : public OptionWidget {
	Q_OBJECT
public:
	OptionWidgetSliderDouble(Option *owner, DoubleSlider::Type, QBoxLayout::Direction = QBoxLayout::LeftToRight);
protected:
	virtual void initializeWidget_Subclass();
	virtual bool ownerTypeIsValid_Subclass() const;
	virtual void setValue_Subclass(const QVariant &);
	virtual QVariant value_Subclass() const;
	virtual bool verifyManufacturedWidget_Subclass() const;
private:
	QPointer<DoubleSlider> m_slider;
};
