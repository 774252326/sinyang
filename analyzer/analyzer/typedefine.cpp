
#include "stdafx.h"
#include <iostream>     // std::cout
//#include <algorithm>    // std::find
#include <vector>       // std::vector
#include <fstream>

#include "typedefine.h"

void mreadini(wchar_t *fn, anp &p1, cvp &p2, sap &p3 )
{

	std::ifstream ifs(fn);
	//double a;
	sapitem s1;
	p3.saplist.clear();

	if(!ifs.bad()){
		
		ifs>>p1.analysistype
			>>p1.calibrationfactortype
			>>p1.calibrationfactor
			>>p1.endpointratio
			>>p1.evaluationratio;

		ifs>>p2.endintegratione
			>>p2.highelimit
			>>p2.lowelimit
			>>p2.combochoice
			>>p2.variationtolerance
			>>p2.noofcycles
			>>p2.rotationrate
			>>p2.scanrate;

		ifs>>p3.vmsvol;

		while(ifs>>s1.Aconc>>s1.Lconc>>s1.Sconc>>s1.addtype>>s1.volconc){
			p3.saplist.push_back(s1);
		}

		ifs.close();

		
	}
}


void mwriteini(wchar_t *fn, const anp &p1, const cvp &p2, const sap &p3 )
{
	std::ofstream ofs(fn);

	if(!ofs.bad()){

		ofs<<p1.analysistype<<'\n'
			<<p1.calibrationfactortype<<'\n'
			<<p1.calibrationfactor<<'\n'
			<<p1.endpointratio<<'\n'
			<<p1.evaluationratio<<'\n';

		ofs<<p2.endintegratione<<'\n'
			<<p2.highelimit<<'\n'
			<<p2.lowelimit<<'\n'
			<<p2.combochoice<<'\n'
			<<p2.variationtolerance<<'\n'
			<<p2.noofcycles<<'\n'
			<<p2.rotationrate<<'\n'
			<<p2.scanrate<<'\n';

		ofs<<p3.vmsvol<<'\n';

		for(size_t i=0;i<p3.saplist.size();i++){
			ofs<<p3.saplist[i].Aconc<<'\n'
				<<p3.saplist[i].Lconc<<'\n'
				<<p3.saplist[i].Sconc<<'\n'
				<<p3.saplist[i].addtype<<'\n'
				<<p3.saplist[i].volconc<<'\n';
		}

		ofs.close();
	}
}


void copyfs(const figspec &fsi, figspec & fso)
{
	fso.bkgndC=fsi.bkgndC;
	fso.borderC=fsi.borderC;
	fso.gridC=fsi.gridC;
	fso.gridType=fsi.gridType;
	fso.labelC=fsi.labelC;
	fso.labelSize=fsi.labelSize;
	fso.metricC=fsi.metricC;
	fso.metricSize=fsi.metricSize;
}