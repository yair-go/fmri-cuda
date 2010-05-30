// corrcoef.cpp : Defines the entry point for the console application.
// remark
//#include <atltime.h>
//#include <iostream>
//#include <cstdlib>
//#include <stdlib.h>
//#include <fstream>
//#include <string>
#include "corrcoef.h"
#include <math.h>
#include <iostream>
using namespace std;


int main(void)
{
	double a[9] = {1,2,3,4,5,4,3,2,1};
	double b[9] = {1,2,3,4,5,4,3,1,1};
	double c[9] = {5,4,3,2,1,2,3,4,5};
	double coef = 0;
	coef = corrCoef(a,c,9);
	cout<<"corrCorf(a,b): "<<coef<<endl;
	coef = corrCoef(a,b,9);
	cout<<"corrCorf(a,c): "<<coef<<endl;
	coef = corrCoef(a,a,9);
	cout<<"corrCorf(a,a): "<<coef<<endl;
	int x;
	cin >> x;
	return 0;
}