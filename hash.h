#include "vector.h"
#include "symbol.h"
/*
typedef struct Symbol {
    unsigned long _size;
    unsigned char *_table;
} Symbol;
*/
typedef struct Data {
    Symbol  *key;
    void    *val;
} Data;

typedef struct Hash {
    Data *hashTable;
    unsigned long initval, size, entries;        // 初期値、サイズ、登録数
} Hash;

Hash * Hash_init(unsigned long size);
void Hash_resize(Hash * h, unsigned long newSize);     // 内部関数
void Hash_free(Hash *pHash);
unsigned long hash(unsigned char *key, unsigned long keysize, unsigned long initval);                 // 内部関数SH
unsigned long Hash_put(Hash * h, Symbol *key, void *val);
void  ** Hash_get(Hash * h, Symbol *key);
void  * eval(Vector * S, Vector * E, Vector * C, Vector * R, Vector * EE, Hash * G); 
void print_hashTable(Hash* h); 
