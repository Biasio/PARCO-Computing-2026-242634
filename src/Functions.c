#include "Functions.h"

void IsPointerNull (void* ptr) {
	if(ptr==NULL)					 
	{
		printf("Error! pointer is NULL.\n");
		exit(1);
	}
}

VECTOR* init_VEC (const int rows){
	VECTOR* ptr = malloc(sizeof(VECTOR*));
	
	ptr->length=rows;
	ptr->val=(double*)malloc(rows*(sizeof(double)));
	IsPointerNull(ptr);

	return ptr;
}

VECTOR* init_rand_VEC(const int rows){
	VECTOR* ptr=NULL;

	ptr = init_VEC(rows);

	#if LOGGING
	printf("\nGenerating a random vector...\n");
	#endif

	for (int i=0; i<rows; ++i){
			ptr->val[i] =(double) (rand() % (RAND_max - RAND_min + 1)) + RAND_min;

			#if LOGGING
			printf(SPACING_FLOAT, ptr->val[i]);
			printf("\n");
			#endif
	}
	#if LOGGING
	printf("\n...Done\n");
	#endif
	return ptr;
}


void print_VEC(VECTOR* ptr){

	IsPointerNull(ptr);

	for (int i=0; i<ptr->length; ++i){
			printf(SPACING_FLOAT, ptr->val[i]);
		printf("\n");
	}
	return;
}

void clean_VECTOR (VECTOR* ptr){
	IsPointerNull(ptr);
	
	free(ptr->val);
	free(ptr);
	return;
}

void clean_CSR (CSR_MATRIX* ptr){
	IsPointerNull(ptr);
	
	free(ptr->indptr);
	free(ptr->col_index);
	free(ptr->val);
	free(ptr);
	return;
}

void print_CSR_matrix(CSR_MATRIX* ptr){

	IsPointerNull(ptr);

	printf("\nNon Zero values count: %d\n", ptr->nnz);
	//print first array
	printf("\nindPtr :");
	
	for (int i=0; i< (ptr->indptr_length); ++i){
		printf(SPACING, (ptr->indptr[i]) );
	}
	printf("\nindCol :");
	for (int j=0; j< (ptr->nnz); ++j){
			printf(SPACING, (ptr->col_index[j]) );
		}
	printf("\nValues :");
	for (int k=0; k< (ptr->nnz); ++k){
		printf(SPACING_FLOAT, (ptr->val[k]) );
	}
	return;
}

CSR_MATRIX* init_CSR_MATRIX (const int indptr_length, const int nnz, const int col_length){

	CSR_MATRIX* ptr=NULL;
	
	ptr=(CSR_MATRIX*) malloc(sizeof(CSR_MATRIX));
	IsPointerNull(ptr);

	//allocate indptr array
	if(indptr_length > 0){
		ptr->indptr = (int*)malloc(indptr_length*(sizeof(int))); 
		IsPointerNull(ptr->indptr);
	}
	//allocate col_index array
	if(nnz>0){
		ptr->col_index = (int*)malloc(nnz*(sizeof(int))); 
		IsPointerNull(ptr->col_index);

		//allocate val array
		ptr->val = (double*)malloc(nnz*(sizeof(double))); 
		IsPointerNull(ptr->val);
	}
	
	// assign other struct var values
	ptr->indptr_length=indptr_length;
	ptr->nnz=nnz;
	ptr->col_length=col_length;
	
	//init the indptr array to 0
	for(int i=0; i<indptr_length; ++i){
		ptr->indptr[i]=0;
	}
	return ptr;
}

