#include "cuda.h"
#define BLOCK_SIZE 128

double* corrcoefGPU_kernel(double v[], int size, int length);