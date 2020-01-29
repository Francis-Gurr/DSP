#ifndef _STRUCTS
#define _STRUCTS
struct Demod {
	const int SIZE;
	const float *p_OSC;
	int index;
	int inverse;
};

struct Buffer {
    const int SIZE;
    float values[1500];
    int offset;
    int wait;
};

struct Filter {
    const int SIZE; // Filter size
    const float *const p_H[3]; // Array of pointers to each filter
    int curr_res_filter;
    int curr_dec_filter;
};
#endif
