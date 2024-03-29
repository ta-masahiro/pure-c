#include <ctype.h>
#include "symbol.h"
//#include "fast_memcpy.h"
//#define memcpy fast_memcpy

Symbol * new_symbol(unsigned char * str, unsigned int size) {
    unsigned char * s = (unsigned char * )malloc((size_t)(size + 1) * sizeof(char)); 
    // strcpy(s, str); // printf("%s\n", s);
    memcpy(s,str,(size_t)size);
    // if (strcmp(s, "LDC") == 0) printf("!LDC!\n");  
    Symbol * sym = (Symbol * )malloc(sizeof(Symbol));
    sym -> _size = size;
    sym -> _sp = size; 
    sym -> _table = s;  
    return sym;  
}

int symbol_eq(Symbol*s1,Symbol*s2) {
    //return (s1->_size) != (s2->_size) ? FALSE : strcmp(s1->_table,s2->_table)==0;
    return (s1->_sp) != (s2->_sp) ? FALSE : memcmp(s1->_table,s2->_table, s1->_sp * sizeof(char)) == 0;
}
Symbol * symbol_cat(Symbol *s1,Symbol *s2) {
    if (s1->_size - s1->_sp >= s2 ->_sp) {
        memcpy(s1->_table + s1->_sp, s2->_table, (s2 ->_sp)*sizeof(char));
        s1->_sp += s2->_sp;
        return s1;
    }
    unsigned char * sd=(char*)malloc((s1->_size+s2->_size)*sizeof(char));
    memcpy(sd, s1->_table, (s1->_sp)*sizeof(char)); memcpy(sd + (s1->_sp), s2->_table, (s2->_sp)*sizeof(char));
    s1->_size += s2->_size;s1->_sp += s2 ->_sp; s1->_table = sd;
    return s1;
}

Symbol * symbol_cat_s(Symbol *s1, char * s2) {
    int s2_size = strlen(s2);
    if (s1->_size - s1->_sp >= s2_size) {
        memcpy(s1->_table + s1->_sp, s2, s2_size*sizeof(char));
        s1->_sp += s2_size;
        return s1;
    }
    unsigned char * sd=(char*)malloc((s1->_size + s2_size)*sizeof(char));
    memcpy(sd, s1->_table, (s1->_sp)*sizeof(char)); memcpy(sd + (s1->_sp), s2, s2_size*sizeof(char));
    s1->_size += s2_size;s1->_sp += s2_size; s1->_table = sd;
    return s1;
}

Symbol * symbol_append(Symbol *s1,Symbol *s2) {
    /*
    if (s1->_size - s1->_sp >= s2 ->_sp) {
        memcpy(s1->_table + s1->_sp, s2->_table, (s2 ->_sp)*sizeof(char));
        s1->_sp += s2->_sp;
        return s1;
    }
    */
    unsigned char * sd=(char*)malloc((s1->_size+s2->_size)*sizeof(char));
    memcpy(sd, s1->_table, (s1->_sp)*sizeof(char)); memcpy(sd + (s1->_sp), s2->_table, (s2->_sp)*sizeof(char));
    Symbol * s = (Symbol*)malloc(sizeof(Symbol));
    s->_size = (s1->_size+s2->_size);s->_sp = s1->_sp + s2 ->_sp; s->_table = sd;
    return s;
}

Symbol*symbol_cpy(Symbol*s) {
    unsigned char *st=(char*)malloc(s->_sp*sizeof(char));
    memcpy(st,s->_table,s->_sp);
    Symbol * sym = (Symbol *)malloc(sizeof(Symbol));
    sym->_size = s->_sp; sym->_sp = s->_sp; sym->_table = st;
    return sym;
}

Symbol*symbol_cpy_n(Symbol*s,unsigned int start, unsigned int size) {
    unsigned char * st = (char *)malloc((size + 1)*sizeof(char));
    memcpy(st, s->_table + start * sizeof(char), size * sizeof(char));
    *(st + size) = '\0';
    Symbol * sym=(Symbol*)malloc(sizeof(Symbol));
    sym->_size = size; sym->_sp = size; sym->_table = st;
    return sym;
}

