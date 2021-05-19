#include "generate.h"

//extern Hash * PRIMITIVE_FUNC;

void * p_set_prec(Vector *v) {
    mpfr_set_default_prec((long)vector_ref(v,0));
    return NULL;
}
void* p_get_prec() {
    int i=mpfr_get_default_prec();
    return (void*)(long)i;
}
// io
void*p_print(Vector*v) {
    int i;
    for(i=0;i<v->_sp;i++) {
        fputs(objtostr((object*)vector_ref(v,i)),stdout); 
        fputs(" ",stdout);
    }
    fputs("\n",stdout);
    return NULL;
}
//void*p_printf(Vector*v) {
//    printf(((Symbol*)vector_ref(v,0))->_table,(object*)vector_ref(v,1)); 
//    printf("\n");
//    return NULL;
//}
void *p_open(Vector*v) {
    FILE *f;
    Symbol*path=vector_ref(v,0);
    Symbol* mode=vector_ref(v,1);
    //object*o=(object*)malloc(sizeof(object));
    //o->type=OBJ_IO;
    //o->data.ptr=(void*)fopen(path->_table,mode->_table);
    f=(FILE*)fopen(path->_table,mode->_table);
    //if (o->data.ptr == NULL) printf("I/O Error in openning:%s",path->_table);
    if (f == NULL) printf("I/O Error in openning:%s\n",path->_table);
    return (void*)f;
}

void*p_close(Vector*v) {
    fclose((FILE*)vector_ref(v,0));
    return NULL;
}

void *p_gets(Vector*v) {
    char buff[4096];
    //if (fgets(buff,4095,(FILE*)vector_ref(v,0))==NULL) return NULL;
    fgets(buff,4095,(FILE*)vector_ref(v,0));
    long n=strlen(buff);
    Symbol*s=new_symbol(buff,n+1);
    //object*o=(object*)malloc(sizeof(object));
    //o->type=OBJ_SYM;
    //o->data.ptr=s;
    //return (void*)o;
    return (void*)s;
}
/*
void* p_puts(Vector *v) {
    if (fputs(((Symbol*)vector_ref(v,0))->_table,(FILE*)vector_ref(v,1))==EOF) {;}
    return NULL;
}
*/
void * p_getc(Vector*v) {
    
    char c=fgetc(vector_ref(v,0));
    Symbol *s = new_symbol(&c,1);
    return (void*)s;    
}
// 数学関数
void *p_sin(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = sin  (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_cos(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = cos  (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_tan(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = tan  (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_asin(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = asin (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_acos(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = acos (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_atan(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = atan (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_sinh(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = sinh (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_cosh(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = cosh (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_tanh(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = tanh (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_asinh(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = asinh(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_acosh(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = acosh(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_atanh(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = atanh(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_log10(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = log10(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_logE(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = log  (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_log1p(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = log1p(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_log(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = log  (*(double*)vector_ref(v,1))/log(*(double*)vector_ref(v,0)); return (void*)f;}



Funcpointer primitive_func[]  = {p_set_prec,p_get_prec,
                                 p_print, p_open, p_close, p_gets, p_getc, p_sin, p_cos, p_tan, 
                                 p_asin, p_acos, p_atan, p_sinh, p_cosh, p_tanh, p_asinh, p_acosh, p_atanh,
                                 p_log10, p_logE, p_log, NULL};
char*primitive_function_name[]={"set_prec","get_prec",
                                "print", "open", "close", "gets", "getc", "sin", "cos", "tan", 
                                 "asin", "acos", "atan", "sinh", "cosh","tanh", "asinh", "acosh", "atanh",
                                 "log10", "logE", "log", NULL};
int primitive_function_arglisti[][3] = {//{OBJ_GEN},                                      // print
                                    {OBJ_INT},                                      // set_prec
                                    {OBJ_NONE},                                     // get_prec
                                    {OBJ_GEN},                                      // print
                                    {OBJ_SYM,OBJ_SYM},                              // open
                                    {OBJ_IO},                                       // close
                                    {OBJ_IO},                                       // gets
                                    {OBJ_IO},                                       // getc
                                    {OBJ_FLT},                                      // sin
                                    {OBJ_FLT},                                      // cos 
                                    {OBJ_FLT},                                      // tan
                                    {OBJ_FLT},                                      // asin
                                    {OBJ_FLT},                                      // acos 
                                    {OBJ_FLT},                                      // atan
                                    {OBJ_FLT},                                      // sinh
                                    {OBJ_FLT},                                      // cosh
                                    {OBJ_FLT},                                      // tanh
                                    {OBJ_FLT},                                      // asinh
                                    {OBJ_FLT},                                      // acosh 
                                    {OBJ_FLT},                                      // atanh
                                    {OBJ_FLT},                                      // log10 
                                    {OBJ_FLT},                                      // logE
                                    {OBJ_FLT, OBJ_FLT},                                      // log
                                    };

int primitive_function_ct[][3]  ={//{OBJ_NONE,1, TRUE},                        // print
                                {OBJ_NONE,1, FALSE},
                                {OBJ_INT, 0, FALSE},
                                {OBJ_NONE,1, TRUE},
                                {OBJ_IO,  2, FALSE},                 // open
                                {OBJ_NONE,1, FALSE},                        // close
                                {OBJ_SYM, 1, FALSE},                         // gets
                                {OBJ_SYM, 1, FALSE},                         // getc
                                {OBJ_FLT, 1, FALSE},                        // sin
                                {OBJ_FLT, 1, FALSE},                        // cos
                                {OBJ_FLT, 1, FALSE},                        // tan
                                {OBJ_FLT, 1, FALSE},                        // asin
                                {OBJ_FLT, 1, FALSE},                        // acos
                                {OBJ_FLT, 1, FALSE},                        // atan
                                {OBJ_FLT, 1 ,FALSE},                        // sinh
                                {OBJ_FLT, 1, FALSE},                        // cosh
                                {OBJ_FLT, 1, FALSE},                        // tanh
                                {OBJ_FLT, 1, FALSE},                        // asinh
                                {OBJ_FLT, 1, FALSE},                        // acosh
                                {OBJ_FLT, 1, FALSE},                        // atanh
                                {OBJ_FLT, 1, FALSE},                        // log10 
                                {OBJ_FLT, 1, FALSE},                        // logE
                                {OBJ_FLT, 2, FALSE},                        // log
                                 };

void * make_primitive() {
    int i = 0,j;
    code_type *ct;
    Vector *v,*vv;
    char *s; 
    PRIMITIVE_FUNC = Hash_init(128);

    while (primitive_func[i] != NULL) {
        v=vector_init(3);
        for (j=0;j<primitive_function_ct[i][1];j++) {
            push(v,(void*)(long)primitive_function_arglisti[i][j]);
        }
        ct = new_ct(OBJ_PFUNC, primitive_function_ct[i][0],v,primitive_function_ct[i][2]);
        //vv=vector_init(2);
        //push(vv,(void*)primitive_func[i]);push(vv,(void*)ct);
        s=primitive_function_name[i];long n=strlen(s);
        Hash_put(PRIMITIVE_FUNC,new_symbol(s,n),(void*)ct);
        Hash_put(G,new_symbol(s,n),(void*)primitive_func[i]);
        i++;
    }
}
