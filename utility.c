#include "vm.h"
#include "object.h"
void*__print(Vector *v) {
    int i;
    for(i=v->_cp;i<v->_sp;i++) {
        printf("%s ",objtostr((object*)v));
    }
    printf("%\n");
    return NULL;
}
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