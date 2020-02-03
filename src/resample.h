#include "structs.h"

void add_to_buffer(float p_elem, struct Buffer *buff);

float fir1(const float *p_H, struct Buffer *buff);

int resample(float *p_in, int *p_out, int *curr_res_filter, struct Buffer *p_buff);
