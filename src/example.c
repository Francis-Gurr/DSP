#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

	FILE *fin1, *fin2, *fout;
	float in1, in2, out;

	// check if we have 3 arguments - remember that the first one is the file name
	if(argc!=4){
		printf("ERROR: not enough input parameters\n");
		printf("USE: %s input1 input2 output\n",argv[0]);
		exit(1);
	}

	// open input files as binary read-only
	fin1=fopen(argv[1],"rb");
	if(fin1 == NULL) {
		printf("ERROR: %s does not exist\n",argv[1]);
		exit(1);
	}

	fin2=fopen(argv[2],"rb");
	if(fin2 == NULL) {
		printf("ERROR: %s does not exist\n",argv[2]);
		exit(1);
	}

	// open output files as binary - overwrite the file if it alredy exists
	fout=fopen(argv[3],"w+b");
	if(fout == NULL) {
		printf("ERROR: %s cannot be created\n",argv[1]);
		exit(1);
	}

	printf("processing ...\n");

	// keep reading a float in each file until the end
	while(fread(&in1, sizeof(float), 1, fin1) && fread(&in2, sizeof(float), 1, fin2)){
		// add the inputs and write the sum to the output
		out = in1+in2;
		fwrite(&out, sizeof(float), 1, fout);
	}

	printf("done\n");

	// close the files
	fclose(fin1);
	fclose(fin2);
	fclose(fout);

	exit(0); // all is good
}
