#include "general.h"
typedef struct {
    unsigned long size; 
    void ** table; 
} array; 

typedef struct {
    unsigned long size; 
    unsigned char * table; 
} i8_array; 

typedef struct {
    unsigned int xsize,ysize;
    void ** table;
} array_2;

typedef struct {
    unsigned int dim;
    unsigned int * sizes;
    void ** table;
} array_n;

array * array_init(unsigned long size) {
    array * a = (array*)malloc(sizeof(array));
    void * table = (void*)malloc(size*sizeof(void));
    return a;
}

i8_array * i8_array_init(unsigned long size) {
    i8_array * a = (i8_array*)malloc(sizeof(array));
    unsigned char * table = (unsigned char *)malloc(size*sizeof(char));
    return a;
}

array_2 * array_2_init(unsigned int xsize, unsigned int ysize) {
    array_2 * a = (array_2*)malloc(sizeof(array_2));
    void * table = (void *)malloc(xsize*ysize*sizeof(void*));
    return a;
}
array_n * array_n__init(unsigned int dim, unsigned int * sizes) {
    unsigned int size  = 1; 
    for(int i = 0; i < dim; i ++ ) {
        size *= sizes[i]; 
    }
    void * table = malloc(size * sizeof(void * ));
    array_n * a = (array_n * )malloc(sizeof(array_n)); 
    a -> dim = dim; a -> sizes = sizes; a -> table = table;
    return a;    
}

void * array_ref(array * a, unsigned long idx) {
    return a->table[idx];
}

void * array_ref(array * a, int  * ind) {
    int index = ind[0]; 
    for(int i =1 ; i < a -> dim; i ++ ) {
        index = (index * a -> sizes[i - 1]) + ind[i]; 
    }
    return a -> table[index]; 
}
