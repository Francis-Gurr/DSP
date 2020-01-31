#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

void read_batch(FILE *fd, const int SIZE, float *p_batch, int *p_exit) {

	int sz = fread(p_batch, 4, SIZE, fd);
	if (sz != SIZE) {
		printf("Didn't read an entire batch\n");
		*p_exit = 1;
		for (int i = sz; i < SIZE; i++){
			*(p_batch+i) = 0.0;
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
