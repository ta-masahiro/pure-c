#include "vm.h"
#include "object.h"
#define MAX_BUF 1024

void*__print(Vector *v) {
    int i;
    for(i=v->_cp;i<v->_sp;i++) {
        printf("%s ",objtostr((object*)v));
    }
    printf("%\n");
    return NULL;
}

void* __gets(Vector *v) {
    char* buff=(char*)malloc(MAX_BUF*sizeof(char));
    if (fgets(buff,MAX_BUF,(FILE*)pop(v))) return NULL;
    return new_symbol(buff,strlen(buff));
}

void* __getc(Vector *v) {
    return (void*)(long)getc((FILE*)pop(v));
}

/*
void*__fprint(Vector *v) {
    Symbol*format;
    object*o;

    long n=v->_sp-v->_cp;
    if (n<=0) return NULL;
    o=(object*)Vector_ref(v,v->_cp);
    if (o->type != OBJ_SYM) {
        v->_cp++;__print(v);return NULL;
    }
    format=((Symbol*)o->data.ptr);
*/