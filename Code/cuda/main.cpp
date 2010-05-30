
#include "stdafx.h"
#include BrainToArray.h"
#include"corrcoef.cuh"

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	int nrow=1280;
	int ncol=132;
	int status;
	double* v_in=(double*)malloc(nrows*ncol*sizeof(double));
	double* res=(double*)malloc(nrows*nrows*sizeof(double));
	v_in=get1DArray(nrows,ncol);
	status=corrcoefGPU_kernel(v_in,res,nrow,ncol);
}
