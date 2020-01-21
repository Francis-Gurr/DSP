struct Buffer {
  const int SIZE;
  float values[SIZE];
  int offset;
  int wait;
} res_buffer, dec_buffer;

void main(pipe, const int MAX_INPUTS) {
  /* Filter declarations */
  const int FILTER_RES_SIZE = 15
  const float h0[FILTER_RES_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  const float h1[FILTER_RES_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  const float h3[FILTER_RES_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  float *const p_H[h0, h1, h3]; /* Array of pointers to each filter */
  int curr_res_filter = 0; /* Index of the current filter in p_H*/
  /* Decimation filter declarations */
  const float h0[FILTER_DEC_SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
                                  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
                                  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
                                  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
                                  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  int curr_dec_filter = 0;

  /* Resample buffer declarations */
  struct Buffer *buff_res = {.SIZE=FILTER_RES_SIZE, .values[FILTER_RES_SIZE]={0}, .offset=0, .wait=1};
  /* Decimate buffer declarations */
  struct Buffer *buff_dec = {.SIZE=FILTER_DEC_SIZE, .values[FILTER_SIZE]={0}, .offset=0, .wait=1};

  /* Other declarations */
  float *p_in; /* Pointer to the input sample */
  float *p_intermdiate;
  float *p_out; /* Pointer to the output sample */

  /* For each sample, resample it */
  int count;
  for (count = MAX_INPUTS; count > 0; count--) {
    /* Get next input sample */
    *p_in = pipe.next();

    /* Add the input sample to the buffer */
    add_to_buffer(*p_in, *buff_res)

    /* If the buffer is ready, resample */
    if (buff_res->wait == 0) {
      /* Resample by a factor of 3/25 */
      fir(*p_in, p_H[curr_res_filter], buff_res, *p_intermediate);
      /* Increase the curr_filter and buffer wait values */
      if (++*curr_res_filter > 2) {
        curr_res_filter = 0;
      }
      if (*curr_res_filter == 2) {
        buff_res->wait = 9;
      }
      else {
        buff_res->wait = 8;
      }

      /* Decimate by 25 */
      add_to_buffer(*p_intermdiate, *buff_dec);
      if (buff_dec->wait == 0) {
        fir(*p_intermdiate, h_dec, buff_dec, *p_out);
        buff_dec->wait = 25;
      }

    }

    /* Send output in the pipe */
    pipe.send(*p_out)
  }
}

void add_to_buffer(float *p_elem, Buffer *buff) {
  /* Add the input sample to the buffer */
  buff->values[buff->offset] = *p_elem;
  /* Increase the offset and wrap if necessary */
  if (++buff->offset > buff->SIZE) {
    buff->offset = 0;
  }
  /* Decrease the buffer wait value */
  buff->wait--;
}

void fir(float *p_in, const double h, Buffer buff, float *p_sum) {
  *sum = 0.0;
  int i;
  for (i = buff->offset; i > 0; i--) {
    *sum += buff->values[i] * h++;
  }
  for (i = buff->SIZE; i > buff->offset; i--) {
    *sum += buff->values[i] * h++;
  }
}
