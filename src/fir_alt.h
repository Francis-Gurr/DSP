#include "structs.h"

void add_to_buff(float p_elem, struct Buffer *buff);
float get_fir_sum(const float *p_H, struct Buffer *buff);
void fir_alt(float *p_batch, int p_batch_size, const float *p_H, struct Buffer *p_fir_buff);
