#include "read.h"
#include "fir.h"
#include "demod.h"
#include "resample.h"
#include "get_lr.h"
#include "write.h"

int main(int argc, char *argv[]) {
    const *char input_file = *arg[0];
    const *char left_file = *arg[1];
    const *char right_file = *arg[2];
    
    static float sum_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the band pass FIR filter here
    };
    static float diff_filter_weights[FIR_FILTER_LEN] = {
    // Insert filter weights for the band pass FIR filter here
    };
    static const float sum_osc[5] = {
    };
    static const float diff_osc[100] = {
    };
    
    // Copy Francis' stuff
    
    int *p_exit = 0;
    const int size = 2000
    float buffer[size] = {0.0}; 
    while (*p_exit == 0) {
        buffer = read(input_file,*p_exit,size);
        sum = fir(buffer,sum_filter_weights);
        diff = fir(buffer,diff_filter_weights);
        sum_demod = demod(sum,sum_osc);
        diff_demod = demod(diff,diff_osc);
        sum_res = resample(sum_demod);
        diff_res = resample(diff_demod);
        [l,r] = get_lr(sum_res,diff_res);
        write(l,left_file);
        write(r,right_file);
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
