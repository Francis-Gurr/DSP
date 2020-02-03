#include "demodulator.h"
#include "structs.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

void demod_coherent(double *p_in, int *phase, int sum_or_diff){

	int phase_update = PHASE_INC[sum_or_diff];
	double I;

	double sample = *p_in;
	double cos_val = OSC[*phase];

	// calculate output
	I = sample * cos_val;
	*p_in = I;

	// update phase value
	// phase is really phase * 100 / pi - assumes 200 values in osc
	if ((*phase += phase_update) >= OSC_SIZE) {
		*phase -= OSC_SIZE;
	}
}

void demod_costas(double *p_in, int *phase, int sum_or_diff) {

	// initialise values
	int phase_update = 0;
	double I, Q, IQ, lpf_out;

	double sample = *p_in;

	// calculate output to NCO
	double cos_val = OSC[*phase];
	double sin_val;
	if (*phase + SIN_PHASE < OSC_SIZE) {
		sin_val = OSC[*phase + SIN_PHASE];
	}
	else {
		sin_val = OSC[*phase + SIN_PHASE - OSC_SIZE];
	}

	// calculate outputs
	I = sample * cos_val;
	Q = sample * sin_val;
	*p_in = I;
	IQ = I * Q;

	//LPF
	lpf_out = IQ;

	//update phase value
	phase_update = floor(lpf_out * PHASE_SCALE);
	if ((*phase += phase_update) >= OSC_SIZE) {
		*phase -= OSC_SIZE;
	}

}
