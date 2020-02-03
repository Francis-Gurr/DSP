#include "demodulator.h"
#include "h/init.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

void demod_coherent(double *p_sum, double *p_diff, int *p_phase){
	int sum_phase;
	int diff_phase;	
	for (int i = 0; i < 1875; i++) {
		// Sum
		sum_phase = p_phase[0];
		p_sum[i] = p_sum[i] * OSC[sum_phase];
		sum_phase = (sum_phase + 20);
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}
		// diff
		diff_phase = p_phase[1];
		p_diff[i] = p_diff[i] * OSC[diff_phase];
		diff_phase = (diff_phase + 21);
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;
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
	*phase = (*phase + inc + phase_update) % OSC_SIZE;
}
