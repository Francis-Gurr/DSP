#include "structs.h"

void add_to_buffer(float p_elem, struct Buffer *buff);

float fir1(const float *p_H, struct Buffer *buff);

int resample(float *p_batch, int batch_size, float *p_res, struct Filter *p_filter, struct Buffer *p_buff_res, struct Buffer *p_buff_dec);
