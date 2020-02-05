#include "h/init.h"
#include "demodulator.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void demod_coherent(float *p_in, double *p_sum, double *p_diff, int *p_phase, int *p_phi, int *p_count){
	int sum_phase = p_phase[0];
	int diff_phase = p_phase[1];	
	double sum_phi = p_phi[0];
	double diff_phi = p_phi[1];
	int count = *p_count;
	for (int i = 0; i < L; i++) {

		double val_in = (double)(p_in[i]);

		// SUM
		/*p_sum[i] = 2 * val_in * OSC[sum_phase] * sum_scale;
		sum_phase = (sum_phase + 40);
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}*/
		p_sum[i] = val_in * cos(2*PI*count/5 + sum_phi);
		// DIFF 
		/*p_diff[i] = 2 * val_in * OSC[diff_phase] * diff_scale;
		diff_phase = (diff_phase + 42);
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}*/
		p_diff[i] = val_in * cos(2*PI*1.05*count/5 + diff_phi);
		count++;
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;
	*p_count = count;
}

void demod_costas(float *p_in, double *p_sum, double *p_diff, int *p_phase, double *p_scale) {
	int sum_phase = p_phase[0];
	int diff_phase = p_phase[1];	
	double I_sum, Q_sum, LPF_sum;
	double I_diff, Q_diff, LPF_diff;
	int offset = 0;
	double buff_q_sum[SIZE] = {0};
	double buff_i_sum[SIZE] = {0};
	double buff_lpf_sum[SIZE] = {0};
	double buff_q_diff[SIZE] = {0};
	double buff_i_diff[SIZE] = {0};
	double buff_lpf_diff[SIZE] = {0};

	for (int i = 0; i < L; i++) {
		double in = (double)(p_in[i]);

		// SUM
		// Get sin phase
		int sin_phase_sum = sum_phase + SIN_PHASE;
		if (sin_phase_sum < 0) {
			sin_phase_sum += OSC_SIZE;
		}
		if(sin_phase_sum >= OSC_SIZE) {
			sin_phase_sum -= OSC_SIZE;
		}
		int sin_phase_diff = diff_phase + SIN_PHASE;
		if (sin_phase_diff < 0) {
			sin_phase_diff += OSC_SIZE;
		}
		if(sin_phase_diff >= OSC_SIZE) {
			sin_phase_diff -= OSC_SIZE;
		}

		// Calculate output
		Q_sum = in * OSC[sin_phase_sum];
		I_sum = in * OSC[sum_phase];
		p_sum[i] = 2 * I_sum;
		Q_diff = in * OSC[sin_phase_diff];
		I_diff = in * OSC[diff_phase];
		p_diff[i] = 2 * I_diff;

		/* FIR */
		// Add to buffers
		buff_q_sum[offset] = Q_sum;
		buff_i_sum[offset] = I_sum;
		buff_q_diff[offset] = Q_diff;
		buff_i_diff[offset] = I_diff;

		// Do FIR
		Q_sum = 0;
		I_sum = 0;
		LPF_sum = 0;
		Q_diff = 0;
		I_diff = 0;
		LPF_diff = 0;

		for (int j = offset; j >= 0; j--) {
		       Q_sum += buff_q_sum[j] * H_DEMOD[0][offset-j];	       
		       I_sum += buff_i_sum[j] * H_DEMOD[0][offset-j];	       
		       Q_diff += buff_q_diff[j] * H_DEMOD[1][offset-j];	       
		       I_diff += buff_i_diff[j] * H_DEMOD[1][offset-j];	       
		       //LPF_sum += buff_lpf_sum[j] * H_DEMOD[2][offset-j];	       
		       //LPF_diff += buff_lpf_diff[j] * H_DEMOD[2][offset-j];	       
		}
		int i_H = offset + SIZE - 1;
		for (int j = SIZE-1; j > offset; j--) {
		       Q_sum += buff_q_sum[j] * H_DEMOD[0][i_H-j];	       
		       I_sum += buff_i_sum[j] * H_DEMOD[0][i_H-j];	       
		       Q_diff += buff_q_diff[j] * H_DEMOD[1][i_H-j];	       
		       I_diff += buff_i_diff[j] * H_DEMOD[1][i_H-j];	       
		       //LPF_sum += buff_lpf_sum[j] * H_DEMOD[2][i_H-j];	       
		       //LPF_diff += buff_lpf_diff[j] * H_DEMOD[2][i_H-j];	       
		}
		buff_lpf_sum[offset] = I_sum * Q_sum;
		buff_lpf_diff[offset] = I_diff * Q_diff;

		for (int j = offset; j >= 0; j--) {
		       LPF_sum += buff_lpf_sum[j] * H_DEMOD[2][offset-j];	       
		       LPF_diff += buff_lpf_diff[j] * H_DEMOD[2][offset-j];	       
		}
		for (int j = SIZE-1; j > offset; j--) {
		       LPF_sum += buff_lpf_sum[j] * H_DEMOD[2][i_H-j];	       
		       LPF_diff += buff_lpf_diff[j] * H_DEMOD[2][i_H-j];	       
		}

		if (++offset >= SIZE) {
			offset = 0;
		}

		// Next phase value
		sum_phase = sum_phase + (int)(LPF_sum * PHASE_SCALE) + 40;
		if (sum_phase >= OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}
		if (sum_phase < 0) {
			sum_phase += OSC_SIZE;
		}

		diff_phase = diff_phase + (int)(LPF_diff * PHASE_SCALE) + 42;
		if (diff_phase >= OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
		if (diff_phase < 0) {
			diff_phase += OSC_SIZE;
		}
	}
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;	
}

void demod_costas_2(float *p_in, double *p_sum, double *p_diff, int *p_phase, int *p_phi, int *p_count) {

	double phi_sum[L] = {0}, phi_diff[L] = {0};
	double I_sum[L] = {0}, I_diff[L] = {0};
	double Q_sum[L] = {0}, Q_diff[L] = {0};
	double out_sum[L] = {0}, out_diff[L] = {0};
	double phase_sum[L] = {0}, phase_diff[L] = {0};
	int sum_phase = p_phase[0], diff_phase = p_phase[1];
	int count = *p_count;

	for (int i = 0; i < L; i++) {
		double in = (double)(p_in[i]);
		if (i > 0) {
			if (out_sum[i-1] * phase_sum[i-1] >= 0) {
				phi_sum[i] = phi_sum[i-1] - (0.00005*PI);
				phi_diff[i] = phi_diff[i-1] - (0.00005*PI);
			}
			else {
				phi_sum[i] = phi_sum[i-1] + (0.00005*PI);
				phi_diff[i] = phi_diff[i-1] + (0.00005*PI);
			}
		}
		else {
			phi_sum[i] = p_phi[0];
			phi_diff[i] = p_phi[1];
		}

		I_sum[i] = in * cos(2*PI*count/5 + phi_sum[i]);
		I_diff[i] = in * cos(2*PI*1.05*count/5 + phi_diff[i]);
		Q_sum[i] = in * sin(2*PI*count/5 + phi_sum[i]);
		Q_diff[i] = in * sin(2*PI*1.05*count/5 + phi_diff[i]);
		count++;

		if (i <= 5) {
			for (int j = 0; j < i; j++) {
				out_sum[i] += I_sum[j];
				out_diff[i] += I_diff[j];
				phase_sum[i] += Q_sum[j];
				phase_diff[i] += Q_diff[j];
			}
		}
		else {
			for (int j = i - 4; j < i; j++) {
				out_sum[i] += I_sum[j];
				out_diff[i] += I_diff[j];
				phase_sum[i] += Q_sum[j];
				phase_diff[i] += Q_diff[j];
			}
		}
		p_sum[i] = out_sum[i];
		p_diff[i] = out_diff[i];
		if (sum_phase += 40 > OSC_SIZE) {
			sum_phase -= OSC_SIZE;
		}
		if (diff_phase += 41 > OSC_SIZE) {
			diff_phase -= OSC_SIZE;
		}
	}
	p_phi[0] = phi_sum[sizeof(phi_sum-1)];
	p_phi[1] = phi_diff[sizeof(phi_diff-1)];
	p_phase[0] = sum_phase;
	p_phase[1] = diff_phase;
	*p_count = count;
}
