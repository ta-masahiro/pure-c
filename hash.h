#include "vector.h"
#include "symbol.h"
/*
typedef struct Symbol {
    unsigned long _size;
    unsigned char *_table;
} Symbol;
*/
#ifndef HASH_DEF
#define HASH_DEF
typedef struct Data {
    Symbol  *key;
    void    *val;
} Data;

typedef struct Hash {
    Data *hashTable;
    unsigned int initval, size, entries;        // 初期値、サイズ、登録数
} Hash;

Hash * Hash_init(unsigned int size);
void Hash_resize(Hash * h, unsigned int newSize);     // 内部関数
void Hash_free(Hash *pHash);
unsigned int hash(unsigned char *key, unsigned int keysize, unsigned int initval);                 // 内部関数SH
unsigned int Hash_put(Hash * h, Symbol *key, void *val);
void  ** Hash_get(Hash * h, Symbol *key);
void  * eval(Vector * S, Vector * E, Vector * C, Vector * R, Vector * EE, Hash * G); 
void print_hashTable(Hash* h); 
void Hash_del(Hash * hashT, Symbol *key);
#endif
