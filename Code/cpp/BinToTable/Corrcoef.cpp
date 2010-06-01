#include "Corrcoef.h"

//*****************************************************************/
//function name:	Corrcoef
//arguments:		A (in) - rendom vector
//					B (in) - rendom vector, same length as A
//					ArrayLentgh (in) - length of the vectors
//return value :	result of Corrcoef
//*****************************************************************/
float Corrcoef ( float * A , float * B , int ArrayLentgh ){

float AvgA = 0 , AvgB = 0;
float CovAB = 0 , CovAA = 0 , CovBB = 0 ; 

/**first loop to find the mean of A and B ***************************/
for (int i = 0 ; i < ArrayLentgh ; i++)
{
	AvgA += *(A+i);
	AvgB += *(B+i);
}
AvgA /= ArrayLentgh ;
AvgB /= ArrayLentgh ;
 
/**secound loop for standard deviation ******************************/
for (int i = 0 ; i < ArrayLentgh ; i++){
CovAB += ( *(A+i) - AvgA ) * ( *(B+i) - AvgB );
CovAA += ( *(A+i) - AvgA ) * ( *(A+i) - AvgA );
CovBB += ( *(B+i) - AvgB ) * ( *(B+i) - AvgB );

}// <p> End of for loop. </p>

return CovAB / ( sqrt (CovAA *CovBB ) + E );

} // <p> End of function Corrcoef(). </p>