VECTOR* CSR_matrix_and_VEC_mult(CSR_MATRIX* CSR_matrix, VECTOR* vec){

	int nnz_row, nnz_curr;
	VECTOR* result_vec;

	IsPointerNull(CSR_matrix);
	IsPointerNull(vec);
	
	if ( CSR_matrix->col_length != vec->length ){
		printf("\nERROR: Cannot multiply CSR matrix and vec, number of cols isn't the same of vector's rows\n");
		exit(1);
	}

	result_vec=init_VEC(CSR_matrix->indptr_length -1);

	for(int i=0; i < (CSR_matrix->indptr_length -1); ++i){ //i is always the current row index (CSR) we need for calculating
			
		nnz_row = CSR_matrix->indptr[(i+1)] - CSR_matrix->indptr[i]; // calculation of number of nnz elements in the curr row of csr

		result_vec->val[i] = 0;  // set the current value to zero
		
		if ( vec->val[i] != 0 && nnz_row != 0 ){ //There's at least a non zero elements in this row,  OPTIMIZATION DONE with ||
				
			for( int j=0 ; j < nnz_row; ++j){
				nnz_curr = ( CSR_matrix->indptr[i] + j );   //nnz values already processed
				result_vec->val[i] += (vec->val[(CSR_matrix->col_index[nnz_curr])]) * (CSR_matrix->val[nnz_curr]); 
			}
		} 
	}

	#if LOGGING
		printf("\nMultiplying the CSR matrix with a vector and printing the resulting vector...\n");
		print_VEC(result_vec);
		printf("\n...Done\n");
	#endif
	return result_vec;
}

// compare function to pass to qsort, if rows are identical then sort by column the coo entries 
int cmp_coo_entries(const void *a, const void *b)
{
	const COO_ENTRY *A = a;
	const COO_ENTRY *B = b;

	//compare rows
	if (A->row < B->row) return -1;
	if (A->row > B->row) return  1;

	// same row, so compare columns
	if (A->col < B->col) return -1;
	if (A->col > B->col) return  1;

	return 0; // A and B are at the same element
}


CSR_MATRIX* read_matrix_MTX(char* file_path, int* rows, int* cols, int* nnz){

	int *rows_array;
	int *cols_array;
	double *val_array;
	MM_typecode matcode;   // header description of matrix   
	
	if ( mm_read_mtx_crd(file_path, rows, cols, nnz, &rows_array, &cols_array, &val_array, &matcode) != 0 ){
		fprintf(stderr, "\nError: Could not read the matrix file %s\n", file_path);
		exit(1);
	}
	#if LOGGING
		printf("\n Read matrix with size:%dx%d  and  nnz:%d\n", *rows, *cols, *nnz);
	#endif
	// CHECK FOR COMPLEX ELEMENTS
	if (mm_is_complex(matcode) && mm_is_matrix(matcode) && 
			mm_is_sparse(matcode) )
	{
		printf("Sorry, this application does not support ");
		printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
		free(rows_array);
		free(cols_array);
		if(val_array!=NULL) free(val_array);
		exit(1);
	}
		
	COO_ENTRY *coo = malloc((*nnz)*sizeof(COO_ENTRY));
	IsPointerNull(coo);
	
	for(int i=0; i< *nnz; ++i){ //-1 because mtx file is 1 indexed
		coo[i].row = rows_array[i] -1;

		coo[i].col = cols_array[i] -1;

		coo[i].val = val_array[i];

	}
	free(rows_array);
	free(cols_array);
	free(val_array);
	
	//order the coo entries using  qsort(*arr, n, size, *comp);
	qsort(coo, *nnz, sizeof(COO_ENTRY), cmp_coo_entries);
	
	// Init a CSR matrix without indptr set
	CSR_MATRIX* CSR_matrix = init_CSR_MATRIX( (*rows) + 1, *nnz, *cols);

	for(int j=0; j < *nnz; ++j){
		CSR_matrix->col_index[j] = coo[j].col;
		CSR_matrix->val[j] = coo[j].val;

		++CSR_matrix->indptr[(coo[j].row + 1)]; 	//add a 1 for each nnz value of the coo matrix (scrlling through rows) in the indptr[row index +1] array
	}
	// clean coo
	free(coo);
	
	// make the cumulative sum of elements in indptr
	for(int p=1; p< (CSR_matrix->indptr_length); ++p){
		CSR_matrix->indptr[p] += CSR_matrix->indptr[(p-1)];
	}
	
	#if LOGGING
	printf("\n Read CSR matrix:\n");
	print_CSR_matrix(CSR_matrix);
	#endif
	
	return CSR_matrix;
}


uint64_t time_nano(){
	struct timespec time_struct;
	uint64_t time_now;

	clock_gettime(CLOCK_MONOTONIC, &time_struct);
	time_now = (uint64_t) (time_struct.tv_sec * ((long) (1000000000)) + time_struct.tv_nsec);
	
	return time_now;
}
