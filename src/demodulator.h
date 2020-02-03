#include "h/init.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

#ifndef _DEMOD
#define _DEMOD

#define OSC_SIZE 200
#define SIN_PHASE 50
#define PHASE_SCALE 31.83099

#endif

void  demod_coherent(double *p_in,  int *phase, int sum_or_diff);

void demod_costas(double *p_in, int *phase, int sum_or_diff);
