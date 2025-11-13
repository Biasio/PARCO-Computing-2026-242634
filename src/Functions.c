#include "Functions.h"

void IsPointerNull (void* ptr) {
	if(ptr==NULL)                     
    {
        printf("Error! pointer is NULL.\n");
        exit(1);
    }
}


int** init_MATRIX (const int rows_a, const int cols_a){
	int** ptr = NULL;
	ptr=(int**)malloc(rows_a*(sizeof(int*)));
	IsPointerNull(ptr);

	for(int i=0; i<rows_a; ++i){
		ptr[i]=(int*)malloc(cols_a*(sizeof(int)));
		IsPointerNull(ptr[i]);
	}
	return ptr;
}


int** init_rand_matrix(const int rows, const int cols){
	int** ptr=NULL;

	ptr = init_MATRIX(rows,cols);

	#if LOGGING
	printf("\nGenerating a random matrix...\n");
	#endif

	for (int i=0; i<rows; ++i){
			for (int j=0; j<cols; ++j){
			ptr[i][j] = (rand() % (RAND_max - RAND_min + 1)) + RAND_min;

			#if LOGGING
			printf(SPACING, ptr[i][j]);
			#endif
		}
	}
	#if LOGGING
	printf("\n...Done\n");
	#endif
	return ptr;
}


int* init_VEC (const int rows){
	int* ptr = NULL;

	ptr=(int*)malloc(rows*(sizeof(int)));
	IsPointerNull(ptr);
	
	return ptr;
}

int* init_rand_VEC(const int rows){
	int* ptr=NULL;

	ptr = init_VEC(rows);

	#if LOGGING
	printf("\nGenerating a random vector...\n");
	#endif

	for (int i=0; i<rows; ++i){
			ptr[i] = (rand() % (RAND_max - RAND_min + 1)) + RAND_min;

			#if LOGGING
			printf(SPACING, ptr[i]);
			printf("\n");
			#endif
	}
	#if LOGGING
	printf("\n...Done\n");
	#endif
	return ptr;
}





int** read_matrix(FILE* file_ptr, const int rows_a, const int cols_a) {
    int i, j;
    int** matrix_a = NULL;

    matrix_a = init_MATRIX(rows_a, cols_a); 
    
    if (file_ptr == NULL) {
		printf("ERROR: file stream pointer is NULL\n");
		exit(1);
	}

    #if LOGGING
    printf("\nScanning file to read a matrix...\n");
	#endif
	
    for (i = 0; i < rows_a; ++i) {
        for (j = 0; j < cols_a; ++j) {
            
            if (fscanf(file_ptr, "%d", &matrix_a[i][j]) != 1) {

                #if LOGGING
                printf("\nWARNING: EOF reached, or character not recognized in position [%d][%d], before completing the matrix. Completing the rest with zeroes'''.\n", i, j);
                #endif
                
                while (i<rows_a){
					while(j<cols_a){
						matrix_a[i][j]=0;
						++j;
					}
					++i;
				}

				return matrix_a;
                }
            }
    }
    
    #if LOGGING
    print_matrix(matrix_a, ROWS_A, COLS_A);
    printf("\n...Done\n");
	#endif

	fseek(file_ptr, 0, SEEK_SET);
    return matrix_a;
}


void clean_matrix (int **ptr, const int rows){
    IsPointerNull(ptr);
	
	for (int i=0; i<rows; ++i){
		free(ptr[i]);
	}
	free(ptr);
}


void print_matrix(int** ptr, const int rows, const int cols){

    IsPointerNull(ptr);
	
    for (int i=0; i<rows; ++i){
	    for (int j=0; j<cols; ++j){
			printf(SPACING, ptr[i][j]);
		}
		printf("\n");
	}
	return;
}

void print_VEC(int* ptr, const int rows){

    IsPointerNull(ptr);

    for (int i=0; i<rows; ++i){
			printf(SPACING, ptr[i]);
		printf("\n");
	}
	return;
}

void print_CSR_matrix(CSR_MATRIX* ptr){

    IsPointerNull(ptr);

	printf("\nNon Zero values count: %d\n", ptr->data[0][(ptr->indptr_length -1)]);
	//print first array
	printf("\nindPtr :");
	
    for (int i=0; i< (ptr->indptr_length); ++i){
	    printf(SPACING, (ptr->data[0][i]) );
	}
	printf("\nindCol :");
	for (int j=0; j< (ptr->col_and_nz_length); ++j){
			printf(SPACING, (ptr->data[1][j]) );
		}
	printf("\nValues :");
    for (int k=0; k< (ptr->col_and_nz_length); ++k){
	    printf(SPACING, (ptr->data[2][k]) );
	}
	return;
}



CSR_MATRIX* init_CSR_MATRIX (const int indptr_length, const int col_and_nz_length){

	CSR_MATRIX* ptr=NULL;
	
	ptr=(CSR_MATRIX*) malloc(sizeof(CSR_MATRIX));
	IsPointerNull(ptr);
	ptr->data=(int**)malloc(3*(sizeof(int*))); //allocate 3 arrays of int*
	IsPointerNull(ptr->data);

	//allocate indptr array
	ptr->data[0]=(int*)malloc(indptr_length*(sizeof(int)));
	IsPointerNull(ptr->data[0]);
	
	//allocate col and value arrays
	ptr->data[1]=(int*)malloc(col_and_nz_length*(sizeof(int)));
	IsPointerNull(ptr->data[1]);
	ptr->data[2]=(int*)malloc(col_and_nz_length*(sizeof(int))); 
	IsPointerNull(ptr->data[2]);

	ptr->indptr_length=indptr_length;
	ptr->col_and_nz_length=col_and_nz_length;
	//init every indptr element to 0
	for(int i=0; i<indptr_length; ++i){
		ptr->data[0][i]=0;
	}

	return ptr;
}



