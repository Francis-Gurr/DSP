#include <stdlib.h>

float get_r(float *p_sum_res,  float *p_diff_res) {
    float r = 0;
    for (size_t i = 0; i < sizeof(sum_res); i++) {
        r[i] = (*p_sum_res[i] + *p_diff_res) / 2;
    }
    return r;
}
