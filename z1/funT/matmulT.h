#ifndef MATMULT_H
#define MATMULT_H

template <typename T>
bool matmul(T **mat1, long rs1, long re1, long cs1, long ce1, 
	T **mat2, long rs2, long re2, long cs2, long ce2, 
	T **mat3){
		//matrix multiply
			//mat3=mat1 x mat2
		if(ce1-cs1!=re2-rs2)
			return false;
		else{
			long r3,c3,t3;

			T sum;
			for( r3=rs1; r3<=re1; r3++ ){
				for( c3=cs2; c3<=ce2; c3++ ){
					sum=0;
					for( t3=cs1; t3<=ce1; t3++ ){
						sum+=mat1[r3][t3]*mat2[t3-cs1+rs2][c3];
					}
					mat3[r3][c3]=sum;
				}
			}
			return true;
		}
}



template <typename T>
void mattp(T **mat1, long rs1, long re1, long cs1, long ce1, T **mat2){
//matrix transpose
	long r3,c3;

	for( r3=cs1; r3<=ce1; r3++ ){
		for( c3=rs1; c3<=re1; c3++ ){
			mat2[r3][c3]=mat1[c3][r3];
		}
	}

}


template <typename T>
bool mtXvt(T **mat1, long rs1, long re1, long cs1, long ce1, 
	T *vt2, long rs2, long re2, T *vt3){
//matrix multiply column vector
			//vt3=mat1 x vt2
		if(ce1-cs1!=re2-rs2)
			return false;
		else{
			long r3,/*c3,*/t3;

			T sum;
			for( r3=rs1; r3<=re1; r3++ ){
				//for( c3=cs2; c3<=ce2; c3++ ){
					sum=0;
					for( t3=cs1; t3<=ce1; t3++ ){
						//sum+=mat1[r3][t3]*mat2[t3-cs1+rs2][c3];
						sum+=mat1[r3][t3]*vt2[t3-cs1+rs2];
					}
					vt3[r3]=sum;
				//}
			}
			return true;
		}
}






#endif