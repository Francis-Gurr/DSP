#include "structs.h"

void add_to_buffer(float p_elem, struct Buffer *buff) {
  buff->values[buff->offset] = p_elem;
  if (++buff->offset > buff->SIZE) {
    buff->offset = 0;
  }
  buff->wait--;
}

float get_fir_sum(const float *p_H, struct Buffer *buff) {
  float sum = 0.0;
  int i;
  for (i = buff->offset; i > 0; i--) {
    sum += buff->values[i] * *(p_H+i);
  }
  for (i = buff->SIZE; i > buff->offset; i--) {
    sum += buff->values[i] * *(p_H+i);
  }
  return sum;
}

void fir_alt(float *p_batch, int p_batch_size, const float *p_H, struct Buffer *p_fir_buff) {
  int i;
  for (i = p_batch_size; i > 0; i--) {
  	add_to_buffer(*(p_batch+i), p_fir_buff);
	*(p_batch+i) = get_fir_sum(p_H, p_fir_buff);
  }
  }
