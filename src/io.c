#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

void read_batch(FILE *fd, float *p_batch, int *p_exit) {

	int sz = fread(p_batch, 4, L, fd);

	/* PADD final batch*/
	if (sz != L) {
		printf("Didn't read an entire batch\n");
		*p_exit = 1;
		for (int i = sz; i < SIZE; i++){
			*(p_batch+i) = 0.0;
		}

	}
}

void check_zeros(float *p_batch, FILE *fd, int *p_all_zeros) {
	int i_old;
	int i_new = 0;
	for (i_old = 0; i_old < L; i_old++) {
		if (p_batch[i_old] != 0) {	
			*p_all_zeros = 0;	
			break;
		}
	}
	if (*p_all_zeros == 0) {
		batch_extra[i_old];
		fread(&batch_extra, 4, i_old, fd);
		for (int i = 0; i < L-i_old; i++){
			p_batch[i] = p_batch[i_old + i];
		}
		for (int i = 0; i < i_old; i++) {
			p_batch[i+i_old] = batch_extra[i];
		}
	}
}

void write_batch(const char *FILE_ID, const int SIZE, double *p_batch) {
	int sz;
	float batch_float[SIZE];
	for (int i = 0; i < SIZE; i++) {
		batch_float[i] = (float) *(p_batch + i);
	}

	FILE *fd = fopen(FILE_ID, "ab");
	sz = fwrite(batch_float, sizeof(float), SIZE, fd);
	fclose(fd);

	if (sz != SIZE) {
		printf("Didn't write an entire batch\n");
	}

}
