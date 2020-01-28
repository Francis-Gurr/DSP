#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int read_batch(FILE *fd, const int SIZE, float *p_batch, int *p_exit) {
	/*int sz;
	char batch_char[SIZE*4];
	// double batch_double[SIZE];

	sz = read(fd, batch_char, SIZE*4);
	int i = 0;
	char float_val[4];
	while (i < SIZE) {
		for (int j = 0; j < 4; j++) {
			float_val[j] = batch_char[4*i+j];
		}
		float *val = (float *)atof(float_val);
		if (*val > 0.000001) {
			printf("%f\n", val);
		}
		*(p_batch+i) = *val;
		i++;
		}
	for (int i = 0; i < SIZE; i++) {
		if(*(p_batch+i) > 0.00001) {
			printf("%f\n", *(p_batch+i));
		}
	}

	if (sz != SIZE*4) {
		printf("didn't read an entire batch");
		*p_exit = 1;
		for (int i = sz/4; i < SIZE; i++){
			*(p_batch+i) = 0.0;
		}

	}*/

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
	int fd, sz;

	fd = fopen(FILE_ID, "a");
	sz = fwrite(p_batch, 4, SIZE, fd);
	fclose(fd);

	if (sz != size) {
		return -1;
	}

	return 0;
}
