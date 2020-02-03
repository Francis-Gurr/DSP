#include "demodulator.h"
#include "h/init.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

void demod_coherent(double *p_in, int *phase, int inc){

	// calculate output
	*p_in = *p_in * OSC[*phase];

	// update phase value
	*phase = (*phase + inc) % OSC_SIZE;
}

void demod_costas(double *p_in, int *phase, int inc) {

	// initialise values
	int phase_update = 0;
	double Q, lpf_out;

	int sin_phase = (*phase + SIN_PHASE) % OSC_SIZE;

	// calculate output to NCO
	double cos_val = OSC[*phase];
	double sin_val = OSC[sin_phase];

	// calculate outputs
	Q = *p_in * sin_val;
	*p_in = *p_in * cos_val; // I

	//LPF
	lpf_out = *p_in * Q; //IQ

	//update phase value
	phase_update = floor(lpf_out * PHASE_SCALE);
	*phase = (*phase + phase_update) % OSC_SIZE;
}
