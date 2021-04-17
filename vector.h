#include "general.h"

#ifndef VECTOR
#define VECTOR

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
Vector * vector_append(Vector*v1,Vector*v2);
void vector_resize(Vector * s);
void vector_print(Vector * s) ;
 
#ifndef INLINE
#define INLINE
inline int is_stac_empty(Vector * s) { return s ->_sp == 0; }
inline int is_queu_empty(Vector * s) { return s ->_sp == s ->_cp; }
inline void push(Vector * s, void * data) {
    if (s ->_sp >= s ->_size ) vector_resize(s);  
    s ->_table[(s ->_sp) ++ ] = data;
}
inline void * pop(Vector * s) {return s ->_table[  -- (s ->_sp)]; }
inline void * dequeue(Vector * s) { return (s ->_cp > s ->_sp ) ? NULL : s ->_table[ (s ->_cp) ++  ]; }
//inline void * vector_ref(Vector * v, int index) {return v ->_table[index + v->_cp];}
inline void * vector_ref(Vector * v, int index) {return v ->_table[index ];}
//inline void vector_set(Vector * v, int index, void ** val) {v ->_table[index + v->_cp] = val; }
inline void vector_set(Vector * v, int index, void ** val) {v ->_table[index] = val; }
//inline int vector_length(Vector *v) {return (v->_sp) - (v->_cp); }
inline int vector_length(Vector *v) {return (v->_sp); }

#endif
#endif