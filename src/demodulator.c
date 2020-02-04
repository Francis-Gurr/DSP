#include "demodulator.h"
#include "h/init.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

void demod_coherent(float *p_in, double *p_sum, double *p_diff, int *p_phase, int){
	int sum_phase = p_phase[0];
	int diff_phase = p_phase[1];	
	for (int i = 0; i < L; i++) {

		val_in = (double)(p_in[i]);

		// SUM
		p_sum[i] = val_in * OSC[sum_phase];
		sum_phase = (sum_phase + 20);
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}
		// DIFF 
		p_diff[i] = val_in * OSC[diff_phase];
		diff_phase = (diff_phase + 21);
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;
}

void demod_costas(float *p_in, double *p_sum, double *p_diff, int *phase) {
	int sum_phase = p_phase[0];
	int diff_phase = p_phase[1];	
	int phase_update = 0;
	double Q, lpf_out;

	for (int i = 0; i < L; i++) {
		
		double val_in = (double)(p_in[i]);

		// SUM
		// Get sin phase
		int sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase >= OSC_SIZE) {
			sin_phase -= OSC_SIZE;
		}
		// Calculate output
		I = val_in * OSC[sum_phase];
		p_sum[i] = I;
		// NCO ouput
		lpf_out = val_in * OSC[sin_phase] * I; // Q * I 
		// Next phase value
		sum_phase = sum_phase + (int)(lpf_out * PHASE_SCALE) + 20;
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}

		// DIFF
		// Get sin phase
		int sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase >= OSC_SIZE) {
			sin_phase -= OSC_SIZE;
		}
		// Calculate output
		I = val_in * OSC[diff_phase];
		p_diff[i] = I;
		// NCO ouput
		lpf_out = val_in * OSC[sin_phase] * I; // Q * I 
		// Next phase value
		diff_phase = diff_phase + (int)(lpf_out * PHASE_SCALE) + 20;
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;	
}
