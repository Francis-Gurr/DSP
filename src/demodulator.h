#include "h/init.h"
#include "fir_fft.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

#ifndef _DEMOD
#define _DEMOD

#define OSC_SIZE 200
#define SIN_PHASE 50
#define PHASE_SCALE 31.83099

#endif

void demod_coherent(float *p_in, double *p_sum, double *p_diff,  int *p_phase);

void demod_costas(float *p_in, double *p_sum, double *p_diff, int *p_phase);
