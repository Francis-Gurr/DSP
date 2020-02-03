#include <stdio.h>
#include<stdbool.h>
#include "structs.h"
#include "io.h"
#include "fir_fft.h"
#include "demodulator.h"
#include "get_lr.h"
#include<time.h>

#define SIZE_RES 500 
#define SIZE_WRITE 3

/* TIMINGS */
double t_total = 0;
double t_zeros = 0;
double t_first_batch = 0;
double t_other_batches = 0;
double t_fir = 0;
double t_demod[2] = {0};
double t_write = 0;
clock_t begin;
clock_t end;
clock_t start;
clock_t finish;

/* FIR FILTER */
double buff_fir_sum[M-1] = {0};
double buff_fir_diff[M-1] = {0};

/* DEMODULATOR */
//void (*demodulators[2])(double *,double *, int *) = {demod_costas, demod_coherent};
int phase[2] = {0};

/*** PROCESS BATCH ***/
void process_batch(float *p_batch_in, double *sum, double *diff, int demod_type) {
	

	/* FIR */
	begin = clock();
	//double sum[N] = {0};
	//double diff[N] = {0};
	fir_fft(p_batch_in, sum, 0, buff_fir_sum);
	fir_fft(p_batch_in, diff, 1, buff_fir_diff);
	end = clock();
	t_fir += (double)(end-begin) / CLOCKS_PER_SEC;
	begin = clock();
	demod_coherent(sum, diff, phase);
	end = clock();
	t_demod[demod_type] += (double)(end-begin) / CLOCKS_PER_SEC;
	/* LOOP THROUGH BATCH */
//	for (int i = 0; i < L; i++) {
//		/* DEMODULATE */
//		if(demod_type == 1) {
//		begin = clock();
//		//coherent
		
		// calculate output
//		*p_sum = *p_sum * OSC[phase[0]];
//		*p_diff = *p_diff * OSC[phase[1]];
	
		// update phase value
//		phase[0] = (phase[0] + 20) % 200;
//		phase[1] = (phase[1] + 21) % 200;
//		end = clock();
//		t_demod[1] += (double)(end-begin) / CLOCKS_PER_SEC;
//		}
//		else {
//			demod_costas(p_sum, phase, 20);
//			demod_costas(p_diff, phase+1, 21);
//		}
		//(*demodulators[demod_type])(p_sum, phase, 20);
		//(*demodulators[demod_type])(p_diff, phase+1, 21);

		/* RESAMPLE */
/*		float sum_res[SIZE_READ] = {0};
		float diff_res[SIZE_READ] = {0};
		batch_size_res = resample(sum, batch_size, sum_res, &filter, &buff_res, &buff_dec);
		batch_size_res = resample(diff, batch_size, diff_res, &filter, &buff_res, &buff_dec);
*/
		/* GET LEFT AND RIGHT */
/*		float left[SIZE_WRITE] = {0};
		float right[SIZE_WRITE];
		get_lr(sum_res, diff_res, left, right, batch_size_res);
*/
//		p_batch_left[i] = *p_sum;
//		p_batch_right[i] = *p_diff;
//		p_sum++;
//		p_diff++;
//	}
}

/***** MAIN *****/
int main(int argc, char *argv[]) {
	start = clock();

	/* FILE PATHS */
	const char *p_FILE_IN = argv[1];
	const char *p_FILE_LEFT = argv[2];
	const char *p_FILE_RIGHT = argv[3];
	

	/* RESAMPLE 
	struct Filter filter = {
		.SIZE = SIZE_RES,
		.p_H = {H0,H1,H2},
		.curr_res_filter = 0,
		.curr_dec_filter = 0
	};
	struct Buffer buff_res = {.SIZE=SIZE_RES, .values={0}, .offset=0, .wait=1}; // Resample buffer
	struct Buffer buff_dec = {.SIZE=SIZE_DEC, .values={0}, .offset=0, .wait=1}; // Decimation buffer
	*/

	FILE *fd = fopen(p_FILE_IN, "rb");
	float batch_in[L];
	double batch_left[L];
	double batch_right[L];

	/* FIND FIRST NON-ZERO BATCH */
	begin = clock();
	int all_zeros = 1;
	int exit = 0;
	int zero_batches = 0;
	while (all_zeros == 1) {
		// Read
		zero_batches++;
		read_batch(fd, batch_in, &exit);

		// Check for non-zeros
		check_zeros(batch_in, fd, &all_zeros);
	}
	end = clock();
	t_zeros = (double)((end-begin) / CLOCKS_PER_SEC) / zero_batches;

	/* FIRST BATCH */
	begin = clock();
	process_batch(batch_in, batch_left, batch_right, 0);
	end = clock();
	t_first_batch = (double)(end-begin) / CLOCKS_PER_SEC;
	// Write
	write_batch(p_FILE_LEFT, L, batch_left);
	write_batch(p_FILE_RIGHT, L, batch_right);

	int other_batches = 0;
	while (exit == 0) {
		// Read
		read_batch(fd, batch_in, &exit);
		
		begin = clock();
		process_batch(batch_in, batch_left, batch_right, 1);
		end = clock();
		t_other_batches += (double)(end-begin) / CLOCKS_PER_SEC;
		other_batches++;

		// Write
		write_batch(p_FILE_LEFT, L, batch_left);
		write_batch(p_FILE_RIGHT, L, batch_right);
	}

	t_other_batches = t_other_batches/other_batches;
	fclose(fd);

	finish = clock();
	t_total = (double)(finish-start) / CLOCKS_PER_SEC;
	printf("Total: %f, Zeros: %f, FIR: %f, First Batch: %f\n, Other batches: %f\n", t_total, t_zeros, t_fir, t_first_batch, t_other_batches);
	printf("Costas: %f, Coherent: %f", t_demod[0], t_demod[1]);
	printf("FIN.");
	return 0;
}
