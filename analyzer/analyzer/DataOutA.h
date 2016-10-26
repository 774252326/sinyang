#pragma once

#include <vector>
#include "sapitemA.h"
// DataOutA command target


//one filter BYTE 
#define DOA_VMS 0x01
#define DOA_S 0x02
#define DOA_A 0x04
#define DOA_L 0x08
#define DOA_MORE 0x10
#define DOA_SAMPLE 0x20
//#define DOA_RECOUNT 0x40
#define DOA_RESET_SOLUTION_AT_END 0x80


class DataOutA : public CObject
{
public:
	DataOutA();
	virtual ~DataOutA();
	void operator=(const DataOutA &src);

	DataOutA(const DataOutA &src){ operator=(src); };

	std::vector<double> Ar;
	double Ar0;
	double Sml;
	double Aml;
	double Lml;
	bool bUnknown;
	double VMSVolume;
	double additiveVolume;
	double addVolume;
	CString stepName;
	BYTE stepFilter;
	int UseIndex;
	//////////////////////////////////

	double TotalVolume(void) const;
	double AConc(void) const;
	double SConc(void) const;
	double LConc(void) const;

	bool Update(sapitemA sapi, BYTE bFlag=DOA_VMS);


	bool VolOnce(sapitemA sapi);
	bool ConcOnce(double &ml, double endConc, double addConc);
	bool ConcIntv(double &ml, double intvConc, double addConc);
	CString GetStepName(int i=-1) const;
	void ResetCompound(void);

	bool Update(sapitemA sapi, BYTE bFlag, const std::vector<double> &ArList);

	virtual void Serialize(CArchive& ar);

	double ArUse(void) const;
};


