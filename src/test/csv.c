#include<stdio.h>

void write_to_csv(float *p_arr, int arr_size, char *filename) {
	printf(filename);
	FILE *fp;
	fp = fopen(filename, "w+");
	int i;
	for (i=0; i<arr_size-1; i++) {
		fprintf(fp,"%f,", *(p_arr+i));
	}
	fprintf(fp, "%f", *(p_arr+arr_size));
	fclose(fp);
}
