#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int read_batch(FILE *fd, const int SIZE, float *p_batch, int *p_exit) {

	int sz = fread(p_batch, 4, SIZE, fd);
	if (sz != SIZE) {
		printf("didn't read an entire batch");
		*p_exit = 1;
		for (int i = sz; i < SIZE; i++){
			*(p_batch+i) = 0.0;
		}

	}
	return sz;
}

int write_batch(const char *FILE_ID, int size, float *p_batch) {
	int sz;

	FILE *fd = fopen(FILE_ID, "a");
	sz = fwrite(p_batch, 4, size, fd);
	fclose(fd);

	if (sz != size) {
		return -1;
	}

	return 0;
}
