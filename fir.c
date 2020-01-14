#include "fir.h"

// Handy macro to find number of elements in an array
#define ELEMOF(X) (sizeof(X)/sizeof(X[0]))


// Return address of the next sample in the circular buffer
float *fir_buffer_next(fir_buffer_t *buffer) {
    float *p_sample = &buffer->samples[buffer->offset]
    buffer->offset++;
    return p_sample
}


// Return address of the next sample in the circular buffer
float *fir_buffer_complex_next(fir_buffer_complex_t *buffer) {
    float complex *p_sample = &buffer->samples[buffer->offset]
    buffer->offset++;
    return p_sample
}


// Convolve filter weights with circular buffer
void fir_filter(
    float weights[FIR_FILTER_LEN],
    fir_buffer_t *input,
    float *output
) {
    *output = 0
    for (size_t i = 0; i < ELEMOF(weights); i++) {
        size_t j = (input->offset + i) % ELEMOF(input->samples);
        *output += weights[i] * input->samples[j];
    }
}


// Convolve filter weights with complex circular buffer
void fir_filter_complex(
    float weights[FIR_FILTER_LEN],
    fir_buffer_complex_t *input,
    float complex *output
) {
    *output = 0
    for (size_t i = 0; i < ELEMOF(weights); i++) {
        size_t j = (input->offset + i) % ELEMOF(input->samples);
        *output += weights[i] * input->samples[j];
    }
}
