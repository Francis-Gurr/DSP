#include<fftw3.h>
#include<stdio.h>
#include "h/init.h"
#include "fir_fft.h"

void fir_fft(float *p_in, double *p_out, int filter_id, double *p_buf) {
	
	double x[N] = {0}, y[N] = {0};
	fftw_complex *X = fftw_alloc_complex(N);
	fftw_complex *Y = fftw_alloc_complex(N);
	fftw_plan p, q;

	// Produce input sample of length N = L + M -1
	for (int i = 0; i < M-1; i++) {
	       x[i] = *(p_buf+i);
	}	       
	for (int i = 0; i < L; i++) {
	       x[M-1+i] = (double)*(p_in+i);
	}

	// Do FFT
	p = fftw_plan_dft_r2c_1d(N, x, X, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);
	
	// Y(K) = H(K)X(K)
	double mux = 1.0/N;

	for (int i = 0; i < N; i++) {
		Y[i][0] = (X[i][0] * H[filter_id][i][0] - X[i][1] * H[filter_id][i][1]) * mux;
		Y[i][1] = (X[i][0] * H[filter_id][i][1] + X[i][1] * H[filter_id][i][0]) * mux;
	}
	
	// IDFT
	q = fftw_plan_dft_c2r_1d(N, Y, y, FFTW_ESTIMATE);
	fftw_execute(q);
	fftw_destroy_plan(q);

	fftw_free(X);
	fftw_free(Y);

	// Drop M-1 first samples to give y(n)	
	for (int i = 0; i < M-1; i++) {
	       *(p_buf+i) = y[i];
	}	       
	for (int i = 0; i < L; i++) {
	       *(p_out+i) = y[M-1+i];
	}
}
