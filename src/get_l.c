#include <stdlib.h>

float get_l(float *p_summ_res,  float *p_diff_res) {
    float l = 0;
    for (size_t i = 0; i < sizeof(sum_res); i++) {
        l[i] = (*p_sum_res[i] - *p_diff_res) / 2;
    }
    return l;
}
