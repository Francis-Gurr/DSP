#include "read.h"
#include "fir.h"
#include "demod.h"
#include "resample.h"
#include "get_lr.h"
#include "write.h"

struct Buffer {
    const int SIZE;
    float values[SIZE];
    int offset;
    int wait;
};

struct Filter_Res {
    const int SIZE; // Filter size
    float *const p_H[3]; // Array of pointers to each filter
    int curr_filter;
};

struct Filter_Dec {
    const int SIZE; // Filter size
    const float H[SIZE]; // Filter coefficients
};

int main(int argc, char *argv[]) {
	/****************************************************************************
	 * START OF DECLARATIONS
	 *****************************************************************************/
	
	/* FILE PATHS */
	const char FILE_IN = *arg[0];
	const char FILE_LEFT = *arg[1];
	const char FILE_RIGHT = *arg[2];
	
	/* FIR FILTER */
	const int F;
	const float H_SUM[FIR_FILTER_LEN] = {}; // Sum filter coefficients
	const float H_DIFF[FIR_FILTER_LEN] = {}; // Diff filter coefficients
	
	/* DEMODULATION */
	const float SUM_OSC[5] = {};
	const float DIFF_OSC[100] = {};
	
	/* RESAMPLE */
	const int SIZE_RES = 0;
	const float H0[SIZE_RES] = {};
	const float H1[SIZE_RES] = {};
	const float H2[SIZE_RES] = {};
	struct Filter_Res *p_filter_res = {
		.SIZE = SIZE_RES,
		.p_H = {H0,H1,H2},
		.curr_filter = 0
	};
	const in SIZE_DEC;
	const float H0[SIZE_DEC] = {};
	struct Filter_Dec *p_filter_dec = {
		.SIZE = SIZE_DEC,
		.H = {}
	};
	struct Buffer *buff_res = {.SIZE=SIZE_RES, .values[SIZE_RES]={0}, .offset=0, .wait=1}; // Resample buffer
	struct Buffer *buff_dec = {.SIZE=SIZE_DEC, .values[SIZE_DEC]={0}, .offset=0, .wait=1}; // Decimation buffer

	/****************************************************************************
	* END OF DECLARATIONS
	*****************************************************************************/

	int *p_exit = 0;
	const int SIZE_READ = 1875; // Max size of read batch
	const int SIZE_WRITE = 9;
	const float RES_FACTOR = 3/625;
	int batch_size_res = 9; 
	while (*p_exit == 0) {
		// Read n=SIZE_READ samples from FILE_IN
		// Return a pointer to the first element in the batch and the batch size
		float *p_batch;
		int batch_size;
		[p_batch, batch_size] = read_batch(FILE_IN, SIZE_READ, *p_exit);
		
		// Use FIR filter to split the batch into sum and diff
		// Return a pointer to the first element in the sum and diff array
		float *p_sum;
		float *p_diff;
		p_sum = fir(p_batch, batch_size, H_SUM);
		p_diff = fir(p_batch, batch_size, H_DIFF);
		
		// Demodulate sum and diff
		float *p_sum_demod;
		float *p_diff_demod;
		p_sum_demod = demod(sum, batch_size, sum_osc);
		p_diff_demod = demod(diff, batch_size, diff_osc);
		
		// Resample sum and diff
		float *p_sum_res;
		float *p_dif_res;
		sum_res = resample(sum_demod, *p_filter_res, *p_filter_dec, *buff_res, *buff_dec);
		diff_res = resample(diff_demod, *p_filter_res, *p_filter_dec, *buff_res, *buff_dec);
		// If last batch, update the batch_size_res
		if (*p_exit != 0) {
			batch_size_res = batch_size * RES_FACTOR;
		}

		// Get left and right signals from sum and diff
		float *p_left;
		float *p_right;
		[p_left, p_right] = get_lr(p_sum_res, p_diff_res);
		
		// Write left and right to file
		write(p_left, FILE_LEFT);
		write(p_right, FILE_RIGHT);
	}
	return 0;
}

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
}
