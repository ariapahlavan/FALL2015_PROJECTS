/* Project2.cpp
 *Aria Pahlavan
 * EE312, Spring 2015
 * Project 2, Matrix Multiplication
 */

#include <stdio.h>
#include <stdint.h>
#include "MatrixMultiply.h"

/*
 * An example routine demonstrating the use of row-major ordering
 *
 * You do not need to use this function in your solution
 * (I didn't use it in mine!)
 *
 * given two matrices a and b, where the number of rows in b is equal
 * to the number of columns in a,
 * and given a row number 'row' and a column number 'col',
 * calculate the dot product of the row'th row from a and the col'th column from b
 *
 * recall the dot product is the sum of the element-by-element product of
 * the two vectors.
 */
double dotProduct(double a[], const uint32_t a_rows, const uint32_t a_cols,
    /* a is a matrix with a_rows rows and a_cols columns */
    double b[], const uint32_t b_cols,
    /* b is also a matrix.  It has a_cols rows and b_cols columns */
    uint32_t row, // an index of a row of a
    uint32_t col) { // an index of a column of b

        double sum = 0.0; // the result of the dot product is stored here
        uint32_t k = 0;
        while (k < a_cols) { // recall: a_cols == b_rows
            /* we need to multiply a[row, k] and b[k, col] and add that to sum */
            sum = sum + a[(row * a_cols) + k] * b[k * b_cols + col];
            /* recall a[i,j] is stored at a[(i * a_cols) + j] and b[i, j] is at b[(i * b_cols) + j] */
            k += 1;
        }

        return sum;
}

void multiplyMatrices(
        double a[],
        const uint32_t a_rows,
        const uint32_t a_cols,
        double b[],
        const uint32_t b_cols,
        double c[]) {
	uint32_t counter = 0;
	for (uint32_t i = 0; i < a_rows; i++) {
		for (uint32_t j = 0; j < b_cols; j++) {
			c[counter] = dotProduct(a, a_rows, a_cols, b, b_cols, i, j);		//c is getting update using coutner and the result of the dot product
			counter++;		//increment counter
		}
	}

}

#define READY_FOR_STAGE2
#ifdef READY_FOR_STAGE2

/* these three variables are used in Stage 2 to implement allocateSpace */
#define TOTAL_SPACE 10000
double memory_pool[TOTAL_SPACE];
uint32_t top_of_pool = 0;

/* you need to write this function. clearly it doesn't currently do anything */
PtrDouble allocateSpace(uint32_t size) {
	uint32_t fake_top = top_of_pool;
	top_of_pool += size; //updating the top_of_pool var
	return &memory_pool[fake_top]; //returning a ptr to the base of allocated space
}

void multiplyMatrixChain(
	PtrDouble matrix_list[],
	uint32_t rows[],
	uint32_t cols[],
	uint32_t num_matrices,
	double out[],
	uint32_t out_rows,
	uint32_t out_cols) {
	/* The loop works by continually multiplying the a_mat
	* and b_mat matrix to obtain the c_mat matrix.
	* After each iteration, the previous "c_mat" becomes the
	* next "a_mat" matrix.  the b_mat matrix will be the
	* next matrix in the matrix_list array.
	* For example, on the first time through a_mat is matrix_list[0]
	* b_mat is matrix_list[1]
	* c_mat (the output) will be a temporary matrix that you must allocate
	* space for.
	* After multiplying a_mat and b_mat together, you need to
	* take the result and multiply it by matrix_list[2],
	* so a_mat will be set to c_mat, and b_mat will be matrix_list[2]
	* you must again allocate a new temporary array to hold the result.
	*
	* There's no guarantee that the temporary arrays are the same size,
	* so there's no practical way to reuse the temporary arrays.
	* Therefore, allocate a new array each iteration.  We'll clean up when we're done.
	*/
	uint32_t size = 0;
	double* a_mat = matrix_list[0];
	uint32_t a_rows = rows[0]; // number of rows in first matrix
	uint32_t a_cols = cols[0]; // number of cols in first matrix
	uint32_t next_src = 1; // index of next matrix to multiply by
	double* b_mat = matrix_list[1];
	double* c_mat = 0;
	uint32_t b_cols = cols[next_src]; //number of cols in second matrix
	while (next_src < num_matrices) {
		/* Each iteration of the loop perform the following
		 * set c_mat = a_mat * b_mat where
		 * a_mat is the result from the previous iteration of the loop (or matrix_list[0] on the first iteration)
		 * b_mat is the next matrix in the matrix list
		 * c_mat is allocated space (using our handy-dandy memory allocator function) */
		b_mat = matrix_list[next_src]; //getting the b matrix
		b_cols = cols[next_src]; //finding the number of cols for the b matrix
		c_mat = allocateSpace(a_rows * b_cols); //allocate space for the new matrix output
		size = a_rows * b_cols; // save the size for the end
		multiplyMatrices(a_mat, a_rows, a_cols, b_mat, b_cols, c_mat); //multiplying matirces
		a_mat = c_mat; //setting a_mat to the result of the product
		a_cols = b_cols; // updating the col of the result to a_col
		next_src += 1; //incrementing the counter for the next matrix
	}
	/* when the loop completes, c_mat points to the final matrix result */

	/* don't forget to ensure that the final result matrix is put
	* into the 'out' array!!! */
	for (uint32_t i = 0; i < size; i++){
		out[i] = c_mat[i];
	}
	/* deallocate all memory (this only takes one line, 'cause we have such a simple memory allocator) */
	top_of_pool = 0;
}

#endif /* READY_FOR_STAGE_2 */