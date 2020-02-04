#include<fftw3.h>
#include<stdio.h>
#include "h/init.h"

#define L 1875
#define M 173
#define N 2048

void fir_fft(double *p_sum, double *p_diff, double *p_buf_sum, double *p_buf_diff);
