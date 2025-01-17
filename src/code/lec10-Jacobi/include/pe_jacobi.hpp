#ifndef _PE_DGEMM_HPP_
#define _PE_DGEMM_HPP_
#include<cstdint>
#include"macro.hpp"

template <typename T>
void pe_jacobi2d_template(T *y, T *x, int64_t jmax, int64_t kmax,
		T scale, int64_t repeat = 1){
  int64_t j, k;
  for(int r = 0; r < repeat; r++) {
    for(j = 1; j < jmax-1; j++) {
      for(k = 1; k < kmax-1; k++) {
        ARRAY_2D(y, j, jmax, k, kmax) = 
          scale * ( ARRAY_2D(x, j - 1,k, jmax, kmax) +
          ARRAY_2D(x, j+1, k, jmax, kmax) +
          ARRAY_2D(x, j, k-1, jmax, kmax) +
          ARRAY_2D(x, j, k+1, jmax, kmax) );
      }
    }
  }
}

template <typename T, unsigned int KBLOCK>
void pe_jacobi2d_blocking_template(T *y, T *x, int64_t jmax, int64_t kmax,
		T scale){
  int64_t j, k, kb;
  for(kb = 1; kb < kmax - 1; kb += KBLOCK) {
    for(j = 1; j < jmax - 1; j++) {
      for(k = kb; k < kb + KBLOCK; k++) {
        ARRAY_2D(y, j, jmax, k, kmax) = 
          scale * ( 
            ARRAY_2D(x, j-1, k, jmax, kmax) +
            ARRAY_2D(x, j+1, k, jmax, kmax) +
            ARRAY_2D(x, j,   k-1, jmax, kmax) + 
	          ARRAY_2D(x, j,   k+1, jmax, kmax) );
      }
    }
  }
}

template <class T>
void pe_jacobi3d_template(T *y, T *x, int64_t imax, int64_t jmax, 
		int64_t kmax, T scale){
  int64_t i, j, k;
  for(i = 1; i < imax-1; i++) {
    for(j = 1; j < jmax-1; j++) {
      for(k = 1; k < kmax-1; k++) {
        ARRAY_2D(y, j, jmax, k, kmax) = 
          scale * ( ARRAY_3D(x, i-1, j, k, imax, jmax, kmax) +
          ARRAY_3D(x, i, j-1, k, imax, jmax, kmax) +
          ARRAY_3D(x, i, j, k-1, imax, jmax, kmax) +
          ARRAY_3D(x, i, j, k+1, imax, jmax, kmax) +
          ARRAY_3D(x, i, j+1, k, imax, jmax, kmax) +
          ARRAY_3D(x, i+1, j, k, imax, jmax, kmax) );
      }
    }
  }
}

template <typename T, unsigned int JBLOCK>
void pe_jacobi3d_jblocking_template(T *y, T *x, int64_t imax, int64_t jmax, 
		int64_t kmax, T scale){
  int64_t i, j, k, jb;
  for(jb = 1; jb < jmax; jb += JBLOCK) {
    for(i = 1; i < imax-1; i++) {
      for(j = jb; j < jb + JBLOCK - 1; j++) {
        for(k = 1; k < kmax-1; k++) {
          ARRAY_2D(y, j, jmax, k, kmax) = 
            scale * ( ARRAY_3D(x, i-1, j, k, imax, jmax, kmax) +
            ARRAY_3D(x, i, j-1, k, imax, jmax, kmax) +
            ARRAY_3D(x, i, j, k-1, imax, jmax, kmax) +
            ARRAY_3D(x, i, j, k+1, imax, jmax, kmax) +
            ARRAY_3D(x, i, j+1, k, imax, jmax, kmax) +
            ARRAY_3D(x, i+1, j, k, imax, jmax, kmax) );
        }
      }
    }
  }
}

void pe_jacobi2d(double *y, double *x, int64_t jmax, int64_t kmax,
		double scale, int64_t repeat);
void pe_jacobi2d_blocking(double *y, double *x, uint64_t jmax,
		uint64_t kmax, double scale, unsigned int kblock);
void pe_jacobi2d_blocking_parallel(double *y, double *x, uint64_t jmax,
		uint64_t kmax, double scale, unsigned int kblock);
void pe_jacobi3d(double *y, double *x, int64_t imax, int64_t jmax,
		int64_t kmax, double scale);
void pe_jacobi3d_iparallel(double *y, double *x, int64_t imax, int64_t jmax, 
  int64_t kmax, double scale);
void pe_jacobi3d_iparallel_block(double *y, double *x, int64_t imax, int64_t jmax, 
  int64_t kmax, double scale, uint32_t jblock);
void pe_jacobi3d_iparallel_ntstore(double *y, double *x, int64_t imax, int64_t jmax, 
  int64_t kmax, double scale);
void pe_jacobi3d_iparallel_block_ntstore(double *y, double *x, int64_t imax, int64_t jmax, 
  int64_t kmax, double scale, uint32_t iblock);
void pe_jacobi3d_iparallel_block_ntstore256(double *y, double *x, int64_t imax, int64_t jmax, int64_t kmax, double scale, uint32_t jblock);
void pe_jacobi3d_iparallel_block_ntstore512(double *y, double *x, int64_t imax, int64_t jmax, int64_t kmax, double scale, uint32_t jblock);

void pe_jacobi3d_jparallel(double *y, double *x, int64_t imax, int64_t jmax, 
  int64_t kmax, double scale); 

void pe_jacobi3d_iparallel_ntstore(double *y, double *x, int64_t imax, int64_t jmax, 
  int64_t kmax, double scale);
#endif
