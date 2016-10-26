#pragma once
#include "../ObjectF.hpp"
// Value command target

///
/// \brief The Value class
///带修正因子的参数

class Value : public ObjectF
{
public:
    ///
    /// \brief LogoId
    ///参数标记的位图ID
	int LogoId;
    ///
    /// \brief raw
    ///参数原始值
	double raw;
    ///
    /// \brief correction
    ///修正因子
	double correction;
public:
	Value()	
		: LogoId(IDB_BITMAP13)
		, raw(0)
		, correction(1)
	{
	};
	virtual ~Value(){};
	virtual void Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{	// storing code
			ar<<LogoId
				<<raw
				<<correction;
		}
		else
		{	// loading code
			ar>>LogoId
				>>raw
				>>correction;
		}
	};

	void operator=(const Value &src)
	{
		LogoId=src.LogoId;
		raw=src.raw;
		correction=src.correction;
	};

	Value(const Value &src){ operator=(src);};

    ///
    /// \brief Output
    /// \return
    ///参数输出值
	double Output(void) const{return raw*correction;};

};
