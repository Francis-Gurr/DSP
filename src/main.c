#include <stdio.h>
#include "structs.h"
#include "h/init.h"
#include "math.h"
#include<time.h>

/* TIMINGS */
double t_total = 0;
clock_t start;
clock_t finish;

/* IN, SUM, DIFF, OUT */
//float batch_in[L] = {0};
double val_in = 0;
double sum[L] = {0};
double diff[L] = {0};
double sum_down[L_2] = {0};
double diff_down[L_2] = {0};
float left[SIZE_OUT] = {0};
float right[SIZE_OUT] = {0};
int i_out = 0;

/* FIR FILTER */
double buff_fir1_sum[M1] = {0};
double buff_fir1_diff[M1] = {0};
int wait1 = 1;
int offset1 = 0;
double buff_fir2_sum[M2] = {0};
double buff_fir2_diff[M2] ={0};
int offset2 = 0;

/* COHERENT DEMODULATOR */
double sum_phi = 0;
double diff_phi = 0;
int sum_phase = 0;
int diff_phase = 0;

double phi[2] = {0};
int count = 0;
int phase[2];

/* RESAMPLE */
double buff_res_sum[M_RES] = {0};
double buff_res_diff[M_RES] = {0};
int offset_res = 0;
int wait_res = 4;
int curr_filter = 0;

const char *p_left_norm = "left_norm.dat";
const char *p_right_norm = "right_norm.dat";

/*** PROCESS BATCH ***/
void process_batch(double *p_batch_in, FILE *fd_left, FILE *fd_right) {
	int index1 = 0;
	for (int i = 0; i < L; i++) {
		/* COHERENT DEMODULATOR */
		val_in = p_batch_in[i];
		// SUM
		buff_fir1_sum[offset1] = val_in * OSC[sum_phase]; // Add to buffer
		// DIFF 
		buff_fir1_diff[offset1] = val_in * OSC[diff_phase]; // Add to buffer
		if((sum_phase += 40) >= 200) {
			sum_phase -= 200;
		}
		if ((diff_phase += 42) >= 200) {
			diff_phase -= 200;
		}
		wait1--;
		
		/* FIR 1 */
		if (wait1 == 0) {
			double sum_fir1 = 0;
			double diff_fir1 = 0;
			for (int j = offset1; j >= 0; j--) {
				sum_fir1 += buff_fir1_sum[j] * H_LOW_1[offset1-j];
				diff_fir1 += buff_fir1_diff[j] * H_LOW_1[offset1-j];
			}
			int i_H =  offset1 + M1;
			for (int j = M1-1; j > offset1; j--) {
				sum_fir1 += buff_fir1_sum[j] * H_LOW_1[i_H-j];
				diff_fir1 += buff_fir1_diff[j] * H_LOW_1[i_H-j];
			}

			/* FIR 2 */
			buff_fir2_sum[offset2] = sum_fir1; // Add to buffer
			buff_fir2_diff[offset2] = diff_fir1; // Add to buffer
			double sum_fir2 = 0;
			double diff_fir2 = 0;
			for (int j = offset2; j >= 0; j--) {
				sum_fir2 += buff_fir2_sum[j] * H_LOW_2[offset2-j];
				diff_fir2 += buff_fir2_diff[j] * H_LOW_2[offset2-j];
			}
			i_H =  offset2 + M2;
			for (int j = M2-1; j > offset2; j--) {
				sum_fir2 += buff_fir2_sum[j] * H_LOW_2[i_H-j];
				diff_fir2 += buff_fir2_diff[j] * H_LOW_2[i_H-j];
			}
			buff_res_sum[offset_res] = sum_fir2;
			buff_res_diff[offset_res] = diff_fir2;
			// Increment offset 2
			if (++offset2 >= M2) {
				offset2 = 0;
			}

			/* RESAMPLE */
			wait_res--;
			if (wait_res == 0) {
				double sum_res = 0;
				double diff_res = 0;
				for (int j = offset_res; j >= 0; j--) {
					sum_res += buff_res_sum[j] * H_RES[curr_filter][offset_res-j];
					diff_res += buff_res_diff[j] * H_RES[curr_filter][offset_res-j];
				}
				i_H =  offset_res + M_RES;
				for (int j = M_RES-1; j > offset_res; j--) {
					sum_res += buff_res_sum[j] * H_RES[curr_filter][i_H-j];
					diff_res += buff_res_diff[j] * H_RES[curr_filter][i_H-j];
				}
				// Increment current filter and buffer wait value 
				if (++curr_filter > 5) {
					curr_filter = 0;
				}
				if (curr_filter == 5) {
					wait_res = 5;
				}
				else {
					wait_res = 4;
				}

				/* GET LEFT AND RIGHT */
				left[i_out] = (float)((sum_res - diff_res) / 2);
				right[i_out] = (float)((sum_res + diff_res) / 2);
				
				/* WRITE TO FILE */
				if (i_out++ >= SIZE_OUT) {
					fwrite(left, 4, SIZE_OUT, fd_left);
					fwrite(right, 4, SIZE_OUT, fd_right);
					i_out = 0;
				}
			}
			// Increment offset res
			if (++offset_res >= M_RES) {
				offset_res = 0;
			} 

			wait1 = 25;
			index1++;
		}
		// Increment offset 1
		if (++offset1 >= M1) {
			offset1 = 0;
		}
	}
}

