#include "Functions.h"


int main(int argc, char *argv[]) {

	int ROWS_DIM, COLS_DIM, NNZ;
	CSR_MATRIX* CSR_MATRIX_A;
	VECTOR *VEC_B, *RESULT_VEC;
	
	//CSR_MATRIX CSR_matrix = (CSR_MATRIX*) malloc(sizeof(CSR_MATRIX*));
	uint64_t t0, t1;

	if (argc < 2) {
		fprintf(stderr, "\nUsage: %s <path-to-matrix>\n You can append ../MATRICES/ to the name of the matrix for simplicity\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	srand(time(0));
	

	

	t0 = time_nano();

		//INIT CSR MATRIX 
		CSR_MATRIX_A = read_matrix_MTX(argv[1], &ROWS_DIM, &COLS_DIM, &NNZ);
		
		//INIT CSR MATRIX B
		VEC_B = init_rand_VEC(ROWS_DIM);
	
		RESULT_VEC = CSR_matrix_and_VEC_mult(CSR_MATRIX_A,VEC_B);
		
	t1 = time_nano();

	t0 = t1-t0;
	printf("\nSerial execution took %" PRIu64 " nanoseconds\n", t0);
	
	//CLEAN ON EXIT
	#if LOGGING
	printf("\n\nCleaning memory....");
	#endif

	clean_CSR(CSR_MATRIX_A);
	#if LOGGING
		printf("Done\n");
	#endif

	return 0;
}
