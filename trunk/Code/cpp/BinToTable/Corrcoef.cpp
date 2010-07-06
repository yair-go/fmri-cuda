#include "Corrcoef.h"

//*****************************************************************/
//function name:	CorrcoefPrepare
//arguments:		A (in) - rendom vector
//					Pstatistics (out) - Pstatistics[0]-avarage,Pstatistics[1]-covariance
//					ArrayLentgh (in) - length of the vectors
//return value :	vector statistics
//*****************************************************************/
void CorrcoefPrepare (float* A , int ArrayLentgh, float* pStatistics)
{
	pStatistics[0] = 0;
	pStatistics[1] = 0;
	for (int i = 0 ; i < ArrayLentgh ; i++)
		pStatistics[0] += A[i];
	pStatistics[0] /= ArrayLentgh ;
	for (int i = 0 ; i < ArrayLentgh ; i++)
		pStatistics[1] += ( A[i] - pStatistics[0] ) * ( A[i] - pStatistics[0] );
}
//*****************************************************************/
//function name:	Corrcoef
//arguments:		A (in) - rendom vector
//					B (in) - rendom vector, same length as A
//					ArrayLentgh (in) - length of the vectors
//return value :	result of Corrcoef
//*****************************************************************/
float Corrcoef ( float * A , float* PstatisticsA,float * B ,float* PstatisticsB, int ArrayLentgh )
{
	float CovAB = 0; 
	/**loop for standard deviation ******************************/
	for (int i = 0 ; i < ArrayLentgh ; i++){
		CovAB += ( A[i] - PstatisticsA[0] ) * ( B[i] - PstatisticsB[0] );
	}
	return CovAB / ( sqrt (PstatisticsA[1] * PstatisticsB[1]) + E );
}
