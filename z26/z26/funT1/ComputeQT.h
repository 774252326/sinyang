#pragma once

#include "IntegralT.h"
#include "SeperateT.h"
#include <functional>




template <typename T>
bool RDPOnce(std::vector<T> x, std::vector<T> y, size_t &newnode, T &dist)
{

	if(	x.size()!=y.size() || x.size()<3 ){
		return false;
	}

	T xf=x.front();
	T xb=x.back();
	T yf=y.front();
	T yb=y.back();

	T D=xf*yb-xb*yf;
	T A=yf-yb;
	T B=xb-xf;

	std::vector<T> d(x.size(),0);
	for(size_t i=0;i<d.size();i++){
		d[i]=A*x[i]+B*y[i]+D;
		d[i]=abs(d[i]);
	}
	std::vector<T>::iterator it;
	it=std::max_element(d.begin(),d.end());
	newnode=it-d.begin();

	dist=d[newnode]/sqrt(A*A+B*B);

	return true;

}

template <typename T>
bool findUpLim(
	const std::vector<T> &u, 
	const std::vector<T> &i,
	size_t startidx,
	size_t endidx,
	size_t &UpLimIdx
	){
		std::vector<T> x;
		x.assign(u.begin()+startidx,u.begin()+endidx+1);
		std::vector<T> y;
		y.assign(i.begin()+startidx,i.begin()+endidx+1);
		T dist=0;
		size_t idx=0;
		bool flg=RDPOnce(x,y,idx,dist);

		if(flg){

			TRACE(L"\nu=%g,i=%g",x[idx],y[idx]);

			if(y[idx]<=0){
				UpLimIdx=idx+startidx;				
			}
			else{
			x.erase(x.begin(),x.begin()+idx);
			y.erase(y.begin(),y.begin()+idx);
			startidx+=idx;

			flg=RDPOnce(x,y,idx,dist);

			UpLimIdx=flg? idx+startidx : startidx;
			}

			TRACE(L";u=%g,i=%g",u[UpLimIdx],i[UpLimIdx]);

			return true;
		}

		return false;

}





template <typename T>
UINT ComputeQListC(
	std::vector<T> u, 
	std::vector<T> i,
	std::vector<T> &QList, 
	T scanRate,
	T umin,
	T umax
	){

		if(u.size()!=i.size()
			//||nCycle==0
			////||u.size()<nCycle*nPperCycle
			){
				return 3;//�������
		}

		std::vector<size_t> mini;
		std::vector<size_t> maxi;

		UINT re=Seperate(u,mini,maxi);

		if(re!=0){
			return re;//�ֶγ���
		}

		if(mini.front()>maxi.front())
			maxi.erase(maxi.begin());

		QList.clear();

		while( !mini.empty() && !maxi.empty() ){

			if(u[mini.front()]<=umin
				&& u[maxi.front()]>=umax
				){//�ֶ��������Ԥ��������Χ[umin,umax]
					std::vector<T>::iterator it;					
					it=std::max_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);
					if(it!=i.begin()+maxi.front()+1){
						size_t maxidx=it-i.begin();

						size_t uplimidx=0;

						if(findUpLim(u,i,maxidx,maxi.front(),uplimidx)){
							maxi.front()=uplimidx;
							T ilowlim=0;
							it=std::find_end(
								i.begin()+mini.front(),								
								i.begin()+maxidx+1, 									
								&ilowlim, 
								(&ilowlim)+1, 
								std::less_equal<T>());


							if(it!=i.begin()+maxidx+1){//�������޳����ֶ�����
								mini.front()=it-i.begin();


								if(mini.front()<maxi.front()){//��������<��������
									std::vector<T> x(u.begin()+mini.front(),u.begin()+maxi.front()+1);
									std::vector<T> y(i.begin()+mini.front(),i.begin()+maxi.front()+1);

									T res=Integral(x,y)/scanRate;

									QList.push_back(res);
								}
							}
						}
					}
			}

			mini.erase(mini.begin());
			maxi.erase(maxi.begin());

		}

		return 0;

}





