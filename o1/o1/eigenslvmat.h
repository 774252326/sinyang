#ifndef EIGENSLVMAT_H
#define EIGENSLVMAT_H

//#include <iostream>
//#include <Eigen/Dense>
#include "../../Eigen/Dense"
//#include "Eigen/src/IterativeLinearSolvers/BiCGSTAB.h"
//#include "omp.h"

//using namespace std;

#include "../../funT/nrutilT.h"

#include <time.h>
#include <stdio.h>

template <typename T>
void eigenslvmat(T **a, long n, T *b, T *x){
	
	using namespace Eigen;
	//T *ad=&a[1][1];
	
	
	//int n = 1000;
	//MatrixXd A;
	//A = MatrixXd::Random(n,n);
	Map<MatrixXd> A(&a[1][1],n,n);
	//MatrixXd B;
	//B = MatrixXd::Random(n,1);
	Map<MatrixXd> B(&b[1],n,1);
	//std::cout << "Here is the invertible matrix A:\n" << A ;
	//std::cout << "\nHere is the matrix B:\n"  << B ;


	clock_t t = clock();
	//double time = omp_get_wtime();
	MatrixXd X = A.lu().solve(B);
	//std::cout << "\ntime elapsed " << omp_get_wtime()-time ;
	printf( "%dx%d Matrix solved! Elapsed time %.3f seconds.\n", n, n, (float)(clock() - t)/CLOCKS_PER_SEC );

	//MatrixXd X = A.partialPivLu().solve(B);
	//std::cout << "\nHere is the (unique) solution X to the equation AX=B:\n" << X;
	//std::cout << "\nRelative error: " << (A*X-B).norm() / B.norm();


	long i;
	for(i=1;i<=n;i++){
		x[i]=X(i-1,0);
	}


}



#endif