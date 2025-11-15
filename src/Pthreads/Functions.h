#ifndef FUNCTIONS_H
#define FUNCTIONS_H


// PREPROCESSOR COSTANTs

#define LOGGING 1 //ON=1 OFF=0
#define SPACING "%6d "  //Define the spacing (%*d) of printf for matrix elements 

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

// CUSTOM DATA STRUCTUREs

typedef struct csr_matrix { //struct for storing Compressed Sparse Row matrices
		int indptr_length;		// 
		int col_and_nz_length;		// 
		int** data;		//first row for data array (nz values), second for column index and third for ind ptr
} CSR_MATRIX;


// FUNCTIONs PROTOTYPEs

void IsPointerNull(void* ptr);									// check's if the pointer passed is a NULL pointer, in that

int** init_MATRIX (const int rows_a, const int cols_a);   		// dynamically allocate a matrix with rowsXcol dimensions

int** init_rand_matrix(const int rows, const int cols);

int* init_VEC (const int rows_vec);

int* init_rand_VEC(const int rows);

void print_VEC(int* ptr, const int rows);

int** read_matrix(FILE* file_ptr, const int rows_a, const int cols_a);	// read the matrix from a file, returns it's struct

void clean_matrix (int **ptr, const int rows);					// free memory of a dynamically allocated matrix as allocated by allocate_MATRIX

void print_matrix(int** ptr, const int rows, const int cols);		// print a matrix content

void print_CSR_matrix(CSR_MATRIX* ptr); //print csr struct  matrix

CSR_MATRIX* init_CSR_MATRIX (const int indptr_length, const int col_and_nz_length); //dynamically allocate a CSR struct, remember to count rows+1 elemnts in indptr_length!

CSR_MATRIX* MATRIX_to_CSR_converter(int** matrix_a, const int rows_a, const int cols_a); //convert a standard matrix into CSR format

CSR_MATRIX* init_rand_CSR_matrix(const int rows, const int cols);

int* CSR_matrix_and_VEC_mult(CSR_MATRIX* CSR_matrix, int* vec);


// MEASUREMENTS FUNCTIONS

uint64_t time_nano();

#endif
