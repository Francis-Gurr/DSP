#include "structs.h"

float * resample(float *p_batch, int *p_batch_size, struct Filter *p_filter, struct Buffer *p_buff_res, struct Buffer *p_buff_dec);

void add_to_buffer(float *p_elem, struct Buffer *buff);

void fir1(const float *p_H, struct Buffer *buff, float *resampled);
