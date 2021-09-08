#include "generate.h"
gmp_randstate_t  RAND_STATE;
unsigned long mulmod(unsigned long a,unsigned long b,unsigned long c);

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
void*p_printf(Vector*v) {
    object *o=(object*)vector_ref(v,1);
    char *s =((Symbol*)vector_ref(v,0))->_table;
    switch(o->type) {
        case OBJ_INT:   printf(s,(long)o->data.intg);break;
        case OBJ_LINT:  mpfr_printf(s,(mpz_ptr)o->data.ptr);break;
        case OBJ_RAT:   mpfr_printf(s,(mpq_ptr)o->data.ptr);break; 
        case OBJ_FLT:   printf(s,(double)o->data.flt);break;
        case OBJ_LFLT:  mpfr_printf(s,(mpfr_ptr)o->data.ptr);break;
        case OBJ_CMPLX: printf(s,*(complex*)o->data.ptr);break;
    }
    return NULL;
}
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
    if (fgets(buff,4095,(FILE*)vector_ref(v,0))==NULL) {printf("RuntimeError:Canot Fgets\n");Throw(3);}
    long n=strlen(buff);
    Symbol*s=new_symbol(buff,n);
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
//void *p_fsin(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = sin  (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_fsin(Vector *v)      {long l = (long)vector_ref(v,0);double f =   sin(*(double*)&l); return (void*)*(long*)&f;}
void *p_fcos(Vector *v)      {long l = (long)vector_ref(v,0);double f =   cos(*(double*)&l); return (void*)*(long*)&f;}
void *p_ftan(Vector *v)      {long l = (long)vector_ref(v,0);double f =   tan(*(double*)&l); return (void*)*(long*)&f;}
void *p_fasin(Vector *v)     {long l = (long)vector_ref(v,0);double f =  asin(*(double*)&l); return (void*)*(long*)&f;}
void *p_facos(Vector *v)     {long l = (long)vector_ref(v,0);double f =  acos(*(double*)&l); return (void*)*(long*)&f;}
void *p_fatan(Vector *v)     {long l = (long)vector_ref(v,0);double f =  atan(*(double*)&l); return (void*)*(long*)&f;}
void *p_fsinh(Vector *v)     {long l = (long)vector_ref(v,0);double f =  sinh(*(double*)&l); return (void*)*(long*)&f;}
void *p_fcosh(Vector *v)     {long l = (long)vector_ref(v,0);double f =  cosh(*(double*)&l); return (void*)*(long*)&f;}
void *p_ftanh(Vector *v)     {long l = (long)vector_ref(v,0);double f =  tanh(*(double*)&l); return (void*)*(long*)&f;}
void *p_fasinh(Vector *v)    {long l = (long)vector_ref(v,0);double f = asinh(*(double*)&l); return (void*)*(long*)&f;}
void *p_facosh(Vector *v)    {long l = (long)vector_ref(v,0);double f = acosh(*(double*)&l); return (void*)*(long*)&f;}
void *p_fatanh(Vector *v)    {long l = (long)vector_ref(v,0);double f = atanh(*(double*)&l); return (void*)*(long*)&f;}
//
void *p_log10(Vector *v)    {long l = (long)vector_ref(v,0);double f = log10(*(double*)&l); return (void*)*(long*)&f;}
void *p_logE(Vector *v)    {long l = (long)vector_ref(v,0);double f = log(*(double*)&l); return (void*)*(long*)&f;}
void *p_log1p(Vector *v)    {long l = (long)vector_ref(v,0);double f = log1p(*(double*)&l); return (void*)*(long*)&f;}
void *p_log(Vector *v)    {long l = (long)vector_ref(v,0),ll = (long)vector_ref(v,1);double f = log(*(double*)&ll)/log(*(double*)&l); return (void*)*(long*)&f;}
void *p_exp(Vector *v)    {long l = (long)vector_ref(v,0);double f = exp(*(double*)&l); return (void*)*(long*)&f;}
void *p_iabs(Vector *v)     {return (void*)labs((long)vector_ref(v,0));}
void *p_fabs(Vector *v)    {long l = (long)vector_ref(v,0);double f = fabs(*(double*)&l); return (void*)*(long*)&f;}
void *p_isqrt(Vector *v)    {return (void*)(long)sqrt((double)(long)vector_ref(v,0));}
void *p_fsqrt(Vector *v)    {long l = (long)vector_ref(v,0);double f = sqrt(*(double*)&l); return (void*)*(long*)&f;}
/*
void *p_fcos(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = cos  (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_ftan(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = tan  (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_fasin(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = asin (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_facos(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = acos (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_fatan(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = atan (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_fsinh(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = sinh (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_fcosh(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = cosh (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_ftanh(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = tanh (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_fasinh(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = asinh(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_facosh(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = acosh(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_fatanh(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = atanh(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_log10(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = log10(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_logE(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = log  (*(double*)vector_ref(v,0)); return (void*)f;}
void *p_log1p(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = log1p(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_log(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = log  (*(double*)vector_ref(v,1))/log(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_exp(Vector *v)      {double *f = (double*)malloc(sizeof(double));*f = exp  (*(double*)vector_ref(v,0)); }
void *p_iabs(Vector *v)     {return (void*)labs((long)vector_ref(v,0));}
void *p_fabs(Vector *v)     {double *f = (double*)malloc(sizeof(double));*f = fabs(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_isqrt(Vector *v)    {return (void*)(long)sqrt((double)(long)vector_ref(v,0));}
void *p_fsqrt(Vector *v)    {double *f = (double*)malloc(sizeof(double));*f = sqrt(*(double*)vector_ref(v,0)); return (void*)f;}
*/
//
void *p_labs(Vector *v)     {mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set(L,(mpz_ptr)vector_ref(v,0));mpz_abs(L,L);return (void*)L;}
void *p_rabs(Vector *v)     {mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));mpq_set(Q,(mpq_ptr)vector_ref(v,0));mpq_abs(Q,Q);return (void*)Q;}
void *p_lfabs(Vector *v)    {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_abs(F,F,MPFR_RNDA);return (void*)F;}
void *p_cabs(Vector *v)     {complex *c = (complex*)malloc(sizeof(complex));*c = cabs(*(complex*)vector_ref(v,0)); return (void*)c;}
void *p_lsqrt(Vector *v)    {mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set(L,(mpz_ptr)vector_ref(v,0));mpz_sqrt(L,L);return (void*)L;}
//void *p_rsqrt(Vector *v)     {mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));mpq_set(Q,(mpq_ptr)vector_ref(v,0));mpq_abs(Q,Q);return (void*)Q;}
void *p_lfsqrt(Vector *v)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_sqrt(F,F,MPFR_RNDA);return (void*)F;}
void *p_csqrt(Vector *v)    {complex *c = (complex*)malloc(sizeof(complex));*c = csqrt(*(complex*)vector_ref(v,0)); return (void*)c;}

