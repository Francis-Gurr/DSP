#include<unistd.h>
#include<fcntl.h>

int read_batch(const char *FILE_ID, const int SIZE, float *p_batch, int *p_exit) {
	int fd, sz;

	fd = open(FILE_ID, O_RDONLY);
	sz = read(fd, p_batch, SIZE);

	if (sz != SIZE) {
		*p_exit = 1;
	}

	close(fd);
	return sz;
}

int write_batch(const char *FILE_ID, int size, float *p_batch) {
	int fd, sz;

	fd = open(FILE_ID, O_WRONLY | O_CREAT | O_APPEND);
	sz = write(fd, p_batch, size);
	close(fd);

	if (sz != size) {
		return -1;
	}

	return 0;
}
