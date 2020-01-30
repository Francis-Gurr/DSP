#include "structs.h"
#include<stdio.h>

//void add_to_buff(float p_elem, struct Buffer *buff);
//float get_fir_sum(const float *p_H, struct Buffer *buff);
//void fir(float *p_batch_in, float *p_batch_out, const float *p_H, struct Buffer *p_fir_buff, const int SIZE);
void fir(float *p_batch_in, double *p_batch_out, const int batch_size, const double *p_H, struct Buffer *p_buff);
