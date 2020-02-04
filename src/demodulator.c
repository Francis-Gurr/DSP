#include "demodulator.h"
#include "h/init.h"
#include "fir_fft.h"
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

void demod_coherent(float *p_in, double *p_sum, double *p_diff, int *p_phase){
	int sum_phase = *p_phase;
	int diff_phase = *p_phase+1;	
	for (int i = 0; i < L; i++) {

		double val_in = (double)(p_in[i]);

		// SUM
		p_sum[i] = val_in * OSC[sum_phase];
		sum_phase = (sum_phase + 40);
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}
		// DIFF 
		p_diff[i] = val_in * OSC[diff_phase];
		diff_phase = (diff_phase + 42);
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;
}

void demod_costas(float *p_in, double *p_sum, double *p_diff, int *p_phase) {
	int sum_phase = *p_phase;
	int diff_phase = *p_phase+1;	
	double I, lpf_out;

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
		//lpf_out = val_in * OSC[sin_phase] * I; // Q * I 
		lpf_out = atan2(OSC[sum_phase],OSC[sin_phase]);
		// Next phase value
		sum_phase = sum_phase + (int)(lpf_out * PHASE_SCALE) + 40;
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}

		// DIFF
		// Get sin phase
		sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase >= OSC_SIZE) {
			sin_phase -= OSC_SIZE;
		}
		// Calculate output
		I = val_in * OSC[diff_phase];
		p_diff[i] = I;
		// NCO ouput
		//lpf_out = val_in * OSC[sin_phase] * I; // Q * I 
		lpf_out = atan2(OSC[diff_phase],OSC[sin_phase]);
		// Next phase value
		diff_phase = diff_phase + (int)(lpf_out * PHASE_SCALE) + 42;
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;	
}
