#include "h/init.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#ifndef _DEMOD
#define _DEMOD

#define SUM_FREQ 1.256637061
#define DIFF_FREQ 1.319468915
#define PHI_INC 0.0001

#endif

void demod_coherent(float *p_in, double *p_sum, double *p_diff, double *p_phi, int *p_phase);

void demod_costas(double *p_in, double *p_sum, double *p_diff, double *p_phi, int *p_count);
