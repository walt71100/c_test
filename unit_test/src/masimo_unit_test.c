/*
 ============================================================================
 Name        : masimo_unit_test.c
 Author      : Walt White (walt.j.white@gmail.com)

 Description : C implementation of the Unit Test for the summation algorithm
 	 	 	 : implementation of the "Software Engineer Test"
 	 	 	 : as provided by Masimo via email on Oct 14th 2013.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXMATSIZE 500
#define EXIT_FAILURE -1
#define EXIT_OK 0

int main(int argc, char *argv[])
{
	// declare local variables and arrays
    double doublesData[MAXMATSIZE];			// values read from summation output file
    double doublesII[MAXMATSIZE][MAXMATSIZE];		// internally calculated matrix values
    double matrixDataElement[MAXMATSIZE][MAXMATSIZE]; 	// externally read matrix values
    double temp_accum = 0;
    int i,j,k,c;
    int matRow = 0;
    int matCol = 0;
    int matSize = 0;
    int strpos = 0;
    int numElementEnds = 0;
    char numString[50]; //  buffer to hold data elements read from text file while parsing output file strings
    char* pEnd; // string to double ptr strtod(s,d);
    FILE * fp_out;
    FILE * fp_in;
    char * line = NULL;
    char * parsed_element = NULL;
    size_t len = 0;
    ssize_t read;
    int debug = 0;
    time_t current_time;
    char* c_time_string;

    // check for proper parameter count as input to the tool
    if(argc<2 ) {
    	printf("\n\nUsage: masimo_unit_test processed_results_file data_input_file [-v=verbose output]\n\n");
   	return EXIT_FAILURE;
    }

    // output unit test information and current time to the console
	current_time = time(NULL);
	c_time_string = ctime(&current_time);
	printf("\n\n3D Summation Algorithm Unit Test:");
	printf("\nExecution time is: %s", c_time_string);

    // if specified, produce verbose output to console including runtime timestamp
    if(argc>3) {
    	if(strncmp(argv[3], "-v", 2)==0) {
    		debug = 1;
    	}
    }

   if(debug){  // log command line parameters to the console
	   printf("\nCommand Line Parameters are:\n");
    	for(i=0; i<argc; i++) {
    		printf("\n%d %s ",i, argv[i]);
    	}
    printf("\n");
    }

    fp_out = fopen(argv[1], "r");
    if (fp_out == NULL){
    	printf("\nError opening output file!\nExiting...\n\n");
        exit(EXIT_FAILURE);
    }

    // read in the input data from the file and display it as its read in,
    // along with the final count
    int dataCount = 0;
    if(debug) printf("\nInput Data:\n");
    while ((read = getline(&line, &len, fp_out)) != -1) {
    	if(debug) printf("\n\String values read from next line of file %s are: %s", argv[1], line); // print in string format
    	// parse line into space diliminated floating point elements
    	for(strpos = 0; strpos < strlen(line);strpos++) {
    		if(line[strpos] == 32) { //search for space character
    			memset(numString,'\0',sizeof(numString)); // clear numString contents
    			parsed_element = strncpy(&numString, &line[numElementEnds], strpos-numElementEnds);
    			numElementEnds = strpos+1; // save the new string index pointer
    			matrixDataElement[matRow][matCol++] = strtod(parsed_element, &pEnd);
            	if(debug) printf("\nDataElement[%d][%d]: %4.6f", matrixDataElement[matRow][matCol-1],matRow,matCol-1);
    		}
    	}
    	numElementEnds = 0;
    	matRow++;
    	matSize = matCol;
    	matCol=0;
    }

    printf("\n\nUUT File Values Read from file: %s\n",argv[1]);
    for(matRow=0;matRow<matSize;matRow++){
    	for(matCol=0;matCol<matSize;matCol++){
    		printf("%4.6f ",matrixDataElement[matRow][matCol]);
    	}
    	printf("\n");
    }

    // open and read input file so independent calculations can be made and checked against the output file
    fp_in = fopen(argv[2], "r");
    if (fp_in == NULL){
    	printf("\nError opening input file!\nExiting...\n\n");
        exit(EXIT_FAILURE);
    }
    // read in the input data from the file and display it as its read in,
    // along with the final count
    dataCount = 0;
    if(debug) printf("\nInput Data from file: %s:\n",argv[2]);
    while ((read = getline(&line, &len, fp_in)) != -1) {
        doublesData[dataCount++] = strtod(line, &pEnd); // print in double format
        if(debug){
        	//printf("string value: %s", line); // print in string format
        	printf("%4.14f\n", doublesData[dataCount-1]);
        }
    }

    if(debug) printf("\nTotal of %d input values read from input file: %s.\n\n",dataCount,argv[2]);
    c = matSize-1;
    for(k=0;k<c+1;k++){
    	for(j=0;j<c+1;j++){
    		for(i=c; i<dataCount - 1 + 1;i++) {
    			temp_accum += doublesData[i-k]*doublesData[i-j];
    		}
    		doublesII[k][j] = temp_accum;
    		temp_accum = 0;
    	}
    }

    // print out the results to the console
    printf("\nCalculated Matrix based on inputs from input file: %s.\n",argv[2]);
    for(k=0;k<c+1;k++){
    	for(j=0;j<c+1;j++){
    	    if(debug){
    	    	printf("%4.14f ", doublesII[k][j]); // output in double precision since the input data is such
    	    }
    	    else {
    	    	printf("%4.6f ", doublesII[k][j]); // provide the value in single precision per the requirement
    	    }
    	}
    	printf("\n");
    }

    if(debug) printf("\n\nChecking each element in the output file: %s, against internally the calculated values:",argv[1]);
    int error_count = 0;
    for (k = 0;k<c+1;k++){
        for (j=0;j<c+1;j++){
            if (abs(matrixDataElement[k][j] - doublesII[k][j]) < 1E-6) {
                if(debug) printf("\nelement %d,%d passed",k,j);
            }
            else{
            	printf("\nelement %d,%d out of range",k,j);
                error_count += 1;
            }
        }
    }

    if(error_count == 0){
        printf("\n\nTest Passed\n");
    }
    else{
        printf("\n\nTest Failed\n");
    }

    return 0;
}

