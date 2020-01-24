#include "structs.h"

void add_to_buff(float p_elem, struct Buffer *p_buff) {
	p_buff->values[p_buff->offset] = p_elem;
	if (++p_buff->offset > p_buff->SIZE) {
		p_buff->offset = 0;
	}
}

float get_fir_sum(const float *p_H, struct Buffer *p_buff) {
	float sum = 0.0;
	int i;
	for (i = p_buff->offset; i > 0; i--) {
		sum += p_buff->values[i] * *(p_H+i);
	}
	for (i = p_buff->SIZE; i > p_buff->offset; i--) {
		sum += p_buff->values[i] * *(p_H+i);
	}
	return sum;
}

void fir_alt(float *p_batch, int batch_size, const float *p_H, struct Buffer *p_buff) {
	int i;
	for (i = batch_size; i > 0; i--) {
		add_to_buff(*(p_batch+i), p_buff);
		*(p_batch+i) = get_fir_sum(p_H, p_buff);
	}
}
