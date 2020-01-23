int,float resample(float *p_batch, int batch_size, struct Filter *p_filter, struct Buffer *p_buff_res, struct Buffer *p_buff_dec) {
	int batch_size_res = batch_size * 0.0048;
	float resampled[batch_size_res];
	float *p_resampled = resampled;
	// For each sample in the batch
	for (int i = batch_size; i--; ) {
		float sample = *p_batch++; // Sample is the current value from the array
		float *const p_H[3] = p_filter->p_H;
		int curr_res_filter = p_filter->curr_res_filter;
		int curr_dec_filter = p_filter->curr_dec_filter;

		// Add the sample to the resampling buffer
		add_to_buffer(sample, *p_buff_res);
		
		// If the buffer is ready, resample
		if (buff_res->wait == 0) {
			// First resample by a factor of 3/25
			fir(p_filter->p_H[curr_res_filter], p_buff_res, resampled);
			
			// Increase the curr_filter and buffer wait values
			if (++curr_res_filter > 2) {
				curr_res_filter = 0;
			}
			if (curr_res_filter == 2) {
				buff_res->wait = 9;
			}
			else {
				buff_res->wait = 8;
			}
			
			// Add the resampled sample to the decimation buffer
			add_to_buffer(resampled, p_buff_dec);
			
			// If the decimation buffer is ready, decimate
			if (buff_dec->wait == 0) {
				fir(resampled, p_filter->p_H[curr_dec_filter], p_buff_dec, resampled);
				buff_dec->wait = 25;
				if (++curr_dec_filter > 2) {
					curr_dec_filter = 0;
				}
			}
		}
	}
	resampled++;
	return [p_resampled, batch_size_res]
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

void fir(const double h, Buffer buff, float *resampled) {
  float sum = 0.0;
  int i;
  for (i = buff->offset; i > 0; i--) {
    sum += buff->values[i] * h(+i);
  }
  for (i = buff->SIZE; i > buff->offset; i--) {
    sum += buff->values[i] * h(+i);
  }
  resampled = &sum
}
