#ifndef _STRUCTS
#define _STRUCTS
struct Filter {
	const int SIZE_IN;
	const int SIZE_FILTER;
	int offset;
};

struct Buffer {
    int offset;
    int wait;
    int curr_filter;
};
#endif
