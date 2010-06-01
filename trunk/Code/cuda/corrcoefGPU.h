#include "cuda.h"
#include "cutil_inline.h"

#define BLOCK_SIZE 128

int corrcoefGPU_kernel(double v[], double** res, int size, int length);