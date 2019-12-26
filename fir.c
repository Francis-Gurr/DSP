#include <stdlib.h>
#include "fir.h"

float firFilter(float *b, struct circbuffer *x) {
    float y = 0;
    for (size_t i = 0; i < ELEMOF(x->samples); i++) {
        y += b[i] * x->samples[(x->offset + i) % ELEMOF(x->samples)];
    }
    return y;
}
