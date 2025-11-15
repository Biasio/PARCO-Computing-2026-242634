
#define FILENAME "matrix1.txt"

#define SPARSITY 30   // Define level of sparsity in % for the write rand matrix to file function
#define SYMMETRIC 1 //ON=1 OFF=0
#define SUBMATRIX 1 //ON=1 OFF=0 If this is ON it ignores symmetric
#define ROWS 1000
#define COLS 1000

#define SUB_ROWS 110 //should not be greater than ROWS
#define SUB_COLS 50 //should not be greater than COLS

#define RAND_min 1  //min random range value
#define RAND_max 100  //max random range value

#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
	
	
	FILE* FILE_PTR=NULL;

	FILE_PTR = fopen(FILENAME,"w");
	
	if (FILE_PTR == NULL) {
		printf("ERROR: file stream pointer passed to \"WRITE_rand_MATRIX_to_file\" is NULL\n");
		exit(1);
	}
	
	
	
	srand(time(0));

	
	#if !SYMMETRIC && !SUBMATRIX
	printf("\nWriting a non symmetric, random MATRIX (size %dx%d) with sparsity 0.%d to ", ROWS, COLS, SPARSITY);
	printf(FILENAME);
	
	for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
			int value;
			
			if( (rand()%100) < SPARSITY	){
				value = (rand() % (RAND_max - RAND_min + 1)) + RAND_min;
			} else {
				value = 0;
			}
			fprintf(FILE_PTR, "%d", value);
			if (j!= (COLS-1)) fprintf(FILE_PTR, " ");
		}
		if(i != (ROWS-1)) fprintf(FILE_PTR, "\n");
	}
	#endif

	#if SYMMETRIC && !SUBMATRIX
	printf("\nWriting a symmetric, random MATRIX (size %dx%d) with sparsity 0.%d to ", ROWS, COLS, SPARSITY);
	printf(FILENAME);
	
	int matrix[ROWS][COLS];
	
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			
			int value;

			if (i <= j) {
				if ((rand() % 100) < SPARSITY) {
					value = (rand() % (RAND_max - RAND_min + 1)) + RAND_min;
				} else {
					value = 0;
				}
				matrix[i][j] = value;     // store it
			} else {

				value = matrix[j][i];
			}

			fprintf(FILE_PTR, "%d", value);
			if (j != COLS - 1) fprintf(FILE_PTR, " ");
		}
		if (i != ROWS - 1) fprintf(FILE_PTR, "\n");
	}
	#endif

	#if SUBMATRIX
	printf("\nWriting a submatrix of nz values of size %dx%d inside a full zero matrix ", SUB_ROWS, SUB_COLS);
	printf(FILENAME);
	
	if(SUB_ROWS>ROWS || SUB_COLS>COLS){
		printf("ERROR: Sub-matrix is bigger than matrix itself");
		exit(1);
	}

	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			int value = 0;

			if (i < SUB_ROWS && j < SUB_COLS) {
				value = (rand() % (RAND_max - RAND_min + 1)) + RAND_min;
			}

			fprintf(FILE_PTR, "%d", value);

			if (j != COLS - 1) fprintf(FILE_PTR, " ");
		}

		fprintf(FILE_PTR, "\n");  // end of row
	}
	
	#endif
	
	printf("\n...Done\n");

	if( fclose(FILE_PTR) != 0){
		printf("\nERROR: File stream closed UNsuccessfully\n");
		exit(1);
	}

}

