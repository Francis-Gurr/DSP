#include "h/init.h"

#ifndef _DEMOD
#define _DEMOD

#define OSC_SIZE 200
#define SIN_PHASE -50
#define SIZE 10
#define PHASE_SCALE 31.83099

#endif

void demod_coherent(float *p_in, double *p_sum, double *p_diff,  int *p_phase);

void demod_costas(float *p_in, double *p_sum, double *p_diff, int *p_phase);
