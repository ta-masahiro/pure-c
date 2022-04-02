#include "vector.h"
#include <cblas.h>
#include <lapacke.h>

#ifndef ARRAY
#define ARRAY
/*
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
*/
typedef struct {
    unsigned int type;
    unsigned int dim;
    unsigned int * sizes;
    unsigned int * sps;
    void ** table;
} array;

array * array_init(unsigned int type, unsigned int dim, unsigned int * sizes);
void * array_ref(array * a, int * ind);
array * array_eye(int dim) ;
array * array_array_mul(array  * A, array * B);
array * array_scler_mull(array *A, void * value);
array * array_array_add(array *A, array * B);
array * solv_liner(array * A, array * Y) ;

#endif