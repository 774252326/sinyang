#ifndef PIKSR2T_H
#define PIKSR2T_H

template <typename T, typename TT>
void piksr2(int n, T arr[], TT brr[])
	//Sorts an array arr[1..n] into ascending numerical order, by straight insertion, while making
	//the corresponding rearrangement of the array brr[1..n].
{
	int i,j;
	T a;
	TT b;
	for (j=2;j<=n;j++) { /*Pick out each element in turn.*/
		a=arr[j];
		b=brr[j];
		i=j-1;
		while (i > 0 && arr[i] > a) { /*Look for the place to insert it.*/
			arr[i+1]=arr[i];
			brr[i+1]=brr[i];
			i--;
		}
		arr[i+1]=a; /*Insert it.*/
		brr[i+1]=b;
	}
}

#endif