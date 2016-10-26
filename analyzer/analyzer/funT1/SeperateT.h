#pragma once

#include <vector>

template <typename T>
UINT Seperate(
	const std::vector<T> &x,
	std::vector<size_t> &mini,
	std::vector<size_t> &maxi)
{
	if(x.size()<2)
		return 1;//序列过短

	//找极大极小
	for(size_t i=1;i<x.size()-1;i++){
		if(x[i]>x[i-1]&&x[i]>x[i+1])
			maxi.push_back(i);
		if(x[i]<x[i-1]&&x[i]<x[i+1])
			mini.push_back(i);
	}

	if(mini.empty()){
		if(maxi.empty()){
			if(x.front()==x.back()){
				return 2;//序列为常数
			}
			else{
				if(x.front()>x.back()){//序列单减
					mini.push_back(x.size()-1);
					maxi.push_back(0);
				}
				else{//序列单增
					maxi.push_back(x.size()-1);
					mini.push_back(0);
				}				
			}
		}
		else{//序列有唯一极大值
			mini.insert(mini.begin(),0);
			mini.push_back(x.size()-1);
		}
	}
	else{
		if(maxi.empty()){//序列有唯一极小值
			maxi.insert(maxi.begin(),0);
			maxi.push_back(x.size()-1);
		}
		else{//补全头尾点

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