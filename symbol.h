#include "general.h"
#ifndef SYMBOL
#define SYMBOL
typedef struct {
    unsigned long _size;
    unsigned char *_table;
} Symbol;

Symbol * new_symbol(unsigned char * str, unsigned long size) ;
int symbol_eq(Symbol*s1,Symbol*s2) ;
Symbol * symbol_append(Symbol*s1,Symbol*s2) ;
Symbol * symbol_ref(Symbol*s,unsigned long pos);
Symbol*symbol_cpy(Symbol*s) ;
Symbol*symbol_set(Symbol*t,unsigned long pos,Symbol*s);
#endif