#pragma once

// sapitem command target

class sapitem : public CObject
{
public:
	sapitem();
	virtual ~sapitem();

	void operator=(const sapitem &src);
	sapitem(const sapitem &src){ operator=(src); };

	double Sconc;
	double Aconc;
	double Lconc;
	int addType;
	double volconc;
	virtual void Serialize(CArchive& ar);
};


