#include "fir.h"

// Handy macro to find number of elements in an array
#define ELEMOF(X) (sizeof(X)/sizeof(X[0])) 


// Return address of the next sample in the circular buffer
float *fir_buffer_next(struct fir_buffer *p_buffer) {
    // Make all of the samples a bit newer
    p_buffer->offset = (p_buffer->offset - 1) % ELEMOF(p_buffer->samples); // update offset value  
    float *p_input = &(p_buffer->samples[p_buffer->offset]); // returns address of value to be overwritten with newest sample
    return p_input;
}

// Convolve filter weights with circular buffer
void fir_filter(
    float weights[FIR_FILTER_LEN],
    struct fir_buffer *p_buffer,
    float *p_output
) {
    *p_output = 0;
    for (size_t i = 0; i < ELEMOF(weights); i++) {
        size_t j = (p_buffer->offset + i) % ELEMOF(p_buffer->samples);
        *p_output += weights[i] * p_buffer->samples[j];
    }
}
