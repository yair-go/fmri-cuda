// corrcoef.cpp : Defines the entry point for the console application.
// remark
//#include <atltime.h>
//#include <iostream>
//#include <cstdlib>
//#include <stdlib.h>
//#include <fstream>
//#include <string>
#include <math.h>
#include <iostream>
using namespace std;

double corrCoef(double x[],double y[],int length);

//*****************************************************************/
//function name:	corrCoef
//arguments:		x[] (in) - rendom vector
//					y[] (in) - rendom vector, same length as x
//					length (in) - length of thr vectors
//return value :	result of corrCoef
//*****************************************************************/
double corrCoef(double x[],double y[],int length)
{
    const double TINY=1.0e-20;//tiny number to avoide division with zero
	double res = 0;
    int j;
    double syy=0.0,sxy=0.0,sxx=0.0,ay=0.0,ax=0.0;
    /**first loop to find the mean of x and y ***************************/
	for (j=0;j<length;j++) 
	{
        ax += x[j];
        ay += y[j];
    }
    ax /= length;
    ay /= length;
	/**secound loop for standard deviation ******************************/
    for (j=0;j<length;j++) 
	{     
        sxx += (x[j]-ax)*(x[j]-ax);
        syy += (y[j]-ay)*(y[j]-ay);
        sxy += (x[j]-ax)*(y[j]-ay);
    }
	/********************************************************************/
    res=sxy/(sqrt(sxx*syy)+TINY);
	return res;
}

