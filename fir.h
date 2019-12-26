#define FILTER_LEN (1024)
#define ELEMOF(X) (sizeof(X)/sizeof(X[0]))

typedef struct circ_buffer {
    float samples[FILTER_LEN];
    size_t offset;
} circ_buffer_t;

float fir_filter(float *b, circ_buffer_t *x);
