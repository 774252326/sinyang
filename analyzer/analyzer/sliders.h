// sliders.h
// QSlider + QSpinBox | QwtSlider + QDoubleSpinBox | QScrollBar + QSpinbox
// sliders alone aren't enough - like TextCompass, pair with spinboxes for display and precise control.

#if ((defined _MSC_VER) && (_MSC_VER > 1000)) || ((defined __GNUC__) && (__GNUC__ > 3))
#	pragma once
#else
#	error "Custom header guard needed - have fun editing all N files in this project...just HOW portable is pragma once??"
#endif

#include "standard_definitions_head.h"

#include <QBoxLayout> // for stupid Direction enum
#include <QPointer>
#include <QWidget>
class QAbstractSlider;
//class QBoxLayout;
class QDoubleSpinBox;
class QScrollBar;
class QSpinBox;
class QwtAbstractSlider;


// QSlider + QSpinBox composite ----------------------------------------------
class IntSlider : public QWidget
{
	Q_OBJECT
public:
	enum Type 
	{	
		Type_Slider, 
		Type_ScrollBar, 
		Type_Dial 
	};

	// oriented direction: Slider to SpinBox
	IntSlider(Type, QWidget *parent = 0, QBoxLayout::Direction = QBoxLayout::LeftToRight, bool validate = false);
	int maximum() const;
	void setMaximum(int);
	int minimum() const;
	void setMinimum(int);
	int singleStep() const;
	void setSingleStep(int);
	int value() const;
	void setValue(int);

Q_SIGNALS:
	void valueChanged(int);

private:
	bool checkPointers() const;

	QPointer<QBoxLayout> m_layout;
	QPointer<QAbstractSlider> m_slider;
	QPointer<QSpinBox> m_spinBox;
};



// QwtSlider + QDoubleSpinBox composite - with option to go to my SpinBoxDouble... 
class DoubleSlider : public QWidget
{
	Q_OBJECT
public:
	enum Type 
	{ 
		Type_Slider, 
		Type_Knob, 
		Type_Dial, // adjustable speedometer
		Type_Wheel // stupid flywheel that REQUIRES min/max - but included for completion (subclasses of QwtAbstractSlider)
		
		// also eligible but not implemented - they're too specialized/temperamental, i think
		// QwtAnalogClock
		// QwtCompass
	};
	DoubleSlider(Type, QWidget *parent = 0, QBoxLayout::Direction = QBoxLayout::LeftToRight, bool validate = false);
	double maximum() const;
	void setMaximum(double);
	double minimum() const;
	void setMinimum(double);
	double singleStep() const;
	void setSingleStep(double);
	double value() const;
	void setValue(double);
	int decimals() const;
	void setDecimals(int);

Q_SIGNALS:
	void valueChanged(double);

private:
	bool checkPointers() const;

	QPointer<QBoxLayout> m_layout;
	QPointer<QwtAbstractSlider> m_slider;
	QPointer<QDoubleSpinBox> m_spinBox;
};