template <typename T>
UINT ComputeQListB(
	std::vector<T> u, 
	std::vector<T> i,
	std::vector<T> &QList, 
	T lowLimit,
	T scanRate,
	T umin,
	T umax
	){

		if(u.size()!=i.size()
			//||nCycle==0
			////||u.size()<nCycle*nPperCycle
			){
				return 3;//�������
		}

		std::vector<size_t> mini;
		std::vector<size_t> maxi;

		UINT re=Seperate(u,mini,maxi);

		if(re!=0){
			return re;//�ֶγ���
		}

		if(mini.front()>maxi.front())
			maxi.erase(maxi.begin());

		QList.clear();

		while( !mini.empty() && !maxi.empty() ){

			if(u[mini.front()]<=umin
				&& u[maxi.front()]>=umax
				){//�ֶ��������Ԥ��������Χ[umin,umax]
					std::vector<T>::iterator it;
					it=std::find_first_of(
						u.begin()+mini.front(), 
						u.begin()+maxi.front()+1, 
						&lowLimit, 
						(&lowLimit)+1, 
						std::greater_equal<T>());

					if(it!=u.begin()+maxi.front()+1){//��������δ�����ֶ�����
						mini.front()=it-u.begin();
						it=std::max_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);

						if(it!=i.begin()+maxi.front()+1){
							size_t maxidx=it-i.begin();

							size_t uplimidx=0;

							if(findUpLim(u,i,maxidx,maxi.front(),uplimidx)){
								maxi.front()=uplimidx;
								
								if(mini.front()<maxi.front()){//��������<��������
									std::vector<T> x(u.begin()+mini.front(),u.begin()+maxi.front()+1);
									std::vector<T> y(i.begin()+mini.front(),i.begin()+maxi.front()+1);

									T res=Integral(x,y)/scanRate;

									QList.push_back(res);
								}
							}
						}
					}
			}

			mini.erase(mini.begin());
			maxi.erase(maxi.begin());

		}

		return 0;

}



template <typename T>
UINT ComputeQList(
	std::vector<T> u, 
	std::vector<T> i,
	std::vector<T> &QList, 
	T upLimit,
	T scanRate,
	T umin,
	T umax
	){

		if(u.size()!=i.size()
			//||nCycle==0
			////||u.size()<nCycle*nPperCycle
			){
				return 3;//�������
		}

		std::vector<size_t> mini;
		std::vector<size_t> maxi;

		UINT re=Seperate(u,mini,maxi);

		if(re!=0){
			return re;//�ֶγ���
		}

		if(mini.front()>maxi.front())
			maxi.erase(maxi.begin());

		QList.clear();

		while( !mini.empty() && !maxi.empty() ){

			if(u[mini.front()]<=umin
				&& u[maxi.front()]>=umax
				){//�ֶ��������Ԥ��������Χ[umin,umax]
					std::vector<T>::iterator it;
					it=std::find_end(
						u.begin()+mini.front(), 
						u.begin()+maxi.front()+1, 
						&upLimit, 
						(&upLimit)+1, 
						std::less_equal<T>());

					if(it!=u.begin()+maxi.front()+1){//��������δ�����ֶ�����
						maxi.front()=it-u.begin();
						it=std::max_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);
						if(it!=i.begin()+maxi.front()+1){
							size_t maxidx=it-i.begin();
							T ilowlim=0;
							it=std::find_end(
								i.begin()+mini.front(),
								i.begin()+maxidx+1, 
								&ilowlim, 
								(&ilowlim)+1, 
								std::less_equal<T>());

							if(it!=i.begin()+maxidx+1){//�������޳����ֶ�����
								mini.front()=it-i.begin();
								if(mini.front()<maxi.front()){//��������<��������
									std::vector<T> x(u.begin()+mini.front(),u.begin()+maxi.front()+1);
									std::vector<T> y(i.begin()+mini.front(),i.begin()+maxi.front()+1);

									T res=Integral(x,y)/scanRate;

									QList.push_back(res);
								}
							}
						}
					}
			}

			mini.erase(mini.begin());
			maxi.erase(maxi.begin());

		}

		return 0;

}




template <typename T>
UINT ComputeQListA(
	std::vector<T> u, 
	std::vector<T> i,
	std::vector<T> &QList, 
	T upLimit,
	T lowLimit,
	T scanRate
	){

		if(u.size()!=i.size()
			//||nCycle==0
			////||u.size()<nCycle*nPperCycle
			){
				return 3;//�������
		}

		std::vector<size_t> mini;
		std::vector<size_t> maxi;

		UINT re=Seperate(u,mini,maxi);

		if(re!=0){
			return re;//�ֶγ���
		}

		if(mini.front()>maxi.front())
			maxi.erase(maxi.begin());

		QList.clear();

		while( !mini.empty() && !maxi.empty() ){

			//if(u[mini.front()]>umin
			//	|| u[maxi.front()]<umax
			//	){
			//	mini.erase(mini.begin());
			//	maxi.erase(maxi.begin());
			//	continue;//�ֶ�������̣����ܰ���Ԥ��������Χ[umin,umax]
			//}


			std::vector<T>::iterator it;
			it=std::find_end(
				u.begin()+mini.front(), 
				u.begin()+maxi.front()+1,
				&upLimit, 
				(&upLimit)+1,
				std::less_equal<T>());
			if(it!=u.begin()+maxi.front()+1){//��������δ�����ֶ�����
				maxi.front()=it-u.begin();
				it=std::find_end(
					u.begin()+mini.front(),
					u.begin()+maxi.front()+1, 
					&lowLimit, 
					(&lowLimit)+1, 
					std::less_equal<T>());
				if(it!=u.begin()+maxi.front()+1){//��������δ�����ֶ�����
					mini.front()=it-u.begin();
					if(mini.front()<maxi.front()){//��������<��������
						std::vector<T> x(u.begin()+mini.front(),u.begin()+maxi.front()+1);
						std::vector<T> y(i.begin()+mini.front(),i.begin()+maxi.front()+1);
						T res=Integral(x,y)/scanRate;
						QList.push_back(res);
					}
				}
			}
			mini.erase(mini.begin());
			maxi.erase(maxi.begin());

		}



		return 0;
}





