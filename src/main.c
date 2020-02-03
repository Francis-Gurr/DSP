#include <stdio.h>
#include<stdbool.h>
#include "structs.h"
#include "io.h"
#include "fir_fft.h"
//#include "demodulator.h"
#include "get_lr.h"
#include<time.h>

#define SIZE_RES 500 
#define SIZE_WRITE 3


/* FIR FILTER */
double buff_fir_sum[M-1] = {0};
double buff_fir_diff[M-1] = {0};

/* DEMODULATOR */
typedef demod(double *p_in, int *phase, int sum_or_diff);
demod = demod_costas;
int phase[2] = {0};

/***** MAIN *****/
int main(int argc, char *argv[]) {

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
	float batch_right[L];
	float batch_left[L];

	/* FIND FIRST NON-ZERO BATCH */
	int all_zeros = 1;
	while (all_zeros == 0) {
		// Read
		read_batch(fd, L, batch_in, &exit);

		// Check for non-zeros
		check_zeros(batch_in, fd, &all_zeros);
	}

	/* FIRST BATCH */
	process_batch(batch_in, batch_out);
	// Write
	write_batch(p_FILE_LEFT, L, batch_left);
	write_batch(p_FILE_RIGHT, L, batch_right);
	free(batch_in);
	free(batch_left);
	free(batch_right);

	// Change demodulator
	demod = demod_coherent;

	int exit = 0;
	while (exit == 0) {
		// Read
		read_batch(fd, L, batch_in, &exit);

		process_batch(batch_in, batch_left, batch_right);

		// Write
		write_batch(p_FILE_LEFT, L, batch_left);
		write_batch(p_FILE_RIGHT, L, batch_right);
		free(batch_in);
		free(batch_left);
	}

	fclose(fd);
	printf("Read: %f, FIR: %f, Demod: %f\n", t_read, t_fir, t_demod);
	printf("FIN.");
	return 0;
}

void process_batch(p_batch_in, p_batch_left, p_batch_right) {
	
	/* TIMINGS */
	double t_read;
	double t_fir;
	double t_demod;
	double t_write;
	clock_t begin;
	clock_t end;

	/* FIR */
	begin = clock();
	double sum[N] = {0};
	double diff[N] = {0};
	fir_fft(p_batch_in, sum, 0, buff_fir_sum);
	fir_fft(p_batch_in, diff, 1, buff_fir_diff);
	end = clock();
	t_fir += (double)(end-begin) / CLOCKS_PER_SEC;

	/* LOOP THROUGH BATCH */
	for (i = 0; i < L; i++) {
		/* DEMODULATE */
		begin = clock();
		demod(p_sum, L, &sum_osc);
		demod(p_diff, L, &diff_osc);
		end = clock();
		t_demod += (double)(end-begin) / CLOCKS_PER_SEC;

		/* RESAMPLE */
/*		float sum_res[SIZE_READ] = {0};
		float diff_res[SIZE_READ] = {0};
		batch_size_res = resample(sum, batch_size, sum_res, &filter, &buff_res, &buff_dec);
		batch_size_res = resample(diff, batch_size, diff_res, &filter, &buff_res, &buff_dec);
		float left[SIZE_WRITE] = {0};
		float right[SIZE_WRITE];
		get_lr(sum_res, diff_res, left, right, batch_size_res);
*/
		p_batch_left[i] = *sum;
		p_batch_right[i] = *diff;
		sum++;
		diff++;
	}
}