//
void *p_lfsin(Vector *v)    {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_sin(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfcos(Vector *v)    {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_cos(F,F,MPFR_RNDA);return (void*)F;}
void *p_lftan(Vector *v)    {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_tan(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfasin(Vector *v)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_asin(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfacos(Vector *v)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_acos(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfatan(Vector *v)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_atan(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfsinh(Vector *v)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_sinh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfcosh(Vector *v)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_cosh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lftanh(Vector *v)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_tanh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfasinh(Vector *v)  {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_asinh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfacosh(Vector *v)  {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_acosh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfatanh(Vector *v)  {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_atanh(F,F,MPFR_RNDA);return (void*)F;}
void *p_lflog10(Vector *v)  {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_log10(F,F,MPFR_RNDA);return (void*)F;}
void *p_lflogE(Vector *v)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_log(F,F,MPFR_RNDA);return (void*)F;}
void *p_lflog(Vector *v)    {
    mpfr_ptr E = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(E);mpfr_init(F);
    mpfr_log(E,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_log(F,(mpfr_ptr)vector_ref(v,1),MPFR_RNDA);
    mpfr_div(F,F,E,MPFR_RNDA);
    return (void*)F;
}
void *p_lflog1p(Vector *v)  {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_log1p(F,F,MPFR_RNDA);return (void*)F;}
void *p_lfexp(Vector *v)    {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set(F,(mpfr_ptr)vector_ref(v,0),MPFR_RNDA);mpfr_exp(F,F,MPFR_RNDA);return (void*)F;}
//
void *p_oabs(Vector *v) {return (void*)objabs((object*)vector_ref(v,0));}
void *p_osqrt(Vector *v) {return (void*)objsqrt((object*)vector_ref(v,0));}
//
void *p_osin(Vector *v) {return (void*)objsin((object*)vector_ref(v,0));}
void *p_ocos(Vector *v) {return (void*)objcos((object*)vector_ref(v,0));}
void *p_otan(Vector *v) {return (void*)objtan((object*)vector_ref(v,0));}
void *p_oasin(Vector *v) {return (void*)objasin((object*)vector_ref(v,0));}
void *p_oacos(Vector *v) {return (void*)objacos((object*)vector_ref(v,0));}
void *p_oatan(Vector *v) {return (void*)objatan((object*)vector_ref(v,0));}
void *p_osinh(Vector *v) {return (void*)objsinh((object*)vector_ref(v,0));}
void *p_ocosh(Vector *v) {return (void*)objcosh((object*)vector_ref(v,0));}
void *p_otanh(Vector *v) {return (void*)objtanh((object*)vector_ref(v,0));}
void *p_oasinh(Vector *v) {return (void*)objasinh((object*)vector_ref(v,0));}
void *p_oacosh(Vector *v) {return (void*)objacosh((object*)vector_ref(v,0));}
void *p_oatanh(Vector *v) {return (void*)objatanh((object*)vector_ref(v,0));}
//void *p_osqrt(Vector *v) {return (void*)objsqrt((object *)vector_ref(v,0));}

void *p_fgamma(Vector *v)    {long l = (long)vector_ref(v,0);double f = tgamma(*(double*)&l); return (void*)*(long*)&f;}
void *p_flgamma(Vector *v)    {long l = (long)vector_ref(v,0);double f = lgamma(*(double*)&l); return (void*)*(long*)&f;}
//void *p_fgamma(Vector *v) {double *f = (double*)malloc(sizeof(double));*f = tgamma(*(double*)vector_ref(v,0)); return (void*)f;}
//void *p_flgamma(Vector *v) {double *f = (double*)malloc(sizeof(double));*f = lgamma(*(double*)vector_ref(v,0)); return (void*)f;}
void *p_ogamma(Vector *v) {return (void*)objgamma((object*)vector_ref(v,0));}
void *p_olgamma(Vector *v) {return (void*)objlgamma((object*)vector_ref(v,0));}
//
void *p_sum(Vector *v) {object* o = objcpy(vector_ref(v,0));int i;for(i=v->_cp+1;i<v->_sp;i++) o = objadd(o,vector_ref(v,i));return (void*)o;}
void *p_vsum(Vector *v) {return p_sum((Vector*)vector_ref(v,0));}
// 数論関数
void * p_lis_prime(Vector *v) {int reps=(v->_sp<=1)?20:(int)(long)vector_ref(v,1);return  (void*)(long)mpz_probab_prime_p(vector_ref(v,0),reps);}
void * p_lnext_prime(Vector *v) {mpz_ptr r=(mpz_ptr)malloc(sizeof(MP_INT));mpz_nextprime(r,(mpz_ptr)vector_ref(v,0));return (void*)r;}
// 乱数
static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
void init_irand(unsigned long s) {z ^=s;z ^= z >>21;z ^= z<< 35; z ^= z >>4; z *=2685821657736338717L;}
void * p_irand(Vector * v) {unsigned long t=(x ^ (x << 11));x=y;y=z;z=w;return (void*) (w = (w ^ (w >>19)) ^ (t ^ (t >>8)));}
void * p_lrand(Vector *v) {mpz_ptr r = (mpz_ptr)malloc(sizeof(MP_INT));mpz_init(r); mpz_urandomm(r, RAND_STATE, (mpz_ptr)vector_ref(v, 0));return (void *)r;}
Funcpointer primitive_func[]  = {p_exit, p_set_prec,p_get_prec,
                                 p_print, p_printf, p_open, p_close, p_gets, p_getc, p_fsin, p_fcos, p_ftan, 
                                 p_fasin, p_facos, p_fatan, p_fsinh, p_fcosh, p_ftanh, p_fasinh, p_facosh, p_fatanh,
                                 p_log10, p_logE, p_log, p_exp, p_iabs, p_fabs, p_isqrt, p_fsqrt,
                                 p_labs, p_rabs, p_lfabs, p_cabs, p_lsqrt, p_lfsqrt, p_csqrt,
                                 p_lfsin, p_lfcos, p_lftan,p_lfasin, p_lfacos, p_lfatan,
                                 p_lfsinh, p_lfcosh, p_lftanh,p_lfasinh, p_lfacosh, p_lfatanh,
                                 p_lflog10, p_lflogE, p_lflog, p_lflog1p, p_lfexp, p_oabs, p_osqrt,
                                 p_osin, p_ocos, p_otan, p_oasin, p_oacos, p_oatan, p_osinh, p_ocosh, p_otanh, p_oasinh, p_oacosh, p_oatanh,
                                 p_fgamma, p_flgamma,p_ogamma, p_olgamma, p_sum, p_vsum, p_lis_prime, p_lnext_prime, p_irand, p_lrand, NULL};
char*primitive_function_name[]={"exit", "set_prec","get_prec",
                                "print", "printf", "open", "close", "gets", "getc", "fsin", "fcos", "ftan", 
                                "fasin", "facos", "fatan", "fsinh", "fcosh","ftanh", "fasinh", "facosh", "fatanh",
                                "log10", "logE", "log", "exp", "iabs", "fabs", "isqrt", "fsqrt",
                                "labs", "rabs", "flabs", "cabs", "lsqrt", "lfsqrt","csqrt",
                                "lfsin","lfcos", "lftan","lfasin","lfacos","lfatan",
                                "lfsinh","lfcosh", "lftanh","lfasinh","lfacosh","lfatanh",
                                "lflog10", "lflogE", "lflog", "lflog1p", "lfexp", "abs", "sqrt", 
                                "sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh","tanh", "asinh", "acosh", "atanh",
                                "fgamma", "flgamma", "gamma", "lgamma", "sum", "vsum", "lis_prime", "lnext_prime", "irand", "lrand",NULL};
int primitive_function_arglisti[][3] = {//{OBJ_GEN},                                      // print
                                {OBJ_NONE},
                                {OBJ_INT},                                      // set_prec
                                {OBJ_NONE},                                     // get_prec
                                {OBJ_GEN},                                      // print
                                {OBJ_SYM,OBJ_GEN},                              // printf
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
                                {OBJ_LFLT},                                      // lflogE
                                {OBJ_LFLT, OBJ_LFLT},                            // lflog
                                {OBJ_LFLT},                                      // lflog1p
                                {OBJ_LFLT},                                      // lfexp
                                {OBJ_GEN },                                      // abs
                                {OBJ_GEN},                                        // sqrt
                                {OBJ_GEN},                                       // sin
                                {OBJ_GEN},//cos
                                {OBJ_GEN},//tan
                                {OBJ_GEN},//asin
                                {OBJ_GEN},//acos
                                {OBJ_GEN},//atan
                                {OBJ_GEN},//sinh
                                {OBJ_GEN},//cosh
                                {OBJ_GEN},//tanh
                                {OBJ_GEN},//asinh
                                {OBJ_GEN},//acosh
                                {OBJ_GEN},//atanh
                                //{OBJ_GEN} //sqrt
                                {OBJ_FLT},//fgamma
                                {OBJ_FLT},//flgamma
                                {OBJ_GEN},//ogamma
                                {OBJ_GEN},//olgamma
                                {OBJ_GEN},//sum
                                {OBJ_VECT},//vsum
                                {OBJ_LINT, OBJ_INT},//is_prime
                                {OBJ_LINT},//next_prime
                                {OBJ_NONE} ,//irand
                                {OBJ_LINT} //lrand
                                };

int primitive_function_ct[][3]  ={//{OBJ_NONE,1, TRUE},                        // print
                                {OBJ_NONE,0, FALSE},
                                {OBJ_NONE,1, FALSE},
                                {OBJ_INT, 0, FALSE},
                                {OBJ_NONE,1, TRUE},
                                {OBJ_NONE,2, FALSE},                        // printf
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
                                {OBJ_GEN,  1, FALSE},                        // abs
                                {OBJ_GEN,  1, FALSE},                         // sqrt
                                {OBJ_GEN,  1, FALSE}, // sin
                                {OBJ_GEN,  1, FALSE}, // cos
                                {OBJ_GEN,  1, FALSE}, // tan
                                {OBJ_GEN,  1, FALSE}, // asin
                                {OBJ_GEN,  1, FALSE}, // acos
                                {OBJ_GEN,  1, FALSE}, // atan
                                {OBJ_GEN,  1, FALSE}, // sinh
                                {OBJ_GEN,  1, FALSE}, // cosh
                                {OBJ_GEN,  1, FALSE}, // tanh
                                {OBJ_GEN,  1, FALSE}, // asinh
                                {OBJ_GEN,  1, FALSE}, // acosh
                                {OBJ_GEN,  1, FALSE}, // atanh
                                //{OBJ_GEN,  1, FALSE}  // sqrt
                                {OBJ_FLT,  1, FALSE}, // fgamma 
                                {OBJ_FLT,  1, FALSE}, // flgamma 
                                {OBJ_GEN,  1, FALSE}, // ogamma 
                                {OBJ_GEN,  1, FALSE}, // olgamma
                                {OBJ_GEN,  1, TRUE} , // sum
                                {OBJ_GEN,  1,FALSE} ,  //vsum
                                {OBJ_INT, 2,TRUE},//is_prime
                                {OBJ_LINT,1,FALSE} ,//next_prime
                                 {OBJ_INT,0,FALSE},//irand
                                 {OBJ_LINT,1,FALSE}
                                 };

void * make_primitive() {
    int i = 0,j;
    code_type *ct;
    Vector *v,*vv;
    char *s;
    complex *c=(complex*)malloc(sizeof(complex));
    *c=I;
    gmp_randinit_default(RAND_STATE);

    PRIMITIVE_FUNC = Hash_init(128);
    Symbol *char_I=new_symbol("I",1);
    Symbol *char_NONE = new_symbol("None",4);
    Hash_put(G,char_I,(void*)c);
    Hash_put(G,char_NONE,(void*)0);
    Hash_put(GLOBAL_VAR,char_I,new_ct(OBJ_CMPLX,OBJ_NONE,(void*)0,FALSE));
    Hash_put(GLOBAL_VAR,char_NONE,new_ct(OBJ_NONE,OBJ_NONE,(void*)0,FALSE));
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
        vv=vector_init(3);
        push(vv,(void*)FUNC_PRIM);push(vv,(void*)primitive_func[i]);
        //Hash_put(G,new_symbol(s,n),(void*)primitive_func[i]);
        Hash_put(G,new_symbol(s,n),(void*)vv);
        i++;
    }
}
