#include "generate.h"

//extern Hash * PRIMITIVE_FUNC;
void * p_exit() {exit(0);}
void * p_forget(Vector *v) {

}
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
void *p_exp(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = exp  (*(double*)vector_ref(v,0)); }
void *p_iabs(Vector *v)     {return (void*)labs((long)vector_ref(v,0));}
void *p_fabs(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = fabs(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_isqrt(Vector *v)    {return (void*)(long)sqrt((double)(long)vector_ref(v,0));}
void *p_fsqrt(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = sqrt(*(double*)vector_ref(v,0)); return (void*)f;}
//
void *p_labs(Vector *v)     {mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set(L,(mpz_ptr)vector_ref(v,0));mpz_abs(L,L);return (void*)L;}
void *p_rabs(Vector *v)     {mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));mpq_set(Q,(mpq_ptr)vector_ref(v,0));mpq_abs(Q,Q);return (void*)Q;}
void *p_lfabs(Vector *v)    {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_abs(F,F,MPFR_RNDA);return (void*)F;}
void *p_cabs(Vector *v)     {complex *c = (complex*)malloc(sizeof(complex));*c = cabs(*(complex*)vector_ref(v,0)); return (void*)c;}
void *p_lsqrt(Vector *v)     {mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set(L,(mpz_ptr)vector_ref(v,0));mpz_sqrt(L,L);return (void*)L;}
//void *p_rsqrt(Vector *v)     {mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));mpq_set(Q,(mpq_ptr)vector_ref(v,0));mpq_abs(Q,Q);return (void*)Q;}
void *p_lfsqrt(Vector *v)    {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_sqrt(F,F,MPFR_RNDA);return (void*)F;}
void *p_csqrt(Vector *v)     {complex *c = (complex*)malloc(sizeof(complex));*c = csqrt(*(complex*)vector_ref(v,0)); return (void*)c;}

