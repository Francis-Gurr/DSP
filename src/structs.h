#ifndef _STRUCTS
#define _STRUCTS
#include<stdbool.h>
struct Demod {
	const int SIZE;
	const double *p_OSC;
	int index;
	bool inverse;
};

struct Buffer {
    const int SIZE;
    float values[600];
    int offset;
    int wait;
};

struct Filter {
    const int SIZE; // Filter size
    double *const p_H[3]; // Array of pointers to each filter
    int curr_res_filter;
    int curr_dec_filter;
};
#endif
