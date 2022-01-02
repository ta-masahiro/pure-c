#include "general.h"
#include <cblas.h>

typedef struct {
    unsigned long size; 
    unsigned long sp; 
    void ** table; 
} array; 

typedef struct {
    unsigned long size; 
    unsigned long sp; 
    unsigned char * table; 
} i8_array; 

typedef struct {
    unsigned int xsize,ysize;
    unsigned int xsp,ysp;
    void ** table;
} array_2;

typedef struct {
    unsigned int dim;
    unsigned int * sizes;
    unsigned int * sps;
    void ** table;
} array_n;

array * array_init(unsigned long size) {
    array * a = (array*)malloc(sizeof(array));
    void ** table = (void**)malloc(size*sizeof(void));
    a->size = size; a->sp = 0; a->table = table;
    return a;
}

i8_array * i8_array_init(unsigned long size) {
    i8_array * a = (i8_array*)malloc(sizeof(array));
    unsigned char * table = (unsigned char *)malloc(size*sizeof(char));
    a->size = size; a->sp = 0; a->table = table;
    return a;
}

array_2 * array_2_init(unsigned int xsize, unsigned int ysize) {
    array_2 * a = (array_2*)malloc(sizeof(array_2));
    void ** table = (void **)malloc(xsize*ysize*sizeof(void*));
    a->xsize = xsize; a->ysize = ysize;
    a->xsp = 0; a->ysp =0;
    a->table = table;
    return a;
}
array_n * array_n__init(unsigned int dim, unsigned int * sizes) {
    unsigned int size  = 1; 
    for(int i = 0; i < dim; i ++ ) {
        size *= sizes[i]; 
    }
    void ** table = (void**)malloc(size * sizeof(void * ));
    array_n * a = (array_n * )malloc(sizeof(array_n)); 
    a -> dim = dim; a -> sizes = sizes; a -> table = table;
    return a;    
}

void * array_ref(array * a, unsigned long idx) {
    return a->table[idx];
}

void * array_2_ref(array_2 *a, unsigned int xid, unsigned int yid) {
    return a->table[xid*a->xsize + yid];
}

void * array_n_ref(array_n * a, int * ind) {
    unsigned int index = ind[0]; 
    for(int i =1 ; i < a -> dim; i ++ ) {
        index = (index * a -> sizes[i - 1]) + ind[i]; 
    }
    return a -> table[index]; 
}

array_2 * matrix_mul(array_2 * A, array_2 * B) {
    if (A->ysp != B->xsp) return NULL;
    array_2 * C = array_2_init(A->xsize, B->ysize);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        A->xsize, B->xsize, C->xsize, 1.0, (double *)A, A->xsp, (double*)B, B->xsp, 0.0, (double*)C, C->xsp);
    // dgemm_(order, TransA, TransB, M, N, K, alpha, A, LDA, B, LDB, beta, C, LDC)
	// C = alpha * A * B + beta * C
	// A=M*K, B=K*N, C=M*N
    // order: CblasRowMajor/CblasColMajor
	// Trans: CblasNoTrans/CblasTrans/CblasConjTrans
	// LDA = number of row of A
    return C;
}
