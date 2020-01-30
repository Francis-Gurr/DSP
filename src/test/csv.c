#include<stdio.h>

void write_to_csv(double *p_arr, int arr_size, char *filename) {
	FILE *fp = fopen(filename, "a");
	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	if (file_size > 0) {
		fprintf(fp,"\n");
	}
	int i;
	for (i=0; i<arr_size-1; i++) {
		fprintf(fp,"%.15f\n", *(p_arr+i));
	}
	i++;
	fprintf(fp, "%.15f", *(p_arr+i));
	fclose(fp);
}
