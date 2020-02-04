#include<fftw3.h>
#include<stdio.h>
#include "h/init.h"
#include "fir_fft.h"

void fir_fft(double *p_sum, double *p_diff, double *p_buf_sum, double *p_buf_diff) {
	
	double x_sum[N] = {0}, y_sum[N] = {0};
	double x_diff[N] = {0}, y_diff[N] = {0};
	fftw_complex *X_SUM = fftw_alloc_complex(N);
	fftw_complex *Y_SUM = fftw_alloc_complex(N);
	fftw_complex *X_DIFF = fftw_alloc_complex(N);
	fftw_complex *Y_DIFF = fftw_alloc_complex(N);
	fftw_plan p1, q1, p2, q2;

	// Produce input sample of length N = L + M -1
	for (int i = 0; i < M-1; i++) {
	       x_sum[i] = *(p_buf_sum+i);
	       x_diff[i] = *(p_buf_diff+1);
	}	       
	for (int i = 0; i < L; i++) {
	       x_sum[M-1+i] = *(p_sum+i);
	       x_diff[M-1+i] = *(p_diff+i);
	}

	// Do FFT
	p1 = fftw_plan_dft_r2c_1d(N, x_sum, X_SUM, FFTW_ESTIMATE);
	fftw_execute(p1);
	fftw_destroy_plan(p1);
	
	p2 = fftw_plan_dft_r2c_1d(N, x_diff, X_DIFF, FFTW_ESTIMATE);
	fftw_execute(p2);
	fftw_destroy_plan(p2);
	
	// Y(K) = H(K)X(K)
	double mux = 1.0/N;

	for (int i = 0; i < N; i++) {
		Y_SUM[i][0] = (X_SUM[i][0] * H_LOW[i][0] - X_SUM[i][1] * H_LOW[i][1]) * mux;
		Y_SUM[i][1] = (X_SUM[i][0] * H_LOW[i][1] + X_SUM[i][1] * H_LOW[i][0]) * mux;
		Y_DIFF[i][0] = (X_DIFF[i][0] * H_LOW[i][0] - X_DIFF[i][1] * H_LOW[i][1]) * mux;
		Y_DIFF[i][1] = (X_DIFF[i][0] * H_LOW[i][1] + X_DIFF[i][1] * H_LOW[i][0]) * mux;
	}
	
	// IDFT
	q1 = fftw_plan_dft_c2r_1d(N, Y_SUM, y_sum, FFTW_ESTIMATE);
	fftw_execute(q1);
	fftw_destroy_plan(q1);

	q2 = fftw_plan_dft_c2r_1d(N, Y_DIFF, y_diff, FFTW_ESTIMATE);
	fftw_execute(q2);
	fftw_destroy_plan(q2);

	fftw_free(X_SUM);
	fftw_free(Y_SUM);
	fftw_free(X_DIFF);
	fftw_free(Y_DIFF);

	// Drop M-1 first samples to give y(n)	
	for (int i = 0; i < M-1; i++) {
	       *(p_buf_sum+i) = y_sum[i];
	       *(p_buf_diff+i) = y_diff[i];
	}	       
	for (int i = 0; i < L; i++) {
	       *(p_sum+i) = y_sum[M-1+i];
	       *(p_diff+i) = y_diff[M-1+i];
	}
}
