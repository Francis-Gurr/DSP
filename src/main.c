#include <stdio.h>
#include<stdbool.h>
#include "structs.h"
#include "h/init_600.h"
#include "io.h"
#include "fir.h"
#include "demodulator.h"
#include "resample.h"
#include "get_lr.h"
#include "test/csv.h"
#include<time.h>

#define SIZE_READ 625
#define SIZE_FIR 600
#define SIZE_RES 500 
#define SIZE_WRITE 3

int main(int argc, char *argv[]) {
	/****************************************************************************
	 * START OF DECLARATIONS
	 *****************************************************************************/
	printf("START\n");

	/* FILE PATHS */
	printf("Reading arguments...\n");
	const char *p_FILE_IN = argv[1];
	const char *p_FILE_LEFT = argv[2];
	const char *p_FILE_RIGHT = argv[3];
	printf("Arguments read!\n");
	
	/* FIR FILTER */
	double H_SUM[SIZE_FIR] = {0};
	double H_DIFF[SIZE_FIR] = {0};
	get_H_SUM(H_SUM);
	get_H_DIFF(H_DIFF);
	struct Buffer buff_fir_sum = {.SIZE=SIZE_FIR, .values={0}, .offset=0, .wait=0};
	struct Buffer buff_fir_diff = {.SIZE=SIZE_FIR, .values={0}, .offset=0, .wait=0};
	
	/* DEMODULATION */
	const double SUM_OSC[5] = {1.000000,0.809017,0.309017,-0.309017,-0.809017};
	const double DIFF_OSC[100] = {
		1.000000,0.790155,0.248690,-0.397148,-0.876307,-0.987688,-0.684547,-0.094108,0.535827,0.940881,
		0.951057,0.562083,-0.062791,-0.661312,-0.982287,-0.891007,-0.425779,0.218143,0.770513,0.999507,
		0.809017,0.278991,-0.368125,-0.860742,-0.992115,-0.707107,-0.125333,0.509041,0.929776,0.960294,
		0.587785,-0.031411,-0.637424,-0.975917,-0.904827,-0.453990,0.187381,0.750111,0.998027,0.827081,
		0.309017,-0.338738,-0.844328,-0.995562,-0.728969,-0.156434,0.481754,0.917755,0.968583,0.612907,
		0.000000,-0.612907,-0.968583,-0.917755,-0.481754,0.156434,0.728969,0.995562,0.844328,0.338738,
		-0.309017,-0.827081,-0.998027,-0.750111,-0.187381,0.453990,0.904827,0.975917,0.637424,0.031411,
		-0.587785,-0.960294,-0.929776,-0.509041,0.125333,0.707107,0.992115,0.860742,0.368125,-0.278991,
		-0.809017,-0.999507,-0.770513,-0.218143,0.425779,0.891007,0.982287,0.661312,0.062791,-0.562083,
		-0.951057,-0.940881,-0.535827,0.094108,0.684547,0.987688,0.876307,0.397148,-0.248690,-0.790155};
	struct Demod sum_osc = {.SIZE=5, .p_OSC=SUM_OSC, .index=0, .inverse=0};
	struct Demod diff_osc = {.SIZE=100, .p_OSC=DIFF_OSC, .index=0, .inverse=0};	

	/* RESAMPLE */
/*	double H0[SIZE_RES] = {0};
	double H1[SIZE_RES] = {0};
	double H2[SIZE_RES] = {0};
	get_H_RES(H0, H1, H2);
	struct Filter filter = {
		.SIZE = SIZE_RES,
		.p_H = {H0,H1,H2},
		.curr_res_filter = 0,
		.curr_dec_filter = 0
	};
	struct Buffer buff_res = {.SIZE=SIZE_RES, .values={0}, .offset=0, .wait=1}; // Resample buffer
	struct Buffer buff_dec = {.SIZE=SIZE_DEC, .values={0}, .offset=0, .wait=1}; // Decimation buffer
*/
	/****************************************************************************
	* END OF DECLARATIONS
	*****************************************************************************/

	int exit = 0;
	FILE *fd = fopen(p_FILE_IN, "rb");
	const char *p_FILE_SUM = "src/test/demod_sum.dat";
	const char *p_FILE_DIFF = "src/test/demod_diff.dat";
	double t_read;
	double t_fir;
	double t_demod;
	double t_write;
	clock_t begin;
	clock_t end;
	while (exit == 0) {
		// Read n=SIZE_READ samples from FILE_IN
		// Return a pointer to the first element in the batch and the batch size
		// begin = clock();
		// float batch[SIZE_READ] = {0};
		// read_batch(fd, SIZE_READ, batch, &exit);
		// end = clock();
		// t_read += (double)(end-begin) / CLOCKS_PER_SEC;

		// Use FIR filter to split the batch into sum and diff
		// Return a pointer to the first element in the sum and diff array
		begin = clock();
		double sum[SIZE_READ] = {0};
		double diff[SIZE_READ] = {0};
		fir(fd, sum, SIZE_READ, 0, &buff_fir_sum);
		fir(fd, diff, SIZE_READ, 1, &buff_fir_diff);
		end = clock();
		t_fir += (double)(end-begin) / CLOCKS_PER_SEC;
	
		// Demodulate sum and diff
		// printf("Entering demodulator...\n");
		begin = clock();
		demod(sum, SIZE_READ, &sum_osc);
		demod(diff, SIZE_READ, &diff_osc);
		end = clock();
		t_demod += (double)(end-begin) / CLOCKS_PER_SEC;
		
//		begin = clock();
//		write_batch(p_FILE_SUM, SIZE_READ, sum);
//		write_batch(p_FILE_DIFF, SIZE_READ, diff);
//		end = clock();
//		t_write += (double)(end-begin) / CLOCKS_PER_SEC;
		
		/*
		// Resample sum and diff
		// printf("Starting resample...\n");
		float sum_res[SIZE_READ] = {0};
		float diff_res[SIZE_READ] = {0};
		batch_size_res = resample(sum, batch_size, sum_res, &filter, &buff_res, &buff_dec);
		batch_size_res = resample(diff, batch_size, diff_res, &filter, &buff_res, &buff_dec);
		printf("Resampling complete!\n");

		// Get left and right signals from sum and diff
		// printf("Getting left and right signals...\n");
		float left[SIZE_WRITE] = {0};
		float right[SIZE_WRITE];
		get_lr(sum_res, diff_res, left, right, batch_size_res);
		// printf("Left and right produced!\n");
		
		//sprintf(filename, "src/test/left.csv");
		//write_to_csv(left, batch_size_res, filename);
		//sprintf(filename, "src/test/right.csv");
		//write_to_csv(right, batch_size_res, filename);
		// Write left and right to file
		// printf("Writing to file...\n");
		//write_batch(p_FILE_LEFT, batch_size_res, left);
		//write_batch(p_FILE_RIGHT, batch_size_res, right);
*/
//		printf("Batch %d complete\n************************************************************************************************************\n", counter++);
	}
	fclose(fd);
	printf("Read: %f, FIR: %f, Demod: %f\n", t_read, t_fir, t_demod);
	printf("FIN.");
	return 0;
}
