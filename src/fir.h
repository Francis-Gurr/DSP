/*#include <stdlib.h>


// Handy buffer length macro, and a fixed filter length
#define FIR_FILTER_LEN (1024)


// Data structures
struct fir_buffer {
    // Circular buffer
    float samples[FIR_FILTER_LEN]; // array with size equal to filter length
    size_t offset; // index of newest sample within samples
}; 


// Find address of next sample in buffer to write
float *fir_buffer_next(struct fir_buffer *p_buffer); // function that returns a pointer to a float


// Compute output of FIR filter by reading buffer and multiplying with weights
void fir_filter( 
    float weights[FIR_FILTER_LEN],
    struct fir_buffer *p_buffer,
    float *p_output
);*/
