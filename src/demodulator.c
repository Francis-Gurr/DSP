#include "demodulator.h"
#include "h/init.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

void demod_coherent(double *p_sum, double *p_diff, int *p_phase, int){
	int sum_phase = p_phase[0];
	int diff_phase = p_phase[1];	
	for (int i = 0; i < L; i++) {
		// SUM
		p_sum[i] = p_sum[i] * OSC[sum_phase];
		sum_phase = (sum_phase + 20);
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}
		// DIFF 
		p_diff[i] = p_diff[i] * OSC[diff_phase];
		diff_phase = (diff_phase + 21);
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;
}

void demod_costas(double *p_sum, double *p_diff, int *phase) {
	int sum_phase = p_phase[0];
	int diff_phase = p_phase[1];	
	int phase_update = 0;
	double Q, lpf_out;

	for (int i = 0; i < L; i++) {
		// SUM
		sum = p_sum[i];
		// Get sin phase
		int sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase >= OSC_SIZE) {
			sin_phase -= OSC_SIZE;
		}
		// Calculate output
		I = sum * OSC[cos_phase];
		p_sum[i] = I;
		// NCO ouput
		lpf_out = sum * OSC[sin_phase] * I; // Q * I 
		// Next phase value
		sum_phase = sum_phase + (int)floor(lpf_out * PHASE_SCALE) + 20; //slow
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}

		// DIFF
		diff = p_diff[i];
		// Get sin phase
		int sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase >= OSC_SIZE) {
			sin_phase -= OSC_SIZE;
		}
		// Calculate output
		I = diff * OSC[cos_phase];
		p_diff[i] = I;
		// NCO ouput
		lpf_out = diff * OSC[sin_phase] * I; // Q * I 
		// Next phase value
		diff_phase = sum_phase + (int)floor(lpf_out * PHASE_SCALE) + 20; //slow
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;	
}
