#include "h/init.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#ifndef _DEMOD
#define _DEMOD

#define OSC_SIZE 200
#define SIN_PHASE -50
#define SIZE 100
#define PHASE_SCALE 31.83099
#define PI 3.14159265

#endif

void demod_coherent(float *p_in, double *p_sum, double *p_diff,  int *p_phase, int *p_phi, int *p_count);

void demod_costas(float *p_in, double *p_sum, double *p_diff, int *p_phase, double *p_scale);

void demod_costas_2(float *p_in, double *p_sum, double *p_diff, int *p_phase, int *p_phi, int *p_count);
