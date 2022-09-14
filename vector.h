#include "general.h"

#ifndef VECTOR
#define VECTOR

typedef union {
    char * i8;
    short *i16;
    int * i32;
    long * i64;
    double *f64;
    void * ptr;
}memblock;

typedef struct Vector {
    int     _size;
    void ** _table;
    int     _cp;
    int     _sp;
} Vector;

Vector *  vector_init( int maxN);
#define enqueue push
Vector * vector_copy1(Vector * v);
Vector * vector_copy0(Vector * v) ;
Vector * vector_copy(Vector * v) ;
Vector *vector_copy_n(Vector *, int);
Vector *vector_copy_nm(Vector *, int, int);
Vector *vector_slice_nm(Vector *, int, int);
Vector * vector_append(Vector*v1,Vector*v2);
void vector_resize(Vector * s);
void vector_print(Vector * s) ;
void vector_delete(Vector * v, int index); 
void vector_delete_n(Vector * v, int index, int size); 
void vector_insert(Vector * v, int index, void * value);
void vector_insert_vector(Vector *v, int index, Vector *c);
void vector_upsize(Vector *s, int up); 
Vector * vector_rev_copy_nm(Vector *v, int n, int m); 
#ifndef INLINE
#define INLINE
inline int is_stac_empty(Vector * s) { return s ->_sp == 0; }
inline int is_queu_empty(Vector * s) { return s ->_sp == s ->_cp; }
inline void push(Vector * s, void * data) {
    if (s ->_sp >= s ->_size ) 
        vector_resize(s);  
    s ->_table[(s ->_sp) ++ ] = data;
}
//inline void * pop(Vector * s) {return s ->_table[  -- (s ->_sp)]; }
inline void * pop(Vector * s) {
    return (s->_sp>0) ? s ->_table[  -- (s ->_sp)] : NULL ;
}
inline void * dequeue(Vector * s) { return (s ->_cp > s ->_sp ) ? NULL : s ->_table[ (s ->_cp) ++  ]; }
//inline void * vector_ref(Vector * v, int index) {return v ->_table[index + v->_cp];}
inline void * vector_ref(Vector * v, int index) {return (index <v->_sp && index >=0) ? v->_table[index ] : NULL;}
//inline void vector_set(Vector * v, int index, void ** val) {v ->_table[index + v->_cp] = val; }
inline void vector_set(Vector * v, int index, void ** val) {if (index<v->_sp && index >=0) v ->_table[index] = val; }
//inline int vector_length(Vector *v) {return (v->_sp) - (v->_cp); }
inline int vector_length(Vector *v) {return (v->_sp); }

#endif
#endif