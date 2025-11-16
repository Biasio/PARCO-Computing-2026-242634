#ifndef FUNCTIONS_H
#define FUNCTIONS_H


// PREPROCESSOR COSTANTs

#define LOGGING 1 //ON=1 OFF=0
#define SPACING "%6d "  //Define the spacing (%*d) of printf for int matrix elements 
#define SPACING_FLOAT "%10.7f "

#define ROWS_A 200
#define COLS_A 200
#define ROWS_VEC COLS_A

#define RAND_min 0  //min random range value
#define RAND_max 100  //max random range value


// INCLUDED LIBRARIEs
#include <inttypes.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"

// CUSTOM DATA STRUCTUREs

typedef struct csr_matrix { //struct for storing Compressed Sparse Row matrices
		int indptr_length; 
		int nnz;
		int col_length;
		int* indptr;
		int* col_index;
		double* val;
} CSR_MATRIX;

typedef struct {
	int row;
	int col;
	double val;
} COO_ENTRY;

typedef struct {
	int length;
	double* val;
} VECTOR;


// FUNCTIONs PROTOTYPEs

void IsPointerNull(void* ptr);									// check's if the pointer passed is a NULL pointer, in that

VECTOR* init_VEC (const int rows);

VECTOR* init_rand_VEC(const int rows);

void print_VEC(VECTOR* ptr);

void clean_VECTOR (VECTOR* ptr);

void clean_CSR (CSR_MATRIX* ptr);	// free memory of a dynamically allocated CSR_MATRIX STRUCT

void print_CSR_matrix(CSR_MATRIX* ptr); //print csr struct  matrix

CSR_MATRIX* init_CSR_MATRIX (const int indptr_length, const int nnz, const int col_length); //dynamically allocate a CSR struct, remember to count rows+1 elemnts in indptr_length!

VECTOR* CSR_matrix_and_VEC_mult(CSR_MATRIX* CSR_matrix, VECTOR* vec);

int cmp_coo_entries(const void *a, const void *b);

CSR_MATRIX* read_matrix_MTX(char* file_path, int* rows, int* cols, int* nnz);

// MEASUREMENTS FUNCTIONS

uint64_t time_nano();

#endif
