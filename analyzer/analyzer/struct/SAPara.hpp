#pragma once

#include "sapitemA.hpp"
#include <vector>
// SAPara command target

class SAPara : public CObject
{
public:
	// VMS Volume
	//double vmsvol;
	std::vector<sapitemA> saplist;


	SAPara(){};
	virtual ~SAPara(){};
	void operator=(const SAPara &src){
		//vmsvol=src.vmsvol;
		saplist.assign(src.saplist.begin(),src.saplist.end());
	};
	SAPara(const SAPara &src){ operator=(src); };
	

	virtual void Serialize(CArchive& ar){
		if (ar.IsStoring())
		{	// storing code
			//ar<<vmsvol
			ar<<saplist.size();

			for(size_t i=0;i<saplist.size();i++){
				saplist[i].Serialize(ar);
			}
		}
		else
		{	// loading code
			size_t nrow;
			//ar>>vmsvol
			ar>>nrow;

			saplist.assign(nrow,sapitemA());

			for(size_t i=0;i<nrow;i++){
				saplist[i].Serialize(ar);
			}
		}
	};

};


