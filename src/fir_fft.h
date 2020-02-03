#include<fftw3.h>
#include<stdio.h>
#include "h/init.h"

#define L 1875
#define M 173
#define N 2048

void fir_fft(float *p_in, double *p_out, int filter_id, double *p_buf);
