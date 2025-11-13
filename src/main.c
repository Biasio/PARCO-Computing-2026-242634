#include "Functions.h"

//#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int main(){

    FILE* FILE_PTR=NULL;
    
    int** MATRIX_A;
    int* VEC_B;
    int* RESULT_VEC;
	CSR_MATRIX* CSR_MATRIX_A;	
	
    srand(time(0));

	#if RANDOMIZE_FILE
	FILE_PTR = fopen("matrix.txt","w");      //opens the input matrix in write mode
	WRITE_rand_MATRIX_to_file(FILE_PTR,ROWS_A,COLS_A,SPARSITY);
	    if( fclose(FILE_PTR) != 0){
		printf("\nERROR: File stream closed UNsuccessfully\n");
		exit(1);
	}
	#endif

	FILE_PTR = fopen("matrix.txt","r");      //opens the input matrix in read mode
	//INIT CSR MATRIX A
	MATRIX_A = read_matrix(FILE_PTR, ROWS_A, COLS_A);

	CSR_MATRIX_A = MATRIX_to_CSR_converter(MATRIX_A, ROWS_A, COLS_A);
	
	//INIT CSR MATRIX B
	VEC_B = init_rand_VEC(ROWS_VEC);
	
	RESULT_VEC = CSR_matrix_and_VEC_mult(CSR_MATRIX_A,VEC_B);
	
	
	//CLEAN ON EXIT
	#if LOGGING
	printf("\n\nCleaning memory....");
	#endif
	
    clean_matrix(MATRIX_A, ROWS_A);
    clean_matrix(CSR_MATRIX_A->data, 3);
    free(CSR_MATRIX_A);
    free(VEC_B);
    free(RESULT_VEC);

 
	
    if( fclose(FILE_PTR) != 0){
		printf("\nERROR: File stream closed UNsuccessfully\n");
		exit(1);
	}

	#if LOGGING
		printf("Done\n");
	#endif
    return 0;
}
