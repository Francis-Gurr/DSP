#include <stdio.h>
#include "fir.h"


// Macros for program arguments
#define INPUT_FILE (argv[0])
#define OUT_LEFT_FILE (argv[1])
#define OUT_RIGHT_FILE (argv[2])


// Data for FDM demultiplexing using bandpass filters
static fir_buffer_t fdm_filter_buffer;
static float sum_fdm_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the band pass FIR filter here
};
static float diff_fdm_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the band pass FIR filter here
};


// Data for DSB-SC demodulating using Costas loop detector
static fir_buffer_complex_t sum_dsbsc_lp_filter_buffer;
static float sum_dsbsc_lp_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the low pass FIR filter here
};
static fir_buffer_t sum_dsbsc_loop_filter_buffer;
static float sum_dsbsc_loop_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the Costas loop filter here
};

static fir_buffer_complex_t diff_dsbsc_lp_filter_buffer;
static float diff_dsbsc_lp_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the low pass FIR filter here
};
static fir_buffer_t diff_dsbsc_loop_filter_buffer;
static float diff_dsbsc_loop_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the Costas loop filter here
};


// Data for resampling using interpolation by 3 then decimation by 625
static fir_buffer_complex_t resampling_filter_buffer;
static float resampling_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the convolved reconstruction and anti-aliasing
    // filters here
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
        fread((void *) p_input, sizeof(*p_input), input_fp);

        // Perform FDM de-multiplexing to obtain amplitude modulated signals for
        // each channel
        float sum_am;
        float diff_am;
        fir_filter(sum_fdm_filter_weights, &fdm_filter_buffer, &sum_am);
        fir_filter(diff_fdm_filter_weights, &fdm_filter_buffer, &diff_am);
    }
}
