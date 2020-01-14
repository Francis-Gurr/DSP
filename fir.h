#include <complex.h>
#include <stdlib.h>


// Handy buffer length macro, and a fixed filter length
#define FIR_FILTER_LEN (1024)


// Data structures
typedef struct fir_buffer {
	// Circular buffer
    float samples[FIR_FILTER_LEN];
    size_t offset;
} fir_buffer_t;

typedef struct fir_buffer_complex {
	// Complex circular buffer
    float complex samples[FIR_FILTER_LEN];
    size_t offset;
} fir_buffer_complex_t;


// Find address of next sample in buffer
float *fir_buffer_next(fir_buffer_t *buffer);
float complex *fir_buffer_complex_next(fir_buffer_complex_t *buffer);


// Find output of FIR filter
void fir_filter(
	float weights[FIR_FILTER_LEN],
	fir_buffer_t *inputs,
	float *output
);
void fir_filter_complex(
	float weights[FIR_FILTER_LEN],
	fir_buffer_complex_t *inputs,
	float complex *output
);
