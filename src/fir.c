#include "structs.h"
#include<stdio.h>
#include<time.h>
#include "h/init_600.h"
double t_read = 0;
double t_for_0 = 0;
double t_for_1 = 0;
double t_write = 0;

void fir(FILE *fd, double *p_batch_out, const int batch_size, int filter_id, struct Buffer *p_buff) {
	int i;
	int offset = p_buff->offset;
	const int SIZE = p_buff->SIZE;
	clock_t begin;
	clock_t end;
	for (i = 0; i < batch_size; i++) {
		// Add next sample to the buffer
		begin = clock();
		fread(&p_buff->values[offset], 4, 1, fd);
		end = clock();
		t_read += (double)(end-begin) / CLOCKS_PER_SEC;
		
		// Get fir sum
		begin = clock();
		double sum = 0;
		#pragma omp parallel for shared(offset, p_buff) reduction(+: sum)
		for (int j = offset; j >= 0; j--) {
			sum += p_buff->values[j] * *(H[filter_id]+(offset-j));
		}
		int i_H = offset + SIZE -1;
		end = clock();
		t_for_0 += (double)(end-begin) / CLOCKS_PER_SEC;
		
		begin = clock();
		#pragma omp parallel for shared(i_H, p_buff) reduction(+: sum)

		for (int j = SIZE-1; j > offset; j--) {
			sum += p_buff->values[j] * *(H[filter_id]+(i_H-j));
		}
		if (++offset >= SIZE) {
			offset = 0;
		}
		end = clock();
		t_for_1 += (double)(end-begin) / CLOCKS_PER_SEC;

		begin = clock();
		*(p_batch_out + i) = sum;
		end = clock();
		t_write += (double)(end-begin) / CLOCKS_PER_SEC;
	}
	p_buff->offset = offset;
	printf("\nFIR times: read to buffer: %f, first for loop: %f, second for loop: %f, write to array: %f\n", t_read, t_for_0, t_for_1, t_write);
}

