#include<stdio.h>
#include<fcntl.h>

int[2] read_batch(FILE_ID, SIZE, *p_exit) {
	int fd, sz;
	float[size] *buf_in;

	fd = open(FILE_ID, 0_RDONLY);

	sz = read(fd, buf_in, SIZE);

	if (sz != SIZE) {
		*p_exit = 1;
	}

	close(fd);
	return [buf_in, sz];
}

int write_batch(FILE_ID, SIZE, buf_out) {
	int fd, sz;

	fd = open(FILE_ID, 0_WRONLY | 0_CREAT | 0_APPEND);

	sz = write(fd, buf_out, SIZE);
	close(fd);

	if (sz != SIZE) {
		return -1;
	}

	return 0;

