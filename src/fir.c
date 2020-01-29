#include "structs.h"
#include<stdio.h>
/*
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

void fir(float *p_batch_in, float *p_batch_out, const float *p_H, struct Buffer *p_buff, const int SIZE) {
	int i;
	for (i = 0; i < SIZE; i++) {
		add_to_buff(*(p_batch_in+i), p_buff);
		*(p_batch_out+i) = get_fir_sum(p_H, p_buff);
	}
}
*/

void fir(float *p_batch_in, float *p_batch_out, const int batch_size, const float *p_H, struct Buffer *p_buff) {
	int i;
	int j;
	int offset = p_buff->offset;
	const int SIZE = p_buff->SIZE;
	for (i = 0; i < batch_size; i++) {
		// Add next sample to the buffer
		p_buff->values[offset] = *(p_batch_in + i);
		
		// Get fir sum
		float sum = 0;
		int i_H = 0;
		for (j = offset; j >= 0; j--) {
			sum += p_buff->values[j] * *(p_H+i_H);
			i_H++;
		}
		for (j = SIZE-1; j > offset; j--) {
			sum += p_buff->values[j] * *(p_H+i_H);
			i_H++;
		}
		if (++offset >= SIZE) {
			offset = 0;
		}
		*(p_batch_out + i) = sum;
	}
	p_buff->offset = offset;
}
	


