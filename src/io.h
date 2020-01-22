#include<stdio.h>
#include<fcntl.h>

/* Read a batch of size SIZE from input file file_id to input buffer 
 * Return pointer to input buffer and size read */
int[2] read_batch(FILE_ID, SIZE, *p_exit);

/* Write a batch of size SIZE from buffer buf_out to output file FILE_ID
 * Return a 0 for success, 1 for error */
int write_batch(FILE_ID, SIZE, buf_out);
