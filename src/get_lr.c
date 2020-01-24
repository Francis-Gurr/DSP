#include <stdlib.h>

void get_lr(float *p_sum_res,  float *p_diff_res, float *p_l, float *p_r, int size) {
    for (int i = 0; i < size; i++) {
        *p_r = (*p_sum_res + *p_diff_res) / 2;
        *p_l = (*p_sum_res - *p_diff_res) / 2;
	p_sum_res++;
	p_diff_res++;
	p_r++;
	p_l++;
    }
}

