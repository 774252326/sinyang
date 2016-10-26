#pragma once

// Value command target

class Value : public CObject
{
public:
	Value();
	virtual ~Value();
	virtual void Serialize(CArchive& ar);


public:
	int LogoId;
	double raw;
	double correction;

	void operator=(const Value &src)
{
	LogoId=src.LogoId;
	raw=src.raw;
	correction=src.correction;
};

	Value(const Value &src){ operator=(src);};


	double Output(void);

};
