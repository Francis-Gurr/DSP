#include <stdio.h>
#include "structs.h"
#include "io.h"
#include "fir_alt.h"
#include "demodulator.h"
#include "resample.h"
#include "get_lr.h"

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
	const char *const p_FILE_IN = argv[1];
	const char *const p_FILE_LEFT = argv[2];
	const char *const p_FILE_RIGHT = argv[3];
	
	/* FIR FILTER */
	const float H_SUM[SIZE_FIR] = {}; // Sum filter coefficients
	const float H_DIFF[SIZE_FIR] = {}; // Diff filter coefficients
	struct Buffer buff_fir_sum = {.SIZE=SIZE_FIR, .values={}, .offset=0};
	struct Buffer buff_fir_diff = {.SIZE=SIZE_FIR, .values={}, .offset=0};
	
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
		float batch[SIZE_READ] = {};
		int batch_size;
		batch_size = read_batch(p_FILE_IN, SIZE_READ, batch, p_exit);
		
		// Use FIR filter to split the batch into sum and diff
		// Return a pointer to the first element in the sum and diff array
		float sum[batch_size];
		float diff[batch_size];
		fir_alt(sum, batch_size, H_SUM, &buff_fir_sum);
		fir_alt(diff, batch_size, H_DIFF, &buff_fir_diff);
		
		// Demodulate sum and diff
		demod(sum, batch_size, &sum_osc);
		demod(diff, batch_size, &diff_osc);
		
		// Resample sum and diff
		float sum_res[batch_size];
		float diff_res[batch_size];
		batch_size_res = resample(sum, batch_size, sum_res, &filter, &buff_res, &buff_dec);
		batch_size_res = resample(diff, batch_size, diff_res, &filter, &buff_res, &buff_dec);

		// Get left and right signals from sum and diff
		float left[batch_size_res];
		float right[batch_size_res];
		get_lr(sum_res, diff_res, left, right, batch_size_res);
		
		// Write left and right to file
		write_batch(p_FILE_LEFT, batch_size_res, left);
		write_batch(p_FILE_RIGHT, batch_size_res, right);
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
