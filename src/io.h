#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include "h/init.h"

/* Read a batch of size SIZE from input file file_id to input buffer 
 * Return pointer to input buffer and size read */
void read_batch(FILE* fd, double *p_batch, int *p_exit);

void check_zeros(float *p_batch, FILE *fd, int *p_all_zeros);

// Write a batch of size SIZE from buffer buf_out to output file FILE_ID
void write_batch(const char *FILE_ID, const int SIZE, double *p_batch);
