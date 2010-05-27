
#include "stdafx.h"

#include <iostream>
#include <fstream>
using namespace std;
double* get1DArray(int nrows,int ncol);

int _tmain(int argc, _TCHAR* argv[])
{

	double *d=get1DArray(132,1000);
     cout<<*(d);
	int a;
	cin>>a;
	 return 0;
}

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
