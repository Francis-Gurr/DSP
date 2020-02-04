#include "demodulator.h"
#include "h/init.h"

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

double fir_demod(double in, double *p_buff, int offset, int filter) {
	p_buff[offset] = in;
	double sum = 0;
	for (int j = offset; j >= 0; j--) {
		sum += p_buff[j] * H_DEMOD[filter][offset-j];
	}
	int i_H =  offset + 10;
	for (int j = 9; j > offset; j--) {
		sum += p_buff[j] * H_DEMOD[filter][i_H-j];
	}
	return sum;
}

void demod_costas(float *p_in, double *p_sum, double *p_diff, int *p_phase) {
	int sum_phase = p_phase[0];
	int diff_phase = p_phase[1];	
	double I, Q, lpf_out;
	int offset = 0;
	double i_sum_buf[10] = {0};
	double i_diff_buf[10] = {0};
	double q_sum_buf[10] = {0};
	double q_diff_buf[10] = {0};
	double iq_sum_buf[10] = {0};
	double iq_diff_buf[10] = {0};

	for (int i = 0; i < L; i++) {
		
		double val_in = (double)(p_in[i]);

		// SUM
		// Get sin phase
		int sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase < 0) {
			sin_phase += OSC_SIZE;
		}
		if(sin_phase >= OSC_SIZE) {
			sin_phase -= OSC_SIZE;
		}
		// Calculate output
		Q = val_in * OSC[sin_phase];
		//Q = fir_demod(Q, q_sum_buf, offset, 0);
		I = val_in * OSC[sum_phase];
		p_sum[i] = I;
		//I = fir_demod(I, i_sum_buf, offset, 0);
		// NCO ouput
		lpf_out = Q * I; // Q * I 
		//lpf_out = fir_demod(lpf_out, iq_sum_buf, offset, 2);
		// Next phase value
		sum_phase = sum_phase + (int)(lpf_out * PHASE_SCALE) + 40;
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}
		if (sum_phase < 0) {
			sum_phase += OSC_SIZE;
		}

		// DIFF
		// Get sin phase
		sin_phase = sum_phase + SIN_PHASE;
		if (sin_phase < 0) {
			sin_phase += OSC_SIZE;
		}
		if (sin_phase >= OSC_SIZE) {
			sin_phase -= OSC_SIZE;
		}
		// Calculate output
		Q = val_in * OSC[sin_phase];
		//Q = fir_demod(Q, q_diff_buf, offset, 1);
		I = val_in * OSC[diff_phase];
		p_diff[i] = I;
		//I = fir_demod(I, i_diff_buf, offset, 1);
		// NCO ouput
		lpf_out = Q * I; // Q * I 
		//lpf_out = fir_demod(lpf_out, iq_diff_buf, offset, 2);
		// Next phase value
		//
		diff_phase = diff_phase + (int)(lpf_out * PHASE_SCALE) + 42;
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
		if (diff_phase < 0) {
			diff_phase += OSC_SIZE;
		}
		if (++offset >= 10) {
			offset = 0;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;	
}
