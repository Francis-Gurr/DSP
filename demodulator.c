#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(int argc, char *argv[]){

	FILE *fin_diff, *fin_sum, *fout_diff, *fout_sum;
	int n;
	float fs_in = 10;
	float[] f = {1.05, 1}, 
		in = new float[2],
		out = new float[2],
		loc_osc = new float[2], 
		fcalc = new float[2];

	// check if we have 6 arguments as required
	if(argc!=6){
		printf("ERROR: wrong number of parameters\n");
		printf("USE: %s diff_in_file sum_in_file diff_out_file sum_out_file n\n",argv[0]);
		exit(1);
	}

	// open input diff file ==> TEMPORARY TESTING
	fin_diff=fopen(argv[1],"rb");
	if(fin_diff == NULL) {
		printf("ERROR: %s does not exist\n", argv[1]);
		exit(1);
	}
	printf("DIFF input file: %s \n", argv[1]);

	// open input sum file ==> TEMPORARY TESTING
	fin_sum=fopen(argv[2],"rb");
	if(fin_sum == NULL) {
		printf("ERROR: %s does not exist\n", argv[2]);
		exit(1);
	}
	printf("SUM input file: %s\n", argv[2]);

	// open output diff file ==> TEMPORARY TESTING
	fout_diff=fopen(argv[3], "w+b");
	if(fout_diff == NULL) {
		printf("ERROR: %s cannot be created\n", argv[3]);
		exit(1);
	}
	printf("DIFF output file: %s\n", argv[3]);

	// open output sum file ==> TEMPORARY TESTING
	fout_sum=fopen(argv[4], "w+b");
	if(fout_sum == NULL) {
		printf("ERROR: %s cannot be created\n", argv[4]);
		exit(1);
	}
	printf("SUM output file: %s\n", argv[4]);

	// assign n to be the current value so we can create the local oscillator
	if(argv[5] == "0") {
		n=0;
	}
	else {
		n=atoi(argv[5]);
		if (n == 0) {
			printf("ERROR: n must be an integer value\n");
			exit(1);
		}
	}

	printf("processing ...\n");

	// keep reading a float from each file until the end
	while(fread(&in[0], sizeof(float), 1, fin_diff) && fread(&in[1], sizeof(float), 1, fin_sum)) {
		n++;
		// create a local oscillator and multiply the input by it
		for(int i=0; i<2; i++) {
			fcalc[i] = 2 * PI * (f[i]/fs_in) * n;
			loc_osc[i] = cosf(fcalc[i]);
			out[i] = loc_osc[i] * in[i];
		}

		// write output to file
		fwrite(&out[0], sizeof(float), 1, fout_diff);
		fwrite(&out[1], sizeof(float), 1, fout_sum);
	}

	printf("done\n");

	// close the files
	fclose(fin_diff);
	fclose(fin_sum);
	fclose(fout_diff);
	fclose(fout_sum);

	exit(0)
}
