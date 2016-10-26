#include "stdafx.h"

bool matmul(float **mat1, long rs1, long re1, long cs1, long ce1, 
	float **mat2, long rs2, long re2, long cs2, long ce2, 
	float **mat3){
		if(ce1-cs1!=re2-rs2)
			return false;
		else{
			long r3,c3,t3;

			float sum;
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

void mattp(float **mat1, long rs1, long re1, long cs1, long ce1, float **mat2){

	long r3,c3,t3;

	for( r3=cs1; r3<=ce1; r3++ ){
		for( c3=rs1; c3<=re1; c3++ ){
			mat2[r3][c3]=mat1[c3][r3];
		}
	}

}

