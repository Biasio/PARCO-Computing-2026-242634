#include "Functions.h"


int main(int argc, char *argv[]){
	FILE* FILE_PTR=NULL;
    
    int** MATRIX_A;
    int* VEC_B;
    int* RESULT_VEC;
	CSR_MATRIX* CSR_MATRIX_A;	
	uint64_t t0, t1;

	if (argc < 2) {
        fprintf(stderr, "Usage: main <path-to-matrix>\n You can append ../MATRICES/ to the name of the matrix for simplicity\n", argv[0]);
        return EXIT_FAILURE;
    }
	
   
	
    srand(time(0));
	
	FILE_PTR = fopen("../MATRICES/matrix1.txt","r");      //opens the input matrix in read mode
	//INIT CSR MATRIX 
	MATRIX_A = read_matrix(FILE_PTR, ROWS_A, COLS_A);


	t0 = time_nano();
		CSR_MATRIX_A = MATRIX_to_CSR_converter(MATRIX_A, ROWS_A, COLS_A);
	
		//INIT CSR MATRIX B
		VEC_B = init_rand_VEC(ROWS_VEC);
	
		RESULT_VEC = CSR_matrix_and_VEC_mult(CSR_MATRIX_A,VEC_B);

	t1 = time_nano();

	t0 = t1-t0;
	printf("\nSerial execution took %" PRIu64 " nanoseconds\n", t0);
	
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
