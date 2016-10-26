#pragma once

#include <vector>

template <typename T>
UINT Seperate(
	const std::vector<T> &x,
	std::vector<size_t> &mini,
	std::vector<size_t> &maxi)
{
	if(x.size()<2)
		return 1;//���й���

	//�Ҽ���С
	for(size_t i=1;i<x.size()-1;i++){
		if(x[i]>x[i-1]&&x[i]>x[i+1])
			maxi.push_back(i);
		if(x[i]<x[i-1]&&x[i]<x[i+1])
			mini.push_back(i);
	}

	if(mini.empty()){
		if(maxi.empty()){
			if(x.front()==x.back()){
				return 2;//����Ϊ����
			}
			else{
				if(x.front()>x.back()){//���е���
					mini.push_back(x.size()-1);
					maxi.push_back(0);
				}
				else{//���е���
					maxi.push_back(x.size()-1);
					mini.push_back(0);
				}				
			}
		}
		else{//������Ψһ����ֵ
			mini.insert(mini.begin(),0);
			mini.push_back(x.size()-1);
		}
	}
	else{
		if(maxi.empty()){//������Ψһ��Сֵ
			maxi.insert(maxi.begin(),0);
			maxi.push_back(x.size()-1);
		}
		else{//��ȫͷβ��

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