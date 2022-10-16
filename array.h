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
typedef enum { OBJ_NONE=0,
    // 1     2          3       4        5         6          7
    OBJ_INT, OBJ_LINT, OBJ_RAT, OBJ_FLT, OBJ_LFLT, OBJ_CMPLX, OBJ_GEN,
    //      8          9      10        11
    OBJ_SYSFUNC, OBJ_PFUNC, OBJ_UFUNC, OBJ_CNT,
    //    12        13        14      15    16
    OBJ_VECT, OBJ_DICT, OBJ_PAIR,OBJ_SYM, OBJ_ARRAY,
    // 17   18       19           20        21
    OBJ_IO, OBJ_KEY, OBJ_CLS_DEF, OBJ_AST, OBJ_UFUNC_S
} obj_type;
/*
typedef struct {
    obj_type type;
    unsigned int dim;
    unsigned int * sizes;
    unsigned int * sps;
    void ** table;
} array;
*/
typedef struct {
    obj_type type;
    unsigned int dim;
    unsigned int * sizes;
    unsigned int * sps;
    union {
        long    * _int;
        double  * _flt;
        complex * _cmplx;
        void   ** _ptr;
    } table;
} array;
array * array_init(obj_type type, unsigned int dim, unsigned int * sizes);
void * array_ref(array * a, int * ind);
void change_array_type(array *, obj_type);
array * array_eye(int dim) ;
array * array_array_mul(array  * A, array * B);
array * array_scler_mull(array *A, void * value);
array * array_array_add(array *A, array * B);
array * solv_liner(array * A, array * Y) ;
array * array_inv(array *);
array * array_copy(array *);
Vector * eigen(array*);
#endif