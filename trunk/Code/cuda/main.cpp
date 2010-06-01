

#include"corrcoefGPU.h"

#include <iostream>
#include <fstream>
using namespace std;



double* get1DArray(int nrows,int ncol)
{

		int size=nrows*ncol;
		std::ifstream file("c:\\Vis\\FMRI.bin", std::ios::binary);
		double *arr=(double*)malloc(size*(sizeof(double)));
		int f;
		if (file)
				{		
 								for (int i=0;i<size;i++)
								{ file.read(reinterpret_cast<char*>(&f), 4);
								
								*(arr+i)=(double)f;			
								}
				}
          
		else
		{cout<<"ERROR FILE!!\n";}
return arr;
}

int main()
{
	int nrows=1280;
	int ncols=132;
	int status;
	double* v_in=(double*)malloc(nrows*ncols*sizeof(double));
	double** res=(double**)malloc(nrows*nrows*sizeof(double));
	v_in=get1DArray(nrows,ncols);
	status=corrcoefGPU_kernel(v_in,res,nrows,ncols);
}
