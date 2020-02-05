#include "h/init.h"

void fir(double *p_sum, double *p_diff, double *p_buff_sum, double *p_buff_diff, int *p_offset, int M, int SIZE) {
	int i;
	int j;
	int offset = *p_offset;
	for (i = 0; i < SIZE; i++) {
		/* ADD TO BUFFER */
		// Add sum
		p_buff_sum[offset] = p_sum[i];
		// Add diff
		p_buff_diff[offset] = p_diff[i];
		
		double sum = 0;
		double diff = 0;
		for (j = offset; j >= 0; j--) {
			sum += p_buff_sum[j] * H_LOW[offset-j];
			diff += p_buff_diff[j] * H_LOW[offset-j];
		}
		int i_H =  offset + M;
		for (j = M-1; j > offset; j--) {
			sum += p_buff_sum[j] * H_LOW[i_H-j];
			diff += p_buff_diff[j] * H_LOW[i_H-j];
		}
		if (++offset >= M) {
			offset = 0;
		}
		p_sum[i] = sum;
		p_diff[i] = diff;
	}
	*p_offset = offset;
}

