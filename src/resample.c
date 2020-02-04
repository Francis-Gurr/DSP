#include "structs.h"
#include "h/init.h"
#include <stdio.h>

/*
void add_to_buffer(float p_elem, struct Buffer *buff) {
  buff->values[buff->offset] = p_elem;
  if (++buff->offset > buff->SIZE) {
    buff->offset = 0;
  }
  buff->wait--;
}

float fir_time(const float *p_H, struct Buffer *buff) {
  float sum = 0.0;
  int i;
  for (i = buff->offset; i > 0; i--) {
    sum += buff->values[i] * p_H[i];
  }
  for (i = buff->SIZE; i > buff->offset; i--) {
    sum += buff->values[i] * p_H[i];
  }
  return sum;
}
*/

void resample(double *p_sum, double *p_diff, double *p_buff_sum, double *p_buff_diff, struct Buffer *p_buff_params, double *p_left, double *p_right) {
	int offset = p_buff_params->offset;
	int wait = p_buff_params->wait;
	int curr_filter = p_buff_params->curr_filter;
	int i;
	int j;
	int k = 0;
	for (i = 0; i < 1875; i++) {
		/* ADD TO BUFFER */
		// Add sum
		p_buff_sum[offset] = p_sum[i];
		// Add diff
		p_buff_diff[offset] = p_diff[i];
		// Increment offset
		if (++offset > M_RES) {
			offset = 0;
		}
		wait--;

		/* RESAMPLE */
		if (wait == 0) {
			// Fir
			double sum = 0.0;
			double diff = 0.0;
			for (j = offset; j >= 0; j--) {
				sum += p_buff_sum[j] * H_RES[curr_filter][offset-j];
				diff += p_buff_diff[j] * H_RES[curr_filter][offset-j];
			}
			int i_H =  offset + M_RES - 1;
			for (j = M_RES-1; j > offset; j--) {
				sum += p_buff_sum[j] * H_RES[curr_filter][i_H-j];
				diff += p_buff_diff[j] * H_RES[curr_filter][i_H-j];
			}
			// Increment current filter and buffer wait value 
			if (++curr_filter > 5) {
				curr_filter = 0;
			}
			if (curr_filter == 5) {
				wait = 105;
			}
			else {
				wait = 104;
			}

			/* GET LEFT AND RIGHT */
			p_left[k] = (sum - diff) / 2;
			p_right[k] = (sum + diff) / 2;
			k++;
		}
	}	
	p_buff_params->offset = offset;
	p_buff_params->wait = wait;
	p_buff_params->curr_filter = curr_filter;
}
