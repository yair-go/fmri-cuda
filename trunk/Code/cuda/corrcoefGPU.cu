#include "cuda.h"

#define BLOCK_SIZE 128

//*****************************************************************/
//function name:	corrcoefGPU
//arguments:		v[] (in) 2-D matrix, size * length				
//					size (in) - num of vectors 
//					length (in) - length of the vectors
//return value :	result of corrCoef - everyone by everyone
//*****************************************************************/
//kernel corrcoef function

void corrcoefGPU(double v[], int size, int length)
{
	double* dev_vin,dev_vout;
	cudaMalloc ((void**)&dev_vin,size*length);
	cudaMalloc ((void**)&dev_vout,size*size);
	
	cudaMemcpy (dev_vin,v,size*length,cudaMemcpyHostToDevice);
	for (int i=0;i<size;i++){
		
	}
}