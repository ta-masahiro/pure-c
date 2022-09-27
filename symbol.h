#include "general.h"
#ifndef SYMBOL
#define SYMBOL
typedef struct {
    unsigned int _size; unsigned int _sp;
    unsigned char *_table;
} Symbol;

Symbol * new_symbol(unsigned char * str, unsigned int size) ;
int symbol_eq(Symbol*s1,Symbol*s2) ;            // if s1 == s2 then TRUE else FALSE 
Symbol * symbol_cat(Symbol *s1, Symbol *s2);    // s1 = s1 + s2
Symbol * symbol_cat_s(Symbol *, char *);        
Symbol * symbol_append(Symbol*s1,Symbol*s2) ;   // new symbol = s1 + s2
Symbol * symbol_ref(Symbol*s,unsigned int pos); // sのpos番目の1文字
Symbol * symbol_cpy(Symbol*s) ;
Symbol * symbol_cpy_n(Symbol*s, unsigned int i, unsigned int j) ;
Symbol * symbol_set(Symbol*t,unsigned int pos,Symbol*s);
Symbol * symbol_pop(Symbol*s);
unsigned char symbol_pop_c(Symbol *s);
void symbol_push(Symbol *s, Symbol *c);
void symbol_push_c(Symbol *s, unsigned char c); 
int symbol_search(Symbol *s, Symbol *c);
void symbol_print(Symbol *S);
void symbol_delete(Symbol *s, int n, int m);
void symbol_insert(Symbol *s, int i, Symbol *t);
#endif