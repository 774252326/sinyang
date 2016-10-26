#ifndef PIKSRTT_H
#define PIKSRTT_H

template <typename T>
void piksrt(int n, T arr[])
	//Sorts an array arr[1..n] into ascending numerical order, by straight insertion. n is input; arr
	//is replaced on output by its sorted rearrangement.
{
	int i,j;
	T a;
	for (j=2;j<=n;j++) { /*Pick out each element in turn.*/
		a=arr[j];
		i=j-1;
		while (i > 0 && arr[i] > a) { /*Look for the place to insert it.*/
			arr[i+1]=arr[i];
			i--;
		}
		arr[i+1]=a; /*Insert it.*/
	}
}

#endif