Symbol* symbol_ref(Symbol * s, unsigned int pos) {
    Symbol * r = (Symbol *)malloc(sizeof(Symbol));
    unsigned char * table = (char *)malloc(2 * sizeof(char));
    table[0]=s->_table[pos]; table[1] = '\0';
    r->_size = 1; r->_table = table;
    return r;
}

Symbol * symbol_set(Symbol * t, unsigned int pos, Symbol * s) {
    if (pos>=t->_sp) return t; 
    if (pos + s->_sp <= t->_size) {memcpy(t->_table + pos, s->_table, s->_sp); return t;}
    unsigned char *sd = (unsigned char *)realloc(t->_table, (pos + s->_sp)*sizeof(char));
    memcpy(sd + pos, s->_table, s->_sp);
    t->_size = t->_sp = pos + s->_sp ;t->_table = sd;
    return t;
}

Symbol * symbol_pop(Symbol *s) {
    Symbol*r=(Symbol*)malloc(sizeof(Symbol));
    unsigned char* table=(char*)malloc(2 * sizeof(char));
    table[0]=s->_table[s->_sp-1]; table[1] = '\0';
    s->_table[s->_sp-1] = '\0'; s->_sp--;
    r->_size = 1; r->_sp =1; r->_table = table;
    return r;
}

unsigned char symbol_pop_c(Symbol *s) {
    return s->_table[--(s->_sp)];
}

int symbol_search(Symbol * s, Symbol * c) {
    unsigned char * ad = strstr(s->_table, c->_table);
    if (ad == NULL) return -1;
    return ad - s->_table;
}
void symbol_resize(Symbol * s) {
    int oldN = s ->_size; 
    int maxN = 3 * oldN / 2 + 1;     /* 1.5倍に拡大  */   
    char * table =(char* )realloc(s ->_table, (maxN+1) * sizeof(char)) ;
    if (table == NULL)  {
        printf("reallock failed!\n"); 
        free(s -> _table); exit(0); }
    s ->_table = table;  
    s ->_size = maxN;
    //printf("vector resize:%d to %d\n", oldN, maxN); // vector_print(s);    
}

void symbol_push(Symbol * s, Symbol * t) {
    if (s ->_sp >= s ->_size ) 
        symbol_resize(s);  
    s ->_table[(s ->_sp)++ ] = t->_table[0];
    //s ->_table[s ->_sp ] = t->_table[1];
    s ->_table[s ->_sp ] = '\0';

    //return s;
}

void symbol_push_c(Symbol * s, unsigned char t) {
    if (s ->_sp >= s ->_size  ) 
        symbol_resize(s);  
    s ->_table[(s ->_sp)++ ] = t;
    s ->_table[s ->_sp ] = '\0';

    //return s;
}

void symbol_print(Symbol *s) {puts(s->_table);}

void symbol_delete(Symbol *s, int n, int m) {
    // symbolのn文字目からm個除去する
    // nが負の時は最終文字+ni-1文字目から逆向きにm個除去する
    if (n < 0) n = s->_sp + n - m + 1;
    // s->table[n+m+1]~s->table[sp]までをs->table[n]にsp-n-m-1個移動
    memmove(s->_table + n, s->_table + n + m, (s->_sp - n - m )*sizeof(char));
    s->_sp -=m;
}

void symbol_insert(Symbol *s, int i, Symbol *t) {
    // symbol のi文字目に別symbolを挿入する
    int m = t->_sp;
    char * n_table;
    if (s->_sp+m+1 > s->_size) {
        n_table = (char*)malloc(((s->_size) + m + 1)*sizeof(char));
        memmove(n_table, s->_table, i*sizeof(char));
        memmove(n_table + i, t->_table, m*sizeof(char));
        memmove(n_table + i + m, s->_table + i , (s->_sp - i + 1)*sizeof(char));
        s->_table = n_table; s->_sp += m; s->_size += m;
    } else {
        memmove(s->_table+i+m, s->_table + i,(s->_sp - i + 1)*sizeof(char));
        memmove(s->_table + i, t->_table, m*sizeof(char));
        s->_sp+=m;
    }
}