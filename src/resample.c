#include "structs.h"

void add_to_buffer(float p_elem, struct Buffer *buff) {
  /* Add the input sample to the buffer */
  buff->values[buff->offset] = p_elem;
  /* Increase the offset and wrap if necessary */
  if (++buff->offset > buff->SIZE) {
    buff->offset = 0;
  }
  /* Decrease the buffer wait value */
  buff->wait--;
}

float fir1(const float *p_H, struct Buffer *buff) {
  float sum = 0.0;
  int i;
  for (i = buff->offset; i > 0; i--) {
    sum += buff->values[i] * *(p_H+i);
  }
  for (i = buff->SIZE; i > buff->offset; i--) {
    sum += buff->values[i] * *(p_H+i);
  }
  return sum;
}

int resample(float *p_batch, int batch_size, float *p_res, struct Filter *p_filter, struct Buffer *p_buff) {
	int batch_size_res = batch_size * 0.0048;
	float *resampled = p_res;
	// For each sample in the batch
	for (int i = batch_size; i--; ) {
		float sample = *(p_batch+i); // Sample is the current value from the array
		int curr_res_filter = p_filter->curr_res_filter;

		// Add the sample to the resampling buffer
		add_to_buffer(sample, p_buff);
		
		// If the buffer is ready, resample
		if (p_buff->wait == 0) {
			// Resample by a factor of 3/625
			 resampled = fir1(p_filter->p_H[curr_res_filter], p_buff);
			
			// Increase the curr_filter and buffer wait values
			if (++curr_res_filter > 2) {
				curr_res_filter = 0;
			}
			if (curr_res_filter == 2) {
				p_buff->wait = 209;
			}
			else {
				p_buff->wait = 208;
			}
			resampled++;
		}	
	}
	return batch_size_res;
}
