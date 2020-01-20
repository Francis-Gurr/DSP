void main(pipe, const int MAX_INPUTS) {
  /* Filter declarations */
  const int FILTER_SIZE = 15
  const double h0[FILTER_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  const double h1[FILTER_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  const double h3[FILTER_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  double *const p_H[h0, h1, h3]; /* Array of pointers to each filter */
  int curr_res_filter = 0; /* Index of the current filter in p_H*/
  /* Decimation filter declarations */
  const double h0[FILTER_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
                                  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
                                  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
                                  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
                                  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  int curr_dec_filter = 0;
  const int dec_m = 5; /* Decimation factor */

  /* Buffer declarations */
  float buffer[FILTER_SIZE] = {0};
  int *p_offset = 0; /* Pointer containing the buffer offset value */
  int *p_buffer_wait = 0; /* Pointer containing the number of input samples required before continuing */

  /* Other declarations */
  float *p_in; /* Pointer to the input sample */
  float *p_out; /* Pointer to the output sample */

  /* For each sample, resample it */
  int count;
  for (count = MAX_INPUTS, count > 0, count--) {
    /* Get next input sample */
    *p_in = pipe.next();

    /* Add the input sample to the buffer */
    buffer[*p_offset] = *p_in;
    /* Increase the offset and wrap if necessary */
    if (++ *p_offset > FILTER_SIZE) {
      *p_offset = 0;
    }
    /* Decrease the buffer wait value */
    *p_buffer_wait --;

    /* If the buffer is ready, resample */
    if (*p_buffer_wait == 0) {
      /* Resample by a factor of 3/5 */
      resample(*p_in, FILTER_SIZE, p_H[curr_res_filter], buffer, *p_offset, *p_out);
      /* Increase the curr_filter and buffer wait values */
      if (++*curr_filter > 2) {
        curr_res_filter = 0;
      }
      if (curr_res_filter == 2) {
        *p_buffer_wait = 1;
      }
      else {
        *p_buffer_wait = 2;
      }

      /* Decimate 5 times by 5 */
      decimate(*p_out, )
    }

    /* Send output in the pipe */
    pipe.send(*p_out)
  }
}

void resample(float *p_in, const int FILTER_SIZE, const double h, float buffer, int *p_offset, float *p_out) {
  float sum = 0.0;
  int i;
  for (i = *p_offset; i > 0; i--) {
    sum += buffer[i] * h++;
  }
  for (i = FILTER_SIZE; i > *p_offset; i--) {
    sum += buffer[i] * h++;
  }
  *p_out = sum
}
