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
double t_zeros = 0;
double t_first_batch = 0;
double t_other_batches = 0;
double t_fir = 0;
double t_demod[2] = {0};
double t_res = 0;
double t_write = 0;
clock_t begin;
clock_t end;
clock_t start;
clock_t finish;

float batch_in[L];

double sum[L];
double diff[L];

double left[SIZE_OUT];
double right[SIZE_OUT];

/* FIR FILTER */
//double buff_fir_sum[M-1] = {0};
//double buff_fir_diff[M-1] = {0};
// Time domain
double buff_fir_sum[M] = {0};
double buff_fir_diff[M] ={0};
int offset = 0;

/* DEMODULATOR */
void (*demodulators[2])(float *, double *, double *, int *, int *, int *) = {demod_costas_2, demod_coherent};
int phase[2] = {0};
int phi[2] = {0};
int count = 0;
const char *p_FILE_SUM = "s.dat";
const char *p_FILE_DIFF = "d.dat";

/* RESAMPLE */
double buff_res_sum[M_RES] = {0};
double buff_res_diff[M_RES] = {0};
struct Buffer buff_params = {.offset=0, .wait=105, .curr_filter=0};

/*** PROCESS BATCH ***/
void process_batch(float *p_batch_in, int demod_type) {
	
	/* DEMODULATE */
	begin = clock();

	(*demodulators[demod_type])(p_batch_in, sum, diff, phase, phi, &count);
	end = clock();
	t_demod[demod_type] += (double)(end-begin) / CLOCKS_PER_SEC;

	/* FIR */
	begin = clock();
	//fir_fft(sum, diff, buff_fir_sum, buff_fir_diff);
	fir(sum, diff, buff_fir_sum, buff_fir_diff, &offset);
	end = clock();
	t_fir += (double)(end-begin) / CLOCKS_PER_SEC;

	write_batch(p_FILE_SUM, L, sum);
	write_batch(p_FILE_DIFF, L, diff);

	/*RESAMPLE */
	begin = clock();
	resample(sum, diff, buff_res_sum, buff_res_diff, &buff_params, left, right);
	end = clock();
	t_res += (double)(end-begin) / CLOCKS_PER_SEC;
}

/***** MAIN *****/
int main(int argc, char *argv[]) {
	start = clock();
	printf("N = %d, M = %d, L = %d, L + M - 1 = %d\n", N, M , L, L+M-1);

	/* FILE PATHS */
	const char *p_FILE_IN = argv[1];
	const char *p_FILE_LEFT = argv[2];
	const char *p_FILE_RIGHT = argv[3];
	
	FILE *fd = fopen(p_FILE_IN, "rb");

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
		//check_zeros(batch_in, fd, &all_zeros);
		all_zeros = 0;
	}
	end = clock();
	t_zeros = (double)((end-begin) / CLOCKS_PER_SEC) / zero_batches;

	/* FIRST BATCH */
	begin = clock();
	//demod_costas_2(batch_in, sum, diff, phase, phi, &count);
	//count = 0;
	process_batch(batch_in, 1);
	end = clock();
	t_first_batch = (double)(end-begin) / CLOCKS_PER_SEC;
	// Write
	write_batch(p_FILE_LEFT, SIZE_OUT, left);
	write_batch(p_FILE_RIGHT, SIZE_OUT, right);

	int other_batches = 0;
	while (exit == 0) {
		// Read
		read_batch(fd, batch_in, &exit);
		
		begin = clock();
		process_batch(batch_in, 1);
		end = clock();
		t_other_batches += (double)(end-begin) / CLOCKS_PER_SEC;
		other_batches++;

		// Write
		write_batch(p_FILE_LEFT, SIZE_OUT, left);
		write_batch(p_FILE_RIGHT, SIZE_OUT, right);
	}

	t_other_batches = t_other_batches/other_batches;
	fclose(fd);

	finish = clock();
	t_total = (double)(finish-start) / CLOCKS_PER_SEC;
	printf("Total: %f, Zeros: %f, FIR: %f, First Batch: %f\n, Other batches: %f\n", t_total, t_zeros, t_fir, t_first_batch, t_other_batches);
	printf("Costas: %f, Coherent: %f, Resample: %f\n", t_demod[0], t_demod[1], t_res);
	printf("\nFIN.\n");
	return 0;
}
