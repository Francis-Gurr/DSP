#include <stdio.h>
#include "structs.h"
#include "io.h"
#include "fir.h"
#include "demodulator.h"
#include "resample.h"

#define SIZE_FIR 5
#define SIZE_RES 5
#define SIZE_DEC 5
#define SIZE_READ 1875
#define SIZE_WRITE 9

int main(int argc, char *argv[]) {
	/****************************************************************************
	 * START OF DECLARATIONS
	 *****************************************************************************/
	
	printf("START");
	/* FILE PATHS */
	const char FILE_IN = argv[1];
	const char FILE_LEFT = argv[2];
	const char FILE_RIGHT = argv[3];
	
	/* FIR FILTER */
	const int F;
	const float H_SUM[SIZE_FIR] = {}; // Sum filter coefficients
	const float H_DIFF[SIZE_FIR] = {}; // Diff filter coefficients
	
	/* DEMODULATION */
	const float SUM_OSC[5] = {};
	const float DIFF_OSC[100] = {};
	struct Demod sum_osc = {.SIZE=5, .p_OSC=SUM_OSC, .index=0, .inverse=0};
	struct Demod diff_osc = {.SIZE=100, .p_OSC=DIFF_OSC, .index=0, .inverse=0};	

	/* RESAMPLE */
	const float H0[SIZE_RES] = {0,1,2,3,4};
	const float H1[SIZE_RES] = {0,1,2,3,4};
	const float H2[SIZE_RES] = {0,1,2,3,4};
	struct Filter filter = {
		.SIZE = SIZE_RES,
		.p_H = {H0,H1,H2},
		.curr_res_filter = 0,
		.curr_dec_filter = 0
	};
	struct Buffer buff_res = {.SIZE=SIZE_RES, .values={}, .offset=0, .wait=1}; // Resample buffer
	struct Buffer buff_dec = {.SIZE=SIZE_DEC, .values={}, .offset=0, .wait=1}; // Decimation buffer

	/****************************************************************************
	* END OF DECLARATIONS
	*****************************************************************************/

	int *p_exit = 0;
	int batch_size_res = 9; 
	while (*p_exit == 0) {
		// Read n=SIZE_READ samples from FILE_IN
		// Return a pointer to the first element in the batch and the batch size
		float *p_batch;
		int batch_size;
		batch_size = read_batch(FILE_IN, SIZE_READ, p_batch, p_exit);
		
		// Use FIR filter to split the batch into sum and diff
		// Return a pointer to the first element in the sum and diff array
		float *p_sum;
		float *p_diff;
		//p_sum = fir(p_batch, batch_size, H_SUM);
		//p_diff = fir(p_batch, batch_size, H_DIFF);
		
		// Demodulate sum and diff
		float *p_sum_demod;
		float *p_diff_demod;
		p_sum_demod = demod(p_sum, batch_size, &sum_osc);
		p_diff_demod = demod(p_diff, batch_size, &diff_osc);
		
		// Resample sum and diff
		float *p_sum_res;
		float *p_diff_res;
		p_sum_res = resample(p_sum_demod, &batch_size, &filter, &buff_res, &buff_dec);
		p_diff_res = resample(p_diff_demod, &batch_size, &filter, &buff_res, &buff_dec);

		// Get left and right signals from sum and diff
		float *p_left;
		float *p_right;
		//p_left = get_l(p_sum_res, p_diff_res);
		//p_right = get_r(p_sum_res, p_diff_res);
		
		// Write left and right to file
		write_batch(FILE_LEFT, SIZE_WRITE, p_left);
		write_batch(FILE_RIGHT, SIZE_WRITE, p_right);
	}
	return 0;
}
/*
// OLD

// Macros for program arguments
#define INPUT_FILE (argv[0])
#define OUT_LEFT_FILE (argv[1])
#define OUT_RIGHT_FILE (argv[2])


// Data for FDM demultiplexing using bandpass filters
static struct fir_buffer fdm_filter_buffer;
static float sum_fdm_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the band pass FIR filter here
};
static float diff_fdm_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the band pass FIR filter here
};

// Main entry point for program
void main(int argc, char *argv[]) {
    // Open input and output files
    FILE *input_fp = fopen(INPUT_FILE);
    FILE *out_left_fp = fopen(OUT_LEFT_FILE);
    FILE *out_right_fp = fopen(OUT_RIGHT_FILE);

    // Process the input file float by float.
    while (!feof(input_f)) {
        // Populate FDM FIR filter buffer from input file
        float *p_input = fir_buffer_next(&fdm_filter_buffer);
        fread((void *) p_input, sizeof(*p_input), input_fp)

        // Perform FDM de-multiplexing to obtain amplitude modulated signals for
        // each channel
        float sum_am;
        float diff_am;
        fir_filter(sum_fdm_filter_weights, &fdm_filter_buffer, &sum_am);
        fir_filter(diff_fdm_filter_weights, &fdm_filter_buffer, &diff_am);
    }
}*/
