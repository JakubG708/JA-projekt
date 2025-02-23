// MathLibrary.h - Contains declarations of math functions
#pragma once

#ifdef JADLL_C_EXPORTS
#define JADLL_C_API __declspec(dllexport)
#else
#define JADLL_C_API __declspec(dllimport)
#endif

//extern "C" JADLL_C_API void matrixMultiplicationThreadOneDimentionalMatrix(double* resultMatrix, double* matrixA, double* matrixB, long long rows, long long cols, long long startRow, long long endRow);

//extern "C" JADLL_C_API void matrixMultiplicationThread(double** resultMatrix, double** matrixA, double** matrixB, long long rows, long long cols, long long startRow, long long endRow);

extern "C" JADLL_C_API void matrixMultiplicationThread(double* resultMatrix, double* matrixA, double* matrixB, long long rows, long long cols,long long rowsPadded, long long colsPadded, long long startRow, long long endRow);
