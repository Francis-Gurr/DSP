#define FILTER_LEN (1024)
#define ELEMOF(X) (sizeof(X)/sizeof(X[0]))

struct circbuffer {
    float samples[FILTER_LEN];
    size_t offset;
};

float firFilter(float *b, struct circbuffer *x);
