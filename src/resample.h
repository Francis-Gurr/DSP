int,float resample(float *p_batch, int batch_size, struct Filter *p_filter, struct Buffer *p_buff_res, struct Buffer *p_buff_dec);

void add_to_buffer(float *p_elem, Buffer *buff);

void fir(const double h, Buffer buff, float *resampled)
