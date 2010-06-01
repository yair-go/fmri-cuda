// readFromFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;


double* readFromFile(int nrows,int ncolumns);

double* readFromFile(int nrows,int ncolumns)
{
	double f;
	
	double *array1 = (double *)malloc(nrows * ncolumns * sizeof(double *));

	std::ifstream file("c:\\FMRI.bin", std::ios::binary);
	if (file){
		for (int i=0;i<nrows*ncolumns;i++){
					file.read(reinterpret_cast<char*>(&f), 4);
					*(array1+i)=f;	 
				}
		}
	} 
	return array1;  
}


int _tmain(int argc, _TCHAR* argv[])
{
	int	nrows=1000;
	int ncolumns=132;
	readFromFile(nrows,ncolumns);
	return 0;
}




