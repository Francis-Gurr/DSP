/***************
 * DSP Group 2
 * Francis Gurr
 * Lauren Miller
 * Vicky Miles
 * *************/

#include <stdio.h>
#include "structs.h"
#include "demodulator.h"
#include<time.h>
#include<stdlib.h>
#include"init.h"

/* TIMINGS */
double t_total = 0;
clock_t start;
clock_t finish;

float batch_double[L2];
double batch_in[L];

double left_max = 0;
double right_max = 0;

double sum[L];
double diff[L];
double sum_down[L_2];
double diff_down[L_2];

double left[SIZE_OUT];
double right[SIZE_OUT];

/* FIR FILTER */
double buff_fir1_sum[M1] = {0};
double buff_fir1_diff[M1] ={0};
int offset1 = 0;
struct Filter f1 = {.SIZE_IN=L, .SIZE_FILTER=M1, .offset=0};
double buff_fir2_sum[M2] = {0};
double buff_fir2_diff[M2] ={0};
struct Filter f2 = {.SIZE_IN=L_2, .SIZE_FILTER=M2, .offset=0};
int offset2 = 0;

/* DEMODULATOR */
double phi[2] = {0};
int sum_phase;
int diff_phase;

/* RESAMPLE */
double buff_res_sum[M_RES] = {0};
double buff_res_diff[M_RES] = {0};
struct Buffer buff_params = {.offset=0, .wait=4, .curr_filter=0};
int offset_res = 0;
int curr_filter = 0;
int wait_res = 4;

int index1 = 0;
/*** PROCESS BATCH ***/
static void process_batch(double *p_batch_in, int demod_type) {
	
	for (int i = 0; i < L; i++) {
		/* COHERENT DEMODULATOR */
		double val_in = (p_batch_in[i]);
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
		/* FIR 1 */
		if (i%25 == 0) {
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
			buff_fir2_sum[offset2] = sum_fir1;
			buff_fir2_diff[offset2] = diff_fir1;

			/* FIR 2 */
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
				left[index1] = ((sum_res - diff_res) / 2);
				right[index1] =((sum_res + diff_res) / 2);
				double a = left[index1];
				if (a > left_max) {
					left_max = a;
				}
				//if ((a*-1) > left_max) {
				//	left_max = (a*-1);
				//}
				a = right[index1];
				if (a > right_max) {
					right_max = a;
				}
				//if ((a*-1) > right_max) {
			//		right_max = (a*-1);
		//		}
				// Increment offset 1
				if (++index1 >= SIZE_OUT) {
					index1 = 0;
				}
			}
			// Increment offset 1
			if (++offset_res >= M_RES) {
				offset_res = 0;
			}
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
	const char *p_FILE_RIGHT = argv[2];
	const char *p_FILE_LEFT = argv[3];
	
	FILE *fd_in = fopen(p_FILE_IN, "rb");
	const char *temp_left = "l_temp.dat";
	const char *temp_right = "r_temp.dat";
	FILE *fd_left1 = fopen(temp_left, "ab");
	FILE *fd_right1 = fopen(temp_right, "ab");

	/* FIND FIRST NON-ZERO BATCH */
	int all_zeros = 1;
	int exit = 0;
	int i_zeros = 0;
	int num_zeros = 0;
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
	int extra = i_zeros;
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

	/* FIRST BATCH */
	demod_costas(batch_in, sum, diff, phi);
	if (phi[0] < 0) {
	       phi[0] = -phi[0];
	}
	if (phi[1] < 0) {
	       phi[1] = -phi[1];
	}
	sum_phase = (int)(phi[0] * 31.83098862);
	diff_phase = (int)(phi[1] * 31.83098862); 
	process_batch(batch_in, 1);
	// Write
	fwrite(left, 8, SIZE_OUT, fd_left1);
	fwrite(right, 8, SIZE_OUT, fd_right1);

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
			batch_in[i] = (double) batch_double[i*2];
		}

		process_batch(batch_in, 1);

		// Write
		fwrite(left, 8, SIZE_OUT, fd_left1);
		fwrite(right, 8, SIZE_OUT, fd_right1);
	}

	fclose(fd_in);
	fclose(fd_left1);
	fclose(fd_right1);
	
	/* NORMALISE */
	double left_factor = 1/left_max;
	double right_factor = 1/right_max;
	printf("left_fact: %f\n", left_factor);
	printf("r_fact: %f\n", right_factor);
	FILE *fd_l_temp = fopen(temp_left, "rb");
	FILE *fd_r_temp = fopen(temp_right, "rb");
	FILE *fd_left = fopen(p_FILE_LEFT, "ab");
	FILE *fd_right = fopen(p_FILE_RIGHT, "ab");
	exit = 0;
	int sz = 0;
	double left_in[1024] = {0};
	double right_in[1024] = {0};
	float left_out[1024] = {0};
	float right_out[1024] = {0};

	while (exit == 0) {
		sz = fread(left_in, 8, 1024, fd_l_temp);
		sz = fread(right_in, 8, 1024, fd_r_temp);
		for (int i = 0; i < 1024; i++) {
			left_out[i] = (float)( left_in[i]*left_factor);
			right_out[i] = (float)(right_in[i]*right_factor);
		}
		fwrite(left_out, 4, sz, fd_left);
		fwrite(right_out, 4, sz, fd_right);
		if (sz < 1024) { 
			exit = 1;
		}
	}
	fclose(fd_l_temp);
	fclose(fd_r_temp);
	fclose(fd_left);
	fclose(fd_right);	
	remove("l_temp.dat");
	remove("r_temp.dat");

	finish = clock();
	t_total = (double)(finish-start) / CLOCKS_PER_SEC;
	printf("Total: %f\n", t_total);
	printf("\nFIN.\n");
	return 0;
}
