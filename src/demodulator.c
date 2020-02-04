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

double fir_demod(double in, double *p_buff, int *p_offset, int filter) {
	int offset = *p_offset;
	p_buff[offset] = in;
	if (++offset >= 10) {
		offset = 0;
	}
	double sum = 0;
	for (int j = offset; j >= 0; j--) {
		sum += p_buff[j] * H_DEMOD[filter][offset-j];
	}
	int i_H =  offset + 9;
	for (int j = 9; j > offset; j--) {
		sum += p_buff[j] * H_DEMOD[filter][i_H-j];
	}
	*p_offset = offset;
	return sum
}

void demod_costas(float *p_in, double *p_sum, double *p_diff, int *p_phase) {
	int sum_phase = *p_phase;
	int diff_phase = *p_phase+1;	
	double I, Q, lpf_out;
	double i_sum_buf[10] = {0};
	int i_sum_offset = 0;
	double i_diff_buf[10] = {0};
	int i_diff_offset = 0;
	double q_sum_buf[10] = {0};
	int q_sum_offset = 0;
	double q_diff_buf[10] = {0};
	int q_diff_offset = 0;
	double iq_sum_buf[10] = {0};
	int iq_sum_offset = 0;
	double iq_diff_buf[10] = {0};
	int iq_diff_offset = 0;

	for (int i = 0; i < L; i++) {
		
		double val_in = (double)(p_in[i]);

		// SUM
		// Get sin phase
		int sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase < 0) {
			sin_phase += OSC_SIZE;
		}
		// Calculate output
		Q = val_in * OSC[sin_phase];
		Q = fir_demod(Q, q_sum_buf, &q_sum_offset, 0);
		I = val_in * OSC[sum_phase];
		p_sum[i] = I;
		I = fir_demod(I, i_sum_buf, &i_sum_offset, 0);
		// NCO ouput
		lpf_out = Q * I; // Q * I 
		lpf_out = fir_demod(lpf_out, iq_sum_buf, &iq_sum_offset, 2);
		//lpf_out = atan2(OSC[sum_phase],OSC[sin_phase]);
		// Next phase value
		sum_phase = sum_phase + (int)(lpf_out * PHASE_SCALE) + 40;
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}

		// DIFF
		// Get sin phase
		sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase < 0) {
			sin_phase += OSC_SIZE;
		}
		// Calculate output
		Q = val_in * OSC[sin_phase];
		Q = fir_demod(Q, q_diff_buf, &q_diff_offset, 1);
		I = val_in * OSC[diff_phase];
		p_diff[i] = I;
		I = fir_demod(I, i_diff_buf, &i_diff_offset, 1);
		// NCO ouput
		lpf_out = Q * I; // Q * I 
		lpf_out = fir_demod(lpf_out, iq_diff_buf, &iq_diff_offset, 2);
		//lpf_out = atan2(OSC[diff_phase],OSC[sin_phase]);
		// Next phase value
		diff_phase = diff_phase + (int)(lpf_out * PHASE_SCALE) + 42;
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;	
}
