#include <stdio.h>
#include<stdbool.h>
#include "structs.h"
#include "io.h"
#include "fir.h"
//#include "fir_fft.h"
#include "demodulator.h"
#include "resample.h"
#include<time.h>

/* TIMINGS */
double t_total = 0;
clock_t start;
clock_t finish;

/* IN, SUM, DIFF, OUT */
float batch_in[L] = {0};
double val_in = 0;
double sum[L_2] = {0};
double diff[L_2] = {0};
double sum_down[L_2] = {0};
double diff_down[L_2] = {0};
double left[SIZE_OUT] = {0};
double right[SIZE_OUT] = {0};

/* FIR FILTER */
double buff_fir1_sum[M1] = {0};
double buff_fir1_diff[M1] = {0};
int wait1 = 1;
int offset1 = 0;
int index1 = 0;
double buff_fir2_sum[M2] = {0};
double buff_fir2_diff[M2] ={0};
struct Filter f2 = {.SIZE_IN=L_2, .SIZE_FILTER=M2, .offset=0};

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
struct Buffer buff_params = {.offset=0, .wait=4, .curr_filter=0};

const char *p_left_norm = "left_norm.dat";
const char *p_right_norm = "right_norm.dat";

/*** PROCESS BATCH ***/
void process_batch(float *p_batch_in) {
	
	for (int i = 0; i < L; i++) {
		/* COHERENT DEMODULATOR */
		val_in = (double)(p_batch_in[i]);
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
			double sum_fir = 0;
			double diff_fir = 0;
			for (int j = offset1; j >= 0; j--) {
				sum_fir += buff_fir1_sum[j] * H_LOW_1[offset1-j];
				diff_fir += buff_fir1_diff[j] * H_LOW_1[offset1-j];
			}
			int i_H =  offset1 + M1;
			for (int j = M1-1; j > offset1; j--) {
				sum_fir += buff_fir1_sum[j] * H_LOW_1[i_H-j];
				diff_fir += buff_fir1_diff[j] * H_LOW_1[i_H-j];
			}
			if (++offset1 >= M1) {
				offset1 = 0;
			}
			sum[index1] = sum_fir;
			diff[index1] = diff_fir;
			wait1 = 25;
  			index1++;
		}
	}
	
	/* FIR 2 */
	fir(sum_down, diff_down, buff_fir2_sum, buff_fir2_diff, H_LOW_2, &f2);

	/*RESAMPLE */
	resample(sum_down, diff_down, buff_res_sum, buff_res_diff, &buff_params, left, right);
}

/***** MAIN *****/
int main(int argc, char *argv[]) {
	start = clock();

	/* FILE PATHS */
	const char *p_FILE_IN = argv[1];
	const char *p_FILE_LEFT = argv[2];
	const char *p_FILE_RIGHT = argv[3];
	
	FILE *fd_in = fopen(p_FILE_IN, "rb");

	/* FIND FIRST NON-ZERO BATCH */
	int all_zeros = 1;
	int exit = 0;
	while (all_zeros == 1) {
		// Read
		read_batch(fd_in, batch_in, &exit);
		// Check for non-zeros
		check_zeros(batch_in, fd_in, &all_zeros);
	}

    /* PROCESS FIRST BATCH */
    // Do costas to get phase shift
    demod_costas(batch_in, sum, diff, phi, &count);
	phase[0] = (count * 40) % 200;
	phase[1] = (count * 42) % 200;
    // Process the batch fully
	process_batch(batch_in);
	// Write
	write_batch(p_FILE_LEFT, SIZE_OUT, left);
	write_batch(p_FILE_RIGHT, SIZE_OUT, right);

	while (exit == 0) {
		// Read
		read_batch(fd_in, batch_in, &exit);
		// Process the batch
		process_batch(batch_in);
		// Write
		write_batch(p_FILE_LEFT, SIZE_OUT, left);
		write_batch(p_FILE_RIGHT, SIZE_OUT, right);
 	}
	fclose(fd_in);
	
	finish = clock();
	t_total = (double)(finish-start) / CLOCKS_PER_SEC;
	printf("Time taken: %f\n", t_total);
	printf("\nFIN.\n");
	return 0;
}
