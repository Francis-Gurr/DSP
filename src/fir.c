#include "structs.h"
#include<stdio.h>

void fir(float *p_batch_in, double *p_batch_out, const int batch_size, const double *p_H, struct Buffer *p_buff) {
	int i;
	int offset = p_buff->offset;
	const int SIZE = p_buff->SIZE;
	for (i = 0; i < batch_size; i++) {
		// Add next sample to the buffer
		p_buff->values[offset] = *(p_batch_in + i);
		
		// Get fir sum
		double sum = 0;
		//int i_H = 0;
		#pragma omp parallel for shared(offset, p_buff) reduction(+: sum)
		for (int j = offset; j >= 0; j--) {
			sum += p_buff->values[j] * *(p_H+(offset-j));
			//i_H++;
		}
		int i_H = offset + SIZE -1;
		#pragma omp parallel for shared(i_H, p_buff) reduction(+: sum)
		for (int j = SIZE-1; j > offset; j--) {
			sum += p_buff->values[j] * *(p_H+(i_H-j));
			//i_H++;
		}
		if (++offset >= SIZE) {
			offset = 0;
		}
		*(p_batch_out + i) = sum;
	}
	p_buff->offset = offset;
}