//
void *p_lfsin(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_sin(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfcos(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_cos(F,F,MPFR_RNDA);return (void*)F;}
void *p_lftan(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_tan(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfasin(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_asin(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfacos(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_acos(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfatan(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_atan(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfsinh(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_sinh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfcosh(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_cosh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lftanh(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_tanh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfasinh(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_asinh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfacosh(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_acosh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfatanh(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_atanh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lflog10(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_log10(F,F,MPFR_RNDA);return (void*)F;}
void *p_lflogE(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_log(F,F,MPFR_RNDA);return (void*)F;}
void *p_lflog(Vector *v)     {
    mpfr_ptr E = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(E);mpfr_init(F);
    mpfr_log(E,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_log(F,(mpfr_ptr)vector_ref(v,1),MPFR_RNDA);
    mpfr_div(F,F,E,MPFR_RNDA);
    return (void*)F;
}
void *p_lflog1p(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_log1p(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfexp(Vector *v)     {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_exp(F,F,MPFR_RNDA);return (void*)F;}


Funcpointer primitive_func[]  = {p_exit, p_set_prec,p_get_prec,
                                 p_print, p_open, p_close, p_gets, p_getc, p_sin, p_cos, p_tan, 
                                 p_asin, p_acos, p_atan, p_sinh, p_cosh, p_tanh, p_asinh, p_acosh, p_atanh,
                                 p_log10, p_logE, p_log, p_exp, p_iabs, p_fabs, p_isqrt, p_fsqrt,
                                 p_labs, p_rabs, p_lfabs, p_cabs, p_lsqrt, p_lfsqrt, p_csqrt,
                                 p_lfsin, p_lfcos, p_lftan,p_lfasin, p_lfacos, p_lfatan,
                                 p_lfsinh, p_lfcosh, p_lftanh,p_lfasinh, p_lfacosh, p_lfatanh,
                                 p_lflog10, p_lflogE, p_lflog, p_lflog1p, p_lfexp,NULL};
char*primitive_function_name[]={"exit", "set_prec","get_prec",
                                "print", "open", "close", "gets", "getc", "sin", "cos", "tan", 
                                "asin", "acos", "atan", "sinh", "cosh","tanh", "asinh", "acosh", "atanh",
                                "log10", "logE", "log", "exp", "iabs", "fabs", "isqrt", "fsqrt",
                                "labs", "rabs", "flabs", "cabs", "lsqrt", "lfsqrt","csqrt",
                                "lfsin","lfcos", "lftan","lfasin","lfacos","lfatan",
                                "lfsinh","lfcosh", "lftanh","lfasinh","lfacosh","lfatanh",
                                "lflog10", "lflogE", "lflog", "lflog1p", "lfexp", NULL};
int primitive_function_arglisti[][3] = {//{OBJ_GEN},                                      // print
                                {OBJ_NONE},
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
                                {OBJ_FLT, OBJ_FLT},                             // log
                                {OBJ_FLT},                                      // exp
                                {OBJ_INT},                                      // iabs
                                {OBJ_FLT},                                      // fabs
                                {OBJ_INT},                                      // isqrt
                                {OBJ_FLT},                                      // fsqrt
                                {OBJ_LINT},                                     // labs
                                {OBJ_RAT},                                      // rabs
                                {OBJ_LFLT},                                     // lfabs
                                {OBJ_CMPLX},                                    // cabs
                                {OBJ_LINT},                                     // lsqrt
                                {OBJ_LFLT},                                     // lfsqrt
                                {OBJ_CMPLX},                                    // caqrt
                                {OBJ_LFLT},                                      // lfsin
                                {OBJ_LFLT},                                      // lfcos
                                {OBJ_LFLT},                                      // lftan
                                {OBJ_LFLT},                                      // lfasin
                                {OBJ_LFLT},                                      // lfacos
                                {OBJ_LFLT},                                      // lfatan
                                {OBJ_LFLT},                                      // lfsinh
                                {OBJ_LFLT},                                      // lfcosh
                                {OBJ_LFLT},                                      // lftanh
                                {OBJ_LFLT},                                      // lfasinh
                                {OBJ_LFLT},                                      // lfacosh
                                {OBJ_LFLT},                                      // lfatanh
                                {OBJ_LFLT},                                      // lflog10
                                {OBJ_LFLT, OBJ_LFLT},                                      // lflogE
                                {OBJ_LFLT},                                      // lflog
                                {OBJ_LFLT},                                      // lflog1p
                                {OBJ_LFLT}                                      // lfexp
                                };

int primitive_function_ct[][3]  ={//{OBJ_NONE,1, TRUE},                        // print
                                {OBJ_NONE,0, FALSE},
                                {OBJ_NONE,1, FALSE},
                                {OBJ_INT, 0, FALSE},
                                {OBJ_NONE,1, TRUE},
                                {OBJ_IO,  2, FALSE},                        // open
                                {OBJ_NONE,1, FALSE},                        // close
                                {OBJ_SYM, 1, FALSE},                        // gets
                                {OBJ_SYM, 1, FALSE},                        // getc
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
                                {OBJ_FLT, 1, FALSE},                        // exp
                                {OBJ_INT, 1, FALSE},                        // iabs
                                {OBJ_FLT, 1, FALSE},                        // fabs
                                {OBJ_INT, 1, FALSE},                        // isqrt
                                {OBJ_FLT, 1, FALSE},                        // fsqrt
                                {OBJ_LINT,1, FALSE},                        // labs
                                {OBJ_RAT, 1, FALSE},                        // rabs
                                {OBJ_LFLT,1, FALSE},                        // lfabs
                                {OBJ_CMPLX,1, FALSE},                       // cabs
                                {OBJ_LINT, 1, FALSE},                       // lsqrt
                                {OBJ_LFLT, 1, FALSE},                       // lfsqrt
                                {OBJ_CMPLX,1, FALSE},                       // caqrt
                                {OBJ_LFLT, 1, FALSE},                        // lfsin
                                {OBJ_LFLT, 1, FALSE},                        // lfcos
                                {OBJ_LFLT, 1, FALSE},                        // lftan
                                {OBJ_LFLT, 1, FALSE},                        // lfasin
                                {OBJ_LFLT, 1, FALSE},                        // lfacos
                                {OBJ_LFLT, 1, FALSE},                        // lfatan
                                {OBJ_LFLT, 1, FALSE},                        // lfsinh
                                {OBJ_LFLT, 1, FALSE},                        // lfcosh
                                {OBJ_LFLT, 1, FALSE},                        // lftanh
                                {OBJ_LFLT, 1, FALSE},                        // lfasinh
                                {OBJ_LFLT, 1, FALSE},                        // lfacosh
                                {OBJ_LFLT, 1, FALSE},                        // lfatanh
                                {OBJ_LFLT, 1, FALSE},                        // lflo10
                                {OBJ_LFLT, 1, FALSE},                        // lflogE
                                {OBJ_LFLT, 2, FALSE},                        // lflog
                                {OBJ_LFLT, 1, FALSE},                        // lflog1p
                                {OBJ_LFLT, 1, FALSE},                        // lfexp
                                 };

void * make_primitive() {
    int i = 0,j;
    code_type *ct;
    Vector *v,*vv;
    char *s;
    complex *c=(complex*)malloc(sizeof(complex));
    *c=I;

    PRIMITIVE_FUNC = Hash_init(128);
    Symbol *char_I=new_symbol("I",1);
    Hash_put(G,char_I,(void*)c);
    Hash_put(GLOBAL_VAR,char_I,new_ct(OBJ_CMPLX,OBJ_NONE,(void*)0,FALSE));
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
