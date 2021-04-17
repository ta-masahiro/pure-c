#include "vm.h"
void*__print(Vector*v) {
    int i;
    for(i=0;i<v->_sp;i++) {
        fputs(objtostr((object*)vector_ref(v,i)),stdout); 
        fputs(" ",stdout);
    }
    fputs("\n",stdout);
    return NULL;
}
void *__open(Vector*v) {
    Symbol*path=vector_ref(v,0);
    Symbol* mode=vector_ref(v,1);
    object*o=(object*)malloc(sizeof(object));
    o->type=OBJ_IO;
    o->data.ptr=(void*)fopen(path->_table,mode->_table);
    return (void*)o;
}

void*__close(Vector*v) {
    fclose((FILE*)vector_ref(v,0));
    return NULL;
}

void *__gets(Vector*v) {
    char*buff[4096];
    fgets(buff,4095,(FILE*)vector_ref(v,0));
    long n=strlen(buff);
    Symbol*s=new_symbol(buff,n+1);
    object*o=(object*)malloc(sizeof(object));
    o->type=OBJ_SYM;
    o->data.ptr=s;
    return (void*)o;
}

void * __getc(Vector*v) {
    char*c=fgetc(vector_ref(v,0));
    long n=1;    
}
Funcpointer primitive_func[] = {__print,__open,NULL};
char*primitive_function_name[]={"print","open",NULL};
