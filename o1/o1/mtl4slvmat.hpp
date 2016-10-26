//#include <iostream>
#include <boost/numeric/mtl/mtl.hpp>
#include <boost/numeric/itl/itl.hpp>
//#include <time.h>


#include "stdafx.h"
#include "../../funT/nrutilT.h"

template <typename T>
void mtl4slvmat(T **am, long n, T *bv, T *xv){

	using namespace mtl;
	using namespace itl;


	long i,j;
	typedef compressed2D<T>  matrix_type;



	dense_vector<T>          x(n, 1.0), b(n);
	for(i=0;i<n;i++){
		b[i]=bv[i+1];
	}

	matrix_type                   A(n, n);
	{
		matrix::inserter<compressed2D<T> > ins(A);  


		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				ins[i][j] << am[i+1][j+1];
				
			}
			
		}
		printf("%d,%d\n",i,j);
		//ins[0][0] << 2.0;
		//ins[1][0] << 0.5;
		//ins[0][1] << 3.0;
		//ins[1][1] << 7.0;
	} // ins is destroyed here



	//b= A * x; x= 0;


	clock_t t = clock();

	// Create an ILU(0) preconditioner
	pc::ilu_0<matrix_type>        P(A);

	// Termination criterion: r < 1e-6 * b or N iterations
	noisy_iteration<T>       iter(b, 500, 1.e-6);

	// Solve Ax == b with left preconditioner P
	bicgstab(A, x, b, P, iter);

	//std::cout<< "A=\n" << A <<'\n';
	//std::cout<< "b=\n" << b <<'\n';
	//std::cout<< "x=\n" << x <<'\n';
	//std::cout<< n << 'x' << n << " Matrix solved! Elapsed time " << (float)(clock() - t)/CLOCKS_PER_SEC << " seconds.\n" ;
	printf( "%dx%d Matrix solved! Elapsed time %.3f seconds.\n", n, n, (float)(clock() - t)/CLOCKS_PER_SEC );
	for(i=0;i<n;i++){
		xv[i+1]=x[i];
	}
}


//int main(int, char**)
//{
//	//clock_t t = clock();
//
//	//   //const int size = 40, N = size * size; 
//	//const int N = 2;
//	//   typedef compressed2D<double>  matrix_type;
//
//	//   // Set up a matrix 1,600 x 1,600 with 5-point-stencil
//	//   matrix_type                   A(N, N);
//	//   //matrix::laplacian_setup(A, size, size);
//
//	//   {
//	//       matrix::inserter<compressed2D<double> > ins(A);  
//	//       ins[0][0] << 2.0;
//	//       ins[1][0] << 0.5;
//	//       ins[0][1] << 3.0;
//	//	ins[1][1] << 7.0;
//	//   } // ins is destroyed here
//
//
//
//	//   // Set b such that x == 1 is solution; start with x == 0
//	//   dense_vector<double>          x(N, 1.0), b(N);
//	//   b= A * x; x= 0;
//
//	//   // Create an ILU(0) preconditioner
//	//   pc::ilu_0<matrix_type>        P(A);
//	//   
//	//   // Termination criterion: r < 1e-6 * b or N iterations
//	//   noisy_iteration<double>       iter(b, 500, 1.e-6);
//	//   
//	//   // Solve Ax == b with left preconditioner P
//	//   bicgstab(A, x, b, P, iter);
//
//	//std::cout<< "A=\n" << A <<'\n';
//	//std::cout<< "b=\n" << b <<'\n';
//	//std::cout<< "x=\n" << x <<'\n';
//	//std::cout<< N << 'x' << N << " Matrix solved! Elapsed time " << (float)(clock() - t)/CLOCKS_PER_SEC << " seconds.\n" ;
//
//
//
//
//	double **a=matrix<double>(1,2,1,2);
//	double *b=vector<double>(1,2);
//	double *x=vector<double>(1,2);
//
//	a[1][1]=2;
//	a[1][2]=4;
//	a[2][1]=5;
//	a[2][2]=11;
//	b[1]=3;
//	b[2]=7;
//
//	mtl4slvmat(a,2,b,x);
//
//
//
//
//	return 0;
//}