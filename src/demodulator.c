#include "h/init.h"
#include "demodulator.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void demod_coherent(float *p_in, double *p_sum, double *p_diff, double *p_phi, int *p_count){
	const double sum_phi = p_phi[0];
	const double diff_phi = p_phi[1];
	int count = *p_count;
	for (int i = 0; i < L; i++) {

		double val_in = (double)(p_in[i]);

		// SUM
		p_sum[i] = val_in * cos(SUM_FREQ*count + sum_phi);
		// DIFF 
		p_diff[i] = val_in * cos(DIFF_FREQ*count + diff_phi);
		count++;
	}
	*p_count = count;
}

void demod_costas(float *p_in, double *p_sum, double *p_diff, double *p_phi, int *p_count) {

	double phi_sum[L] = {0}, phi_diff[L] = {0};
	double I_sum[L] = {0}, I_diff[L] = {0};
	double Q_sum[L] = {0}, Q_diff[L] = {0};
	double out_sum[L] = {0}, out_diff[L] = {0};
	double phase_sum[L] = {0}, phase_diff[L] = {0};
	int count = *p_count;

	for (int i = 0; i < L; i++) {
		double in = (double)(p_in[i]);
		if (i > 0) {
			if (out_sum[i-1] * phase_sum[i-1] >= 0) {
				phi_sum[i] = phi_sum[i-1] - (PHI_INC);
				phi_diff[i] = phi_diff[i-1] - (PHI_INC);
			}
			else {
				phi_sum[i] = phi_sum[i-1] + (PHI_INC);
				phi_diff[i] = phi_diff[i-1] + (PHI_INC);
			}
		}
		else {
			phi_sum[i] = p_phi[0];
			phi_diff[i] = p_phi[1];
		}

		I_sum[i] = in * cos(SUM_FREQ*count + phi_sum[i]);
		I_diff[i] = in * cos(DIFF_FREQ*count + phi_diff[i]);
		Q_sum[i] = in * sin(SUM_FREQ*count + phi_sum[i]);
		Q_diff[i] = in * sin(DIFF_FREQ*count + phi_diff[i]);
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
	}
	p_phi[0] = phi_sum[sizeof(phi_sum-1)];
	p_phi[1] = phi_diff[sizeof(phi_diff-1)];
	*p_count = count;
}
