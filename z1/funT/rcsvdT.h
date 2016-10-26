
//read data from csv file
template <typename T>
int rcsvd(FILE *pFile, T *t, T *x, int n){
	int i;
	if (pFile!=NULL)
	{
		for (i=1;i<=n;i++){
			fscanf (pFile, "%f,%f,,,", t+i,x+i);
			//printf("%f,%f\n",t[i],x[i]);
		}
		return 1;
	}
	else
		return 0;
}
