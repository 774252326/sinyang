#pragma once

// sapitem command target

class sapitem : public CObject
{
public:
	sapitem();
	virtual ~sapitem();
	sapitem(const sapitem &src);
	void operator=(const sapitem &src);

	double Sconc;
	double Aconc;
	double Lconc;
	int addType;
	double volconc;
	virtual void Serialize(CArchive& ar);
};


