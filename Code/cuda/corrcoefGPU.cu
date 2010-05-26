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

__global__ void mean(double *dev_vec, duoble *dev_meanArray, int length)
{
	int block=blockIdx.x;
	int thread=threadIdx.x;
	int index = block*BLOCK_SIZE+thread; //voxel's num = line number
	double *pt=dev_vec+index*length;	//enter point to vec
	int i;
	double res=0;
	for (i=0;i<length;i++){
		res+=pt[i];
	}
	res/=legnth;
	dev_meanArray[index]=res;
	__syncthreads();
}

__global__ void corrCoefGPU(double *dev_vin, int i,double *dev_meanArray, double *dev_vout,int length)
{
	int block=blockIdx.x;
	int thread=threadIdx.x;
	int index = block*BLOCK_SIZE+thread;
	
	const double TINY=1.0e-20;
	
	double *current=dev_vin+i*length;	//enter point to vec, specific vector i
	double *pt=dev_vin+index*length;	//enter point to vec, for this one thread
	double syy=0.0,sxy=0.0,sxx=0.0;
	//TODO: parallel
	 for (int j=0;j<length;j++) 
	{     
        sxx += (current[j]-dev_meanArray[i])*(current[j]-dev_meanArray[i]);
        syy += (pt[j]-dev_meanArray[index])*(pt[j]-dev_meanArray[index]);
        sxy += (current[j]-dev_meanArray[i])*(pt[j]-dev_meanArray[index]);
    }

    dev_vout[index]=sxy/(sqrt(sxx*syy)+TINY);
}

void corrcoefGPU_kernel(double v[], int size, int length)
{
	double* dev_vin,dev_vout,dev_meanArray;
	cudaMalloc ((void**)&dev_vin,size*length);
	cudaMalloc ((void**)&dev_meanArray,size);
	cudaMalloc ((void**)&dev_vout,size*size);
	
	cudaMemcpy (dev_vin,v,size*length,cudaMemcpyHostToDevice);
	dim3 dimBlock(BLOCK_SIZE,1);
	dim3 dimGrid();	
	
	mean<<<dimGrid,dimBlock>>>(dev_vin,dev_meanArray,length);
	
	for (int i=0;i<size;i++){
		corrCoefGPU<<<dimGrid,dimBlock>>>(dev_vin,i,dev_meanArray,dev_vout,length);
	}
	cudaMemcpy (dev_vout,v,size*length,cudaMemcpyHostToDevice);
}