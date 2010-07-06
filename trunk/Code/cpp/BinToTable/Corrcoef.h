#ifndef CORRCOEF
#define CORRCOEF 1

#include <math.h>
#include <iostream>

using namespace std ;
#define E (float) 1.0e-6
//E is a tiny number to avoide division by zero
float Corrcoef ( float * A , float* PstatisticsA,float * B ,float* PstatisticsB, int ArrayLentgh );
void CorrcoefPrepare (float* A , int ArrayLentgh, float* pStatistics);

#endif