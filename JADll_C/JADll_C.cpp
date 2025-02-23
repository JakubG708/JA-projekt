// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <iostream>
#include <utility>
#include <limits.h>
#include "JADll_C.h"




void matrixMultiplicationThread(double* resultMatrix, double* matrixA, double* matrixB, long long rows, long long cols, long long rowsPadded, long long colsPadded, long long startRow, long long endRow)
{

    for (long long i = startRow; i < endRow; i++) {
        for (long long j = 0; j < rowsPadded; j++) {
            double sum = 0.0;
            for (long long k = 0; k < colsPadded; k++) {
                sum += matrixA[i * colsPadded + k] * matrixB[j * rowsPadded + k];
            }
            resultMatrix[i * rowsPadded + j] = sum;
        }
    }
    

}