/***** MAIN *****/
int main(int argc, char *argv[]) {
	start = clock();

	/* FILE PATHS */
	const char *p_FILE_IN = argv[1];
	const char *p_FILE_LEFT = argv[2];
	const char *p_FILE_RIGHT = argv[3];
	
	FILE *fd_in = fopen(p_FILE_IN, "rb");
	FILE *fd_left = fopen(p_FILE_LEFT, "ab");
	FILE *fd_right = fopen(p_FILE_RIGHT, "ab");

	/* FIND FIRST NON-ZERO BATCH */
	int all_zeros = 1;
	int num_zeros = 0;
	int i_zeros = 0;
	int exit = 0;
	double batch_in[L] = {0};
	float batch_double[L2] = {0};
	while (all_zeros == 1) {
		// Read
		fread(batch_double, 4, L2, fd_in);
		// Decimate by two
		for (int i = 0; i < L2; i++) {
			if (batch_double[i] != 0) {
			       all_zeros = 0;
			       i_zeros = i;
			       break;
			}		
			else {
				num_zeros++;
			}
		}
	}
	// Write num_zeros*3/625 to output file
	
	// Create first non-zero batch
	int extra = i_zeros-1;
	float batch_extra[extra];
	fread(batch_extra, 4, extra, fd_in);
	int i_in = 0;
	for (int i = i_zeros; i < L2; i += 2){
		batch_in[i_in] = (double)batch_double[i];
		i_in++;
	}
	for (int i = 0; i < extra; i += 2) {
		batch_in[i_in] = (double)batch_extra[i];
		i_in++;
	}
	
	/* COSTAS */
	double sum_costas[L] = {0};
	double diff_costas[L] = {0};
	double phi_sum[L] = {0}, phi_diff[L] = {0};
	double I_sum[L] = {0}, I_diff[L] = {0};
	double Q_sum[L] = {0}, Q_diff[L] = {0};
	double out_sum[L] = {0}, out_diff[L] = {0};
	double phase_sum[L] = {0}, phase_diff[L] = {0};
	for (int i = 0; i < L; i++) {
		double in = batch_in[i];
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
			phi_sum[i] = phi[0];
			phi_diff[i] = phi[1];
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
		sum_costas[i] = out_sum[i];
		diff_costas[i] = out_diff[i];
	}
	sum_phase = (count * 40) % 200;
	diff_phase = (count * 42) % 200;

	/* PROCESS BATCHES */	
	process_batch(batch_in, fd_left, fd_right);
	while (exit == 0) {
		// Read
		float batch_double[L2] = {0};
		int sz = fread(batch_double, 4, L2, fd_in);
		// Pad if didn't read full file and update exit flag
		if (sz < L2) {
			exit = 1;
			for (int i = sz; i < L2; i++) {
				batch_double[i] = 0;
			}
		}
		for (int i = 0; i < L; i++) {
			batch_in[i] = batch_double[i*2];
		}
		// Process the batch
		process_batch(batch_in, fd_left, fd_right);
 	}
	// Write leftovers
	if (i_out != 0) {
		fwrite(left, 4, i_out, fd_left);
		fwrite(right, 4, i_out, fd_right);

	}
	fclose(fd_in);
	fclose(fd_left);
	fclose(fd_right);
	
	finish = clock();
	t_total = (double)(finish-start) / CLOCKS_PER_SEC;
	printf("Time taken: %f\n", t_total);
	printf("\nFIN.\n");
	return 0;
}