CSR_MATRIX* MATRIX_to_CSR_converter(int** matrix_a, const int rows_a, const int cols_a){

		// Allocate a temporary CSR struct with the same COO length, that will be shrinked in size later into CSR_matrix
		int k, curr_row;
		
		int** COO_matrix_tmp = NULL;
		CSR_MATRIX* CSR_matrix = NULL;

		//the max size ishould be as if the matrix has all nnz values
		COO_matrix_tmp = init_MATRIX(3, (rows_a*cols_a));
		
		// first fetch the matrix for nz values (COO format)
		k=0;
		for(int i=0; i<rows_a; ++i){
			for(int j=0; j<cols_a; ++j){
				if(matrix_a[i][j] != 0){
					COO_matrix_tmp[0][k]=i; //store row in first array
					COO_matrix_tmp[1][k]=j; //store col in second array
					COO_matrix_tmp[2][k]=matrix_a[i][j]; //write nnz value in the third array
					++k; //stores nnz value
				}	
			}
		}

		//init of CSR matrix
		if(k==0){
			printf("\nERROR: conversion to CSR of an empty matrix is not permitted!\n");
		}
		CSR_matrix=init_CSR_MATRIX( (rows_a + 1), k);
		CSR_matrix->data[0][(k-1)]=k; //set the last indptr to nnz

		#if LOGGING
		printf("\nFirst converting it to COO matrix format...\n");
		print_matrix(COO_matrix_tmp, 3, k);
		printf("\n...Done\n");
		#endif
		
		//copy the col and nz val array from tmp COO matrix
		for(int c=0; c<k; ++c){
			CSR_matrix->data[1][c] = COO_matrix_tmp[1][c];
			CSR_matrix->data[2][c] = COO_matrix_tmp[2][c];
		}
		// we can assume the row array is ordered, because parsing of COO is done row by row
		curr_row=0;
		for(int w=0; w<k; ++w){
			curr_row = ( COO_matrix_tmp[0][w] + 1 );  // increment by one because indptr stored value are with an offset of 1 as far as index and original matrix row numbers are concerned.
			CSR_matrix->data[0][curr_row] += 1;
			
		}
		// make the cumulative sum of elements
		for(int p=1; p< (CSR_matrix->indptr_length); ++p){
			CSR_matrix->data[0][p] += CSR_matrix->data[0][(p-1)];
		}
		
		//clean the tmp matrix
		clean_matrix(COO_matrix_tmp,3);

		#if LOGGING
		printf("\nConverting it to CSR matrix format...\n");
		print_CSR_matrix(CSR_matrix);
		printf("\n...Done\n");
		#endif
		return CSR_matrix;
}


CSR_MATRIX* init_rand_CSR_matrix(const int rows, const int cols){

	int** random_matrix=NULL;
	CSR_MATRIX* CSR_matrix=NULL;
	
	random_matrix = init_rand_matrix(rows,cols);
	CSR_matrix = MATRIX_to_CSR_converter(random_matrix, rows, cols);
	free (random_matrix);
	
	return ( CSR_matrix );
}


int* CSR_matrix_and_VEC_mult(CSR_MATRIX* CSR_matrix, int* vec){

	int nnz_row, nnz_curr;
	int* result_vec;

	IsPointerNull(CSR_matrix);
	IsPointerNull(vec);
		
	if ( COLS_A != ROWS_VEC ){  //JUST A PRECAUTION, if preprocessor directives is changed
		printf("\nERROR: Cannot multiply CSR matrix and vec, number of cols isn't the same of vector's rows\n");
		exit(1);
	}

	result_vec=init_VEC(ROWS_A);
	
	for(int i=0; i < (CSR_matrix->indptr_length -1); ++i){ //i is always the current row index (CSR) we need for calculating
			
		nnz_row = CSR_matrix->data[0][(i+1)] - CSR_matrix->data[0][i]; // calculation of number of nnz elements in the curr row of csr

		result_vec[i] = 0;  // set the current value to zero
		
		if ( vec[i] != 0 && nnz_row != 0 ){ //There's at least a non zero elements in this row,  OPTIMIZATION DONE with ||
				
			for( int j=0 ; j < nnz_row; ++j){
				nnz_curr = ( CSR_matrix->data[0][i] + j );   //nnz values already processed
				result_vec[i] += vec[ (CSR_matrix->data[1][nnz_curr]) ] * (CSR_matrix->data[2][nnz_curr]); 
			}
		} 
	}

	#if LOGGING
		printf("\nMultiplying the CSR matrix with a vector and printing the resulting vector...\n");
		print_VEC(result_vec, ROWS_A);
		printf("\n...Done\n");
	#endif
	return result_vec;
}

void WRITE_rand_MATRIX_to_file(FILE* file_ptr, const int rows, const int cols, const int sparsity){

	int value;

	if (file_ptr == NULL) {
		printf("ERROR: file stream pointer passed to \"WRITE_rand_MATRIX_to_file\" is NULL\n");
		exit(1);
	}

	#if LOGGING
	printf("\nWriting a rand MATRIX with sparsity %d to the specified file...\n", sparsity);
	#endif
	
	for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
			if( (rand()%100) < sparsity	){
				value = (rand() % (RAND_max - RAND_min + 1)) + RAND_min;
			} else {
				value = 0;
			}
			fprintf(file_ptr, "%d", value);
			if (j!= (cols-1)) fprintf(file_ptr, " ");
		}
		if(i != (rows-1)) fprintf(file_ptr, "\n");
	}
	
	#if LOGGING
	printf("\n...Done\n");
	#endif

	fseek(file_ptr, 0, SEEK_SET);
	return;
}
