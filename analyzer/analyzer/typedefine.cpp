
#include "stdafx.h"
#include <iostream>     // std::cout
//#include <algorithm>    // std::find
#include <vector>       // std::vector
#include <fstream>

#include "typedefine.h"

void mreadini(wchar_t *fn, anp &p1, cvp &p2, std::vector<sap> &p3 )
{

	std::ifstream ifs(fn);
	double a;
	sap s1;
	p3.clear();

	if(!ifs.bad()){
		
		ifs>>p1.analysistype
			>>p1.calibrationfactor
			>>p1.endpointratio
			>>p1.evaluationratio;

		ifs>>p2.endintegratione
			>>p2.highelimit
			>>p2.lowelimit
			>>p2.noofcycles
			>>p2.rotationrate
			>>p2.scanrate;

		while(ifs>>s1.Aconc>>s1.Lconc>>s1.Sconc>>s1.volume){
			p3.push_back(s1);
		}

		ifs.close();

		
	}
}


void mwriteini(wchar_t *fn, const anp &p1, const cvp &p2, const std::vector<sap> &p3 )
{
	std::ofstream ofs(fn);

	if(!ofs.bad()){

		ofs<<p1.analysistype<<'\n'
			<<p1.calibrationfactor<<'\n'
			<<p1.endpointratio<<'\n'
			<<p1.evaluationratio<<'\n';

		ofs<<p2.endintegratione<<'\n'
			<<p2.highelimit<<'\n'
			<<p2.lowelimit<<'\n'
			<<p2.noofcycles<<'\n'
			<<p2.rotationrate<<'\n'
			<<p2.scanrate<<'\n';

		for(size_t i=0;i<p3.size();i++){
			ofs<<p3[i].Aconc<<'\n'
				<<p3[i].Lconc<<'\n'
				<<p3[i].Sconc<<'\n'
				<<p3[i].volume<<'\n';
		}

		ofs.close();
	}
}