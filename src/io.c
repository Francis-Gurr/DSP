#include<stdio.h>
#include<fcntl.h>

int read_batch(const char FILE_ID, const int SIZE, float *p_batch, int *p_exit) {
	int fd, sz;

	fd = open(FILE_ID, 0_RDONLY);
	sz = read(fd, p_batch, SIZE);

	if (sz != SIZE) {
		*p_exit = 1;
	}

	close(fd);
	return sz;
}

int write_batch(const char FILE_ID, const int SIZE, float *p_batch) {
	int fd, sz;

	fd = open(FILE_ID, 0_WRONLY | 0_CREAT | 0_APPEND);
	sz = write(fd, p_batch, SIZE);
	close(fd);

	if (sz != SIZE) {
		return -1;
	}

	return 0;

