#include <afxcmn.h>
#include <vector>
#include "DataOutA.h"
#include "ANPara.h"
#include "VPara.h"
#include "SAPara.h"
#include "RawData.h"

#define SC_STEP_COMPLETE 0x01
#define SC_NEW_RIGHT_PLOT 0x02
#define SC_NO_PLOT 0x04
#define SC_PLOT_LAST 0x08
#define SC_NEW_LINE 0x10
#define SC_NEW_ONCE 0x20


#define PF_Q_NORMALIZED 0x04
#define PF_CONCERTRATION 0x08
#define PF_S 0x10
#define PF_A 0x20
#define PF_L 0x40
#define PF_SAMPLE 0x80

void AdjustWidth(CListCtrl *ls, int nCol, CString str, int gap=15);
void AdjustWidth(CListCtrl *ls, int nCol, int nRow, int gap=15);



template <typename T>
int Seperate(const std::vector<T> &x, std::vector<size_t> &mini, std::vector<size_t> &maxi)
{
	if(x.size()<2)
		return 1;


	for(size_t i=1;i<x.size()-1;i++){
		if(x[i]>x[i-1]&&x[i]>x[i+1])
			maxi.push_back(i);
		if(x[i]<x[i-1]&&x[i]<x[i+1])
			mini.push_back(i);
	}

	if(mini.empty()){
		if(maxi.empty()){
			if(x.front()==x.back()){
				return 2;
			}
			else{
				if(x.front()>x.back()){
					mini.push_back(x.size()-1);
					maxi.push_back(0);
				}
				else{
					maxi.push_back(x.size()-1);
					mini.push_back(0);
				}				
			}
		}
		else{
			mini.insert(mini.begin(),0);
			mini.push_back(x.size()-1);
		}
	}
	else{
		if(maxi.empty()){
			maxi.insert(maxi.begin(),0);
			maxi.push_back(x.size()-1);
		}
		else{

			if(mini.front()>maxi.front()){
				mini.insert(mini.begin(),0);
			}
			else{
				maxi.insert(maxi.begin(),0);
			}

			if(mini.back()>maxi.back()){
				maxi.push_back(x.size()-1);
			}
			else{
				mini.push_back(x.size()-1);
			}
		}
	}

	return 0;

}






template <typename T>
int ComputeQList(const std::vector<T> &u, const std::vector<T> &i, T* QList, size_t nCycle, T upLimit, T scanRate, size_t nPperCycle=4)
{
	if(u.size()!=i.size()
		||nCycle==0
		||u.size()<nCycle*nPperCycle){
			return 1;
	}

	std::vector<size_t> mini;
	std::vector<size_t> maxi;

	int re=Seperate(u,mini,maxi);

	if(re!=0){
		return 2;
	}
	
	std::vector<T> intgi(i.size(),0);

	if(mini.front()>maxi.front())
		maxi.erase(maxi.begin());

	size_t ci=0;

	while(ci<nCycle){

		if(mini.empty() || maxi.empty())
			return 3;
	
		if(u[mini.front()]>=upLimit)
			continue;

		QList[ci]=0;
		bool bStart=false;
		for(size_t j=maxi.front();j>mini.front();j--){			
			if(bStart){
				QList[ci]+=i[j]*(u[j]-u[j-1]);
				if(i[j-1]<0)
					break;
			}
			else{
				bStart|=(u[j]<=upLimit);
			}
		}
		QList[ci]/=scanRate;

		ci++;
		mini.erase(mini.begin());
		maxi.erase(maxi.begin());

	}

	return 0;
}