template <typename T>
UINT ComputeQList(
	const std::vector<T> &u, 
	const std::vector<T> &i,
	T* QList, 
	size_t nCycle,
	T upLimit,
	T scanRate,
	T umin,
	T umax,
	size_t nPperCycle=4)
{

	size_t ci=0;

	if(u.size()!=i.size()
		||nCycle==0
		||u.size()<nCycle*nPperCycle){
			return ci;//�������
	}

	std::vector<size_t> mini;
	std::vector<size_t> maxi;

	int re=Seperate(u,mini,maxi);

	if(re!=0){
		return ci;//�ֶγ���
	}

	std::vector<T> intgi(i.size(),0);

	if(mini.front()>maxi.front())
		maxi.erase(maxi.begin());



	while(ci<nCycle){

		if(mini.empty() || maxi.empty())
			return ci;//���ݲ�����

		if(u[mini.front()]>=upLimit
			|| u[mini.front()]>umin
			|| u[maxi.front()]<umax
			|| u[maxi.front()]<=upLimit){
				mini.erase(mini.begin());
				maxi.erase(maxi.begin());
				continue;//�ֶ�������̣����ܰ���[umin,umax]����������޳����ֶ�����
		}

		//////////////////////////////////�һ�������/////////////////////////////////
		auto result = std::minmax_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);
		size_t maxidx=result.second-i.begin();

		if(*result.second<=0)
			continue;//����ֵ��Ϊ��

		size_t minidx=maxidx;
		for(;minidx>mini.front();minidx--){
			if(i[minidx]>0 && i[minidx-1]<=0)
				break;//�һ�������
		}

		if(u[minidx]>=upLimit)
			continue;//�������޴��ڻ�������
		///////////////////////////////////////////////
		QList[ci]=0;

		for(size_t j=minidx;j<maxi.front();j++){
			if(u[j]<upLimit){
				QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
			}
			else{
				break;
			}
		}

		//bool bStart=false;
		//for(size_t j=maxi.front();j>mini.front();j--){			
		//	if(bStart){
		//		QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
		//		//TRACE(L"\n%g,%g",u[j],i[j]);
		//		if(i[j-1]<0)
		//			break;
		//	}
		//	else{
		//		bStart|=(u[j]<=upLimit);
		//	}
		//}

		QList[ci]/=scanRate*2;

		ci++;
		mini.erase(mini.begin());
		maxi.erase(maxi.begin());

	}

	return ci;
}



template <typename T>
UINT ComputeQListA(
	const std::vector<T> &u, 
	const std::vector<T> &i,
	T* QList, 
	size_t nCycle,
	T upLimit,
	T lowLimit,
	T scanRate,
	//T umin,
	//T umax,
	size_t nPperCycle=4)
{

	size_t ci=0;

	if(u.size()!=i.size()
		||nCycle==0
		||u.size()<nCycle*nPperCycle){
			return ci;//�������
	}

	std::vector<size_t> mini;
	std::vector<size_t> maxi;

	int re=Seperate(u,mini,maxi);

	if(re!=0){
		return ci;//�ֶγ���
	}

	std::vector<T> intgi(i.size(),0);

	if(mini.front()>maxi.front())
		maxi.erase(maxi.begin());



	while(ci<nCycle){

		if(mini.empty() || maxi.empty())
			return ci;//���ݲ�����

		if(u[mini.front()]>=lowLimit
			//|| u[mini.front()]>umin
			//|| u[maxi.front()]<umax
			|| u[maxi.front()]<=upLimit){
				mini.erase(mini.begin());
				maxi.erase(maxi.begin());
				continue;//�ֶ�������̣����ܰ���[umin,umax]������������޳����ֶ�����
		}

		QList[ci]=0;

		for(size_t j=mini.front();j<maxi.front();j++){
			if(u[j]<upLimit && u[j]>lowLimit){
				QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
			}
			//else{
			//break;
			//}
		}

		QList[ci]/=scanRate*2;

		ci++;
		mini.erase(mini.begin());
		maxi.erase(maxi.begin());

	}

	return ci;
}
