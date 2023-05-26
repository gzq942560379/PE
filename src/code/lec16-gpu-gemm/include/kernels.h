#ifndef __PE_KERNELS_H__
#define __PE_KERNELS_H__

void pe_dgemm_v0(int M, int N, int K, pe_f64 alpha, pe_f64* A, pe_f64* B, 
                 pe_f64 beta, pe_f64* C);

#endif