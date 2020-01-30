#include "structs.h"
#include<stdio.h>
void fir(float *p_batch_in, double *p_batch_out, const int batch_size, const double *p_H, struct Buffer *p_buff) {
	int i;
	int j;
	int offset = p_buff->offset;
	const int SIZE = p_buff->SIZE;
	for (i = 0; i < batch_size; i++) {
		// Add next sample to the buffer
		p_buff->values[offset] = *(p_batch_in + i);
		
		// Get fir sum
		double sum = 0;
		int i_H = 0;
		for (j = offset; j >= 0; j--) {
			sum += p_buff->values[j] * *(p_H+i_H);
//			if (p_buff->values[j] > 0.000001) {
//				printf("value=%f\n", p_buff->values[j]);
//				printf("coeff=%f\n", *(p_H+i_H));
//				printf("sum=%0.14f\n\n", sum);
//			}
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
//		if (sum > 0.00000000000001){
//			printf("%.15f\n", sum);
//		}
	}
	p_buff->offset = offset;
}
