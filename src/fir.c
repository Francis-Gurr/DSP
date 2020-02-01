#include "structs.h"
#include<stdio.h>
#include "h/init.c"

void fir(FILE *fd, double *p_batch_out, const int batch_size, int filter_id, struct Buffer *p_buff) {
	int i;
	int offset = p_buff->offset;
	const int SIZE = p_buff->SIZE;
	for (i = 0; i < batch_size; i++) {
		// Add next sample to the buffer
		fread(&p_buff->values[offset], 4, 1, fd);
		
		// Get fir sum
		double sum = 0;
		#pragma omp parallel for shared(offset, p_buff) reduction(+: sum)
		for (int j = offset; j >= 0; j--) {
			sum += p_buff->values[j] * *(H[filter_id]+(offset-j));
		}
		int i_H = offset + SIZE -1;
		#pragma omp parallel for shared(i_H, p_buff) reduction(+: sum)
		for (int j = SIZE-1; j > offset; j--) {
			sum += p_buff->values[j] * *(H[filter_id]+(i_H-j));
		}
		if (++offset >= SIZE) {
			offset = 0;
		}
		*(p_batch_out + i) = sum;
	}
	p_buff->offset = offset;
}
