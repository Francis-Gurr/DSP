#include "h/init.h"

void fir(double *p_sum, double *p_diff, double *p_buff_sum, double *p_buff_diff, int *p_offset) {
	int i;
	int offset = *p_offset;
	for (i = 0; i < L; i++) {
		/* ADD TO BUFFER */
		// Add sum
		p_buff_sum[offset] = p_sum[i];
		// Add diff
		p_buff_diff[offset] = p_diff[i];
		
		double sum = 0;
		double diff = 0;
		for (j = offset; j >= 0; j--) {
			sum += p_buff_sum[j] * H_LOW[0][offset-j];
			diff += p_buff_diff[j] * H_LOW[1][offset-j];
		}
		int i_H =  offset + L - 1;
		for (j = L-1; j > offset; j--) {
			sum += p_buff_sum[j] * H_LOW[0][i_H-j];
			diff += p_buff_diff[j] * H_LOW[1][i_H-j];
		}
		if (++offset >= SIZE) {
			offset = 0;
		}
		p_sum[i] = sum;
		p_diff[i] = diff;
	}
	*p_offset = offset;
}

