#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

/* Read a batch of size SIZE from input file file_id to input buffer 
 * Return pointer to input buffer and size read */
void read_batch(FILE* fd, const int SIZE, float *p_batch, int *p_exit);

/* Write a batch of size SIZE from buffer buf_out to output file FILE_ID
 * Return a 0 for success, 1 for error */
void write_batch(const char *FILE_ID, const int SIZE, double *p_batch);
