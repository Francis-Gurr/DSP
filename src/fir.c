#include "h/init.h"
#include "structs.h"

void fir(double *p_sum, double *p_diff, double *p_buff_sum, double *p_buff_diff, const double *p_H_LOW, struct Filter *p_f1) {
	int i;
	int offset = p_f1->offset;
	const int M = p_f1->SIZE_FILTER;
	for (i = 0; i < p_f1->SIZE_IN; i++) {
		/* ADD TO BUFFER */
		// Add sum
		p_buff_sum[offset] = p_sum[i];
		// Add diff
		p_buff_diff[offset] = p_diff[i];
		
		double sum = 0;
		double diff = 0;
		#pragma omp parallel for shared(p_H_LOW, p_buff_sum, p_buff_diff, offset) reduction(+:sum,diff)
		for (int j = offset; j >= 0; j--) {
			sum += p_buff_sum[j] * p_H_LOW[offset-j];
			diff += p_buff_diff[j] * p_H_LOW[offset-j];
		}
		int i_H =  offset + M;
		#pragma omp parallel for shared(p_H_LOW, p_buff_sum, p_buff_diff, offset, i_H) reduction(+:sum,diff)
		for (int j = M-1; j > offset; j--) {
			sum += p_buff_sum[j] * p_H_LOW[i_H-j];
			diff += p_buff_diff[j] * p_H_LOW[i_H-j];
		}
		if (++offset >= M) {
			offset = 0;
		}
		p_sum[i] = sum;
		p_diff[i] = diff;
	}
	p_f1->offset = offset;
}

