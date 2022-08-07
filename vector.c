#include "vector.h"
//#include "fast_memcpy.h"
//#define memcpy fast_memcpy
#define INIT_SIZE 32    //vectorの初期サイズ
Vector *  vector_init( int init) {
    Vector * s = (Vector * )malloc(sizeof(Vector)); 
    //int size=MAX(INIT_SIZE,init);//printf("vector_init:size=%d\n",size);
    int size=INIT_SIZE>init ? INIT_SIZE : init;//printf("vector_init:size=%d\n",size);
    s ->_size = size; 
    s ->_table = (void ** )malloc(size * sizeof(void * ));
    s ->_cp = 0;
    s ->_sp = 0;  
    return s; 
}

extern int is_stac_empty(Vector * s);
extern int is_queu_empty(Vector * s);
extern void push(Vector * s, void * data);
extern void * pop(Vector * s);
extern void * dequeue(Vector * s);
extern void * vector_ref(Vector * v, int index);
extern void vector_set(Vector * v, int index, void ** val);
extern int vector_length(Vector *v) ;

Vector * vector_copy1(Vector * v) {
    Vector * s = (Vector * )malloc(sizeof(Vector)); 
    s ->_size = v ->_size; 
    s ->_table = v ->_table;
    s ->_cp = v ->_cp;
    s ->_sp = v ->_sp;  
    return s; 
}
 
Vector * vector_copy0(Vector * v) {
    Vector * r = vector_init(v ->_size );
    //memcpy(r ->_table, v ->_table, (v->_size)*sizeof(void*));
    memcpy(r ->_table, v ->_table, (v->_sp)*sizeof(void*));
    r->_sp = v->_sp;r ->_cp = v ->_cp; 
    return r;
}

Vector * vector_copy(Vector * v) {
    Vector * r = vector_init(v ->_sp-v->_cp );//vector_print(v);
    //memcpy(r ->_table, v ->_table, (v->_size)*sizeof(void*));
    memcpy(r->_table, v->_table+(v->_cp), (v->_sp-v->_cp)*sizeof(void*));
    r->_sp = v->_sp-v->_cp;r ->_cp = 0;//vector_print(r); 
    return r;
}

Vector *vector_copy_n(Vector *v, int n) {
    // 先頭からn個copyする
    Vector * r = vector_init(n );
    memcpy(r ->_table, v ->_table, n*sizeof(void*));
    r->_sp = n; r ->_cp = v ->_cp; 
    return r;
}

void vector_resize(Vector * s) {
    int oldN = s ->_size; 
    int maxN = 3 * oldN / 2 + 1;     /* 1.5倍に拡大  */   
    void ** table =(void ** )realloc(s ->_table, maxN * sizeof(void * )) ;
    if (table == NULL)  {
        printf("reallock failed!\n"); 
        free(s -> _table); exit(0); }
    s ->_table = table;  
    s ->_size = maxN;
    //printf("vector resize:%d to %d\n", oldN, maxN); // vector_print(s);    
}

void vector_upsize(Vector *s, int up) {
    // 現在のvectorをupだげ増やせるようにする
    // s->_table、s->_sizeが変更されるかもしれない
    // s->_spは変更しない
    int rq_size, old_size;
    void **table;
    if ((rq_size = s->_sp + up) > (old_size = s->_size)) { //
        table = (void **)realloc(s->_table, rq_size*(sizeof(void*)));
        if (table == NULL)  {
            printf("reallock failed!\n"); 
            free(s -> _table); exit(0); }
        s ->_table = table;  
        s ->_size = rq_size;
        printf("vector upsize:%d to %d\n", old_size, rq_size);
    }
}

Vector * vector_append(Vector*v1,Vector*v2) {
    int new_size; 
    void**vd=(void**)malloc((new_size = MAX(v1->_sp+v2->_sp, INIT_SIZE))*sizeof(void*));
    memcpy(vd,v1->_table,(v1->_sp)*sizeof(void*));memcpy(vd+(v1->_sp),v2->_table,(v2->_sp)*sizeof(void*));
    Vector *v=(Vector*)malloc(sizeof(Vector));
    v->_cp=0;v->_sp=(v1->_sp+v2->_sp);v->_table=vd;v->_size=new_size;
    return v;
}

void vector_delete(Vector * v, int index) {
    memmove(v->_table + index, v->_table + index +1, sizeof(void*)*(v->_sp - index));
    v->_sp--;
}

void vector_insert(Vector * v, int index, void * value) {
    if (v->_size <= v->_sp) vector_resize(v);
    memmove(v->_table + index + 1, v->_table + index, sizeof(void*)*(v->_sp - index));
    v->_table[index]=value;
    v->_sp++;
}

void vector_print(Vector * s) {
    int i;
    if (s==NULL) {printf("None\n");return;}
    printf("[ ");  
    for(i = s->_cp; i< s ->_sp; i ++ ) {
        printf("%ld ", (long)(s ->_table[i])); 
    }
    printf("]\n"); 
}

Vector * vector_rev_copy_nm(Vector *v, int n, int m) {
    // n番目からm番目までm-n+1個を逆順でcopyする
    Vector *r = vector_init(n);
    for (int i = n; i <= m; i++) {
        r->_table[i] = v->_table[m - i];
    }
    r->_sp = m - n + 1;
    return r;
}

 /* 以下は適当なテスト   
int main(int argc, char * argv[]) {
    long i; 
    Vector * v = vector_init(2); 
    for (i = 1000; i<1020; i ++ ) {
        push(v, (void * )i); 
    }
    vector_print(v); 
   for (i = 1; i<= 5; i ++ ){
        printf("pop:%ld\n", (long )pop(v)); 
   } 
    vector_print(v); 
    printf("Queue Test\n");  
    for (i = 0; i<10; i ++ ) {
        push(v, (void * )i); 
    }
    vector_print(v); 

    for(i = 0; i<12; i ++ ){
        printf("dequeue:%ld\n", (long)dequeue(v)); 
    }
    vector_print(v);
    printf("Index Test\n"); 
    for(i = 0; i<(v ->_sp); i ++ ) {
        printf("v[%ld] = %ld\n", i, (long)vector_ref(v, i)); 
    }
    for(i = 0; i<(v ->_sp); i ++ ) {
        vector_set(v, i, (void * )(i * i)); 
    }
    vector_print(v); 
}
*/
