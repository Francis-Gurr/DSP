#ifndef _DEMOD
#define _DEMOD

#include "structs.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

const int OSC_SIZE = 200;
const int SIN_PHASE = 50;
const double PHASE_SCALE = 31.83099
const int PHASE_INC[2] = {20, 21};

void  demod_coherent(double *p_in,  struct Demod *osc, int sum_or_diff);

void demod_costas(double *p_in, struct Demod *osc, int sum_or_diff);

#endif
