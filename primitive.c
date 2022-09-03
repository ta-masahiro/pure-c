#include "generate.h"
#include <time.h>
gmp_randstate_t  RAND_STATE;
unsigned long mulmod(unsigned long a,unsigned long b,unsigned long c);
//
// primtive関数は引数としてvoid**(void*へのpointer)と引数の個数をもらい
// 第一引数の場所に結果をsetする
// func(void **p, int n)のとき第一引数は*p、第二引数は*(p+1)、第ｎ引数は*(p+n-1)
// 値を返さない関数はNULLをsetすること
//
void p_exit() {exit(0);}
#ifndef VMTEST
// 共通global環境、コンパイラの変数テーブル、primitive関数テーブルから削除する
void p_forget(void** sp, int n) {
    Hash_del(G, (Symbol*)*sp);
    Hash_del(GLOBAL_VAR, (Symbol*)*sp);
    Hash_del(PRIMITIVE_FUNC, (Symbol*)*sp);
    *(sp) = NULL;
}
#endif
void p_set_prec(void **sp, int n) {
    if (n == 1) mpfr_set_default_prec((long)*sp);
    else if(n == 2) mpfr_set_prec(obj2lflt((object*)*(sp+1)),(long)*sp);
    //else {printf("no!\n");Throw(3); 
    *(sp) = NULL;
}
void p_get_prec(void **sp, int n) {
    int i;
    if (n == 0) {i = mpfr_get_default_prec(); sp++;}
    else i = mpfr_get_prec(obj2lflt((object*)*sp));
    *(sp) = (void*)(long)i;
}
// io
void p_print(void **sp, int n) {
    for(int i = 0; i < n; i ++) {
        fputs(objtostr((object*)*(sp + i)),stdout); 
        fputs(" ",stdout);
    }
    fputs("\n",stdout);
    *(sp) = NULL;
}
void p_printf(void **sp,int n) {
    char *s = ((Symbol*)*(sp))->_table;
    object *o=(object*)*(sp+1);
    switch(o->type) {
        case OBJ_INT:   printf(s, (long)o->data.intg);          break;
        case OBJ_LINT:  mpfr_printf(s, (mpz_ptr)o->data.ptr);   break;
        case OBJ_RAT:   mpfr_printf(s, (mpq_ptr)o->data.ptr);   break; 
        case OBJ_FLT:   printf(s, (double)o->data.flt);         break;
        case OBJ_LFLT:  mpfr_printf(s, (mpfr_ptr)o->data.ptr);  break;
        case OBJ_CMPLX: printf(s, *(complex*)o->data.ptr);      break;
    }
    *(sp) = NULL;
}
void p_open(void **sp, int n) {
    Symbol *path = *(sp);
    Symbol *mode = *(sp+1);
    FILE *f = (FILE*)fopen(path->_table,mode->_table);
    if (f == NULL) printf("I/O Error in openning:%s\n",path->_table);
    *(sp) = (void*)f;
}

void p_close(void **sp, int n) {
    fclose((FILE*)*sp);
    *(sp) == NULL;
}
#define BUFSIZE 4096
void p_gets(void **sp, int m) {
    char buff[BUFSIZE];
    if (fgets(buff, BUFSIZE, (FILE*)*sp)==NULL) {printf("RuntimeError:Canot Fgets\n"); Throw(3);}
    long n = strlen(buff);
    Symbol *s = new_symbol(buff, n);
    *(sp) = (void*)s;
}

void p_puts(void **sp, int n) {
    if (fputs(((Symbol*)*sp)->_table, (FILE*)*(sp+1)) == EOF) {;}
    *(sp) = NULL;
}

void p_getc(void **sp, int n) {
    
    char c=fgetc((FILE*)*sp);
    Symbol *s = new_symbol(&c,1);
    *(sp) = (void*)s;    
}
void p_get_time(void **sp, int n) {
    struct timespec Time;
    clock_gettime(CLOCK_REALTIME,&Time);
    double d = Time.tv_sec+Time.tv_nsec/(1.0e+9);
    *(sp) = (void*)*(long *)&d;}
// 数学関数
// float
#define math_func(g) void p_f##g(void **sp, int n) {long l = (long)*sp;double f = g(*(double*)&l); *(sp) = (void*)*(long*)&f;}
math_func(sin)
math_func(cos)
math_func(tan)
math_func(asin)
math_func(acos)
math_func(atan)
math_func(sinh)
math_func(cosh)
math_func(tanh)
math_func(asinh)
math_func(acosh)
math_func(atanh)
math_func(log10)
math_func(log1p)
math_func(exp)
// 自然対数
void p_flogE(void **sp, int n) {
    long l = (long)*sp;
    double f = log(*(double*)&l); 
    *(sp) = (void*)*(long*)&f;
}
// xを底とするyの対数 LOG(x,y)
void p_flog(void **sp, int n) {
    long l = (long)*(sp);long ll = (long)*(sp+1);
    double f = log(*(double*)&ll)/log(*(double*)&l); 
    *(sp) = (void*)*(long*)&f;
}
// abs
void p_iabs  (void **sp, int n)   {*(sp) = (void*)labs((long)*sp);}
void p_fabs  (void **sp, int n)   {long     l = (long)*sp; double f = fabs(*(double*)&l); (*sp) = (void*)*(long*)&f;}
void p_labs  (void **sp, int n)   {mpz_ptr  L = (mpz_ptr)malloc(sizeof(MP_INT)); mpz_init_set(L,(mpz_ptr)*sp); mpz_abs(L,L); *(sp) = (void*)L;}
void p_rabs  (void **sp, int n)   {mpq_ptr  Q = (mpq_ptr)malloc(sizeof(MP_RAT)); mpq_set(Q,(mpq_ptr)*sp); mpq_abs(Q,Q); *(sp) = (void*)Q;}
void p_lfabs (void **sp, int n)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct)); mpfr_init_set(F,(mpfr_ptr)*sp,MPFR_RNDN); mpfr_abs(F,F,MPFR_RNDN); *(sp) = (void*)F;}
void p_cabs  (void **sp, int n)   {double   c = cabs(*(complex*)*sp); *(sp) = &c;}
// sqrt
void p_isqrt (void **sp, int n)   {*(sp) = (void*)(long)sqrt((double)(long)*sp);}
void p_fsqrt (void **sp, int n)   {long     l = (long)*sp;double f = sqrt(*(double*)&l); *(sp) = (void*)*(long*)&f;}
void p_lsqrt (void **sp, int n)   {mpz_ptr  L = (mpz_ptr)malloc(sizeof(MP_INT)); mpz_init_set(L,(mpz_ptr)*sp); mpz_sqrt(L,L);*(sp) = (void*)L;}
void p_lfsqrt(void **sp, int n)   {mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct)); mpfr_init_set(F,(mpfr_ptr)*sp,MPFR_RNDN); mpfr_sqrt(F,F,MPFR_RNDN);*(sp) = (void*)F;}
void p_csqrt (void **sp, int n)   {complex *c = (complex*)malloc(sizeof(complex)); *c = csqrt(*(complex*)*sp); *(sp) = (void*)c;}

// long float
void lf_function(void **sp, int (*function)(__mpfr_struct * f, const __mpfr_struct * g, mpfr_rnd_t r))  {
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init_set(F,(mpfr_ptr)*sp, MPFR_RNDN);
    function(F, F, MPFR_RNDN);
    *(sp) = (void*)F;
}
//
void p_lfsin  (void **sp, int n) { lf_function(sp, mpfr_sin  );}
void p_lfcos  (void **sp, int n) { lf_function(sp, mpfr_cos  );}
void p_lftan  (void **sp, int n) { lf_function(sp, mpfr_tan  );}
void p_lfasin (void **sp, int n) { lf_function(sp, mpfr_asin );}
void p_lfacos (void **sp, int n) { lf_function(sp, mpfr_acos );}
void p_lfatan (void **sp, int n) { lf_function(sp, mpfr_atan );}
void p_lfsinh (void **sp, int n) { lf_function(sp, mpfr_sinh );}
void p_lfcosh (void **sp, int n) { lf_function(sp, mpfr_cosh );}
void p_lftanh (void **sp, int n) { lf_function(sp, mpfr_tanh );}
void p_lfasinh(void **sp, int n) { lf_function(sp, mpfr_asinh);}
void p_lfacosh(void **sp, int n) { lf_function(sp, mpfr_acosh);}
void p_lfatanh(void **sp, int n) { lf_function(sp, mpfr_atanh);}
void p_lflog10(void **sp, int n) { lf_function(sp, mpfr_log10);}
void p_lflogE (void **sp, int n) { lf_function(sp, mpfr_log  );}
void p_lflog1p(void **sp, int n) { lf_function(sp, mpfr_log1p);}
void p_lfexp  (void **sp, int n) { lf_function(sp, mpfr_exp  );}
void p_lflog(void **sp, int n)   {
    //mpfr_ptr E = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_t E;
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(E); mpfr_init(F);
    mpfr_log(E, (mpfr_ptr)*(sp--), MPFR_RNDN); mpfr_log(F, (mpfr_ptr)*sp, MPFR_RNDN);
    mpfr_div(F, F, E, MPFR_RNDN);
    *(sp) = (void*)F;
}
//
void p_oabs  (void **sp, int n) {*(sp) = (void*)objabs((object*)*sp);}
void p_osqrt (void **sp, int n) {*(sp) = (void*)objsqrt((object*)*sp);}
//
void p_osin  (void **sp, int n) {*(sp) = (void*)objsin((object*)*sp);}
void p_ocos  (void **sp, int n) {*(sp) = (void*)objcos((object*)*sp);}
void p_otan  (void **sp, int n) {*(sp) = (void*)objtan((object*)*sp);}
void p_oasin (void **sp, int n) {*(sp) = (void*)objasin((object*)*sp);}
void p_oacos (void **sp, int n) {*(sp) = (void*)objacos((object*)*sp);}
void p_oatan (void **sp, int n) {*(sp) = (void*)objatan((object*)*sp);}
void p_osinh (void **sp, int n) {*(sp) = (void*)objsinh((object*)*sp);}
void p_ocosh (void **sp, int n) {*(sp) = (void*)objcosh((object*)*sp);}
void p_otanh (void **sp, int n) {*(sp) = (void*)objtanh((object*)*sp);}
void p_oasinh(void **sp, int n) {*(sp) = (void*)objasinh((object*)*sp);}
void p_oacosh(void **sp, int n) {*(sp) = (void*)objacosh((object*)*sp);}
void p_oatanh(void **sp, int n) {*(sp) = (void*)objatanh((object*)*sp);}
void p_olog10(void **sp, int n) {*(sp) = (void*)objlog10((object*)*sp);}
void p_ologE (void **sp, int n) {*(sp) = (void*)objlogE((object*)*sp);}
void p_olog1p(void **sp, int n) {*(sp) = (void*)objlog10((object*)*sp);}
void p_oexp  (void **sp, int n) {*(sp) = (void*)objexp((object*)*sp);}
void p_olog  (void **sp, int n) {*(sp-1) = (void*)objdiv(objlogE((object*)*(sp-1)), objlogE((object*)*sp));}
void p_ofloor(void **sp, int n) {*(sp) = (void *)objfloor((object*)*sp);}
// constnt
void p_lpi(void **sp, int n) {mpfr_ptr r = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init2(r,(long)*sp);mpfr_const_pi(r,MPFR_RNDN);*(sp) = (void*)r;}
//void p_lpi(void **sp, int n) {mpfr_ptr r = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init(r);mpfr_const_pi(r,MPFR_RNDN);*(sp) = (void*)r;}
void p_llog2(void **sp, int n) {mpfr_ptr r = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init2(r,(long)*sp);mpfr_const_log2(r,MPFR_RNDN);*(sp) = (void*)r;}
void p_fgamma(void **sp, int n)    {long l = (long)*sp;double f = tgamma(*(double*)&l); *(sp) = (void*)*(long*)&f;}
void p_flgamma(void **sp, int n)    {long l = (long)*sp;double f = lgamma(*(double*)&l); *(sp) = (void*)*(long*)&f;}
//void p_fgamma(void **sp, int n) {double *f = (double*)malloc(sizeof(double));*f = tgamma(*(double*)sp); *(sp) = (void*)f;}
//void p_flgamma(void **sp, int n) {double *f = (double*)malloc(sizeof(double));*f = lgamma(*(double*)sp); *(sp) = (void*)f;}
void p_ogamma(void **sp, int n) {*(sp) = (void*)objgamma((object*)*sp);}
void p_olgamma(void **sp, int n) {*(sp) = (void*)objlgamma((object*)*sp);}
//
void p_sum(void **sp, int n) {
    if (n == 0) {
        *(sp) = (void*)newINT(0); 
        return;} 
    object* o = objcpy(*sp);
    for(int i = 1; i < n; i++) o = objadd(o, (object *)*(sp+i)); 
    *(sp) = (void*)o;
}
void p_vsum(void **sp, int n) {
    object *o =newINT(0);
    Vector *v;//printf("%d\n",n);
    for(int j = 0; j < n; j++) {
        if (((object*)*(sp + j))->type != OBJ_VECT) {printf("DataError:引数はvectorの必要があります\n");Throw(3);}
        v = (Vector*)(((object*)*(sp + j))->data.ptr);//printf("%d\n",v->_sp);
        //if (v->_sp == 0) continue; 
        for(int i = 0; i < v->_sp; i ++) {
            o = objadd(o, (object*)(v->_table[i])); //printf("%s\n",objtostr(o));
        }
    }
    *sp = (void*)o;
}
void p_irange(void ** sp, int m) {
    int n = (int)(long)*sp;
    Vector *r = vector_init(n);
    for (int i = 0; i < n; i ++) push(r, (void*)newINT((long)i));
    *(sp) = (void*)r;
}
// vector swap : vector[i]<->vector[j]
void p_vswap(void **sp, int n) {
    Vector *t = (Vector*)*(sp);
    int i = (int)(long)*(sp+1);
    int j = (int)(long)*(sp+2);
    void* w = t->_table[i]; t->_table[i] = t->_table[j]; t->_table[j] = w;
    *(sp) = NULL;
}
//  sorting !!うまく動いていない
int __cmp__(const void* x,const void* y) {return objcmp((object*)x,(object*)y);}
void p_sort(void **sp, int n) {
    Vector *v=(Vector *)*sp;
    int data_size  = v->_sp;
    void * data_pt = v->_table;
    //qsort(v->_table, v->_sp, sizeof(void*), cmp);
    qsort(data_pt, v->_sp, 8, __cmp__);
    //qsort(v->_table, v->_sp, 8, objcmp);
    *(sp) = NULL;
}
void p_ddel(void **sp, int n) {Hash_del((Hash*)*sp, (Symbol*)*(sp +1)); *(sp) = NULL;}
void p_vdel(void **sp, int n) {Vector*vv=(Vector*)*(sp) ;long index=(long)*(sp+1); if (index<0) index=vv->_sp+index;vector_delete(vv, index); *(sp) = NULL;}
void p_vins(void **sp, int n) {vector_insert((Vector*)*sp, (long)*(sp + 1), (void*)*(sp + 2)); *(sp) = NULL;}

void p_cmp(void **sp, int n) {*(sp) = (void*)(long)objcmp((object*)*sp, (object*)*(sp+1));}
void p_fact(void **sp, int n) {mpz_ptr r = (mpz_ptr)malloc(sizeof(MP_INT)); mpz_init(r); mpz_fac_ui(r, (long)*sp); *(sp) = (void*)r;}
void p_fib(void **sp, int n) {mpz_ptr r = (mpz_ptr)malloc(sizeof(MP_INT)); mpz_init(r); mpz_fib_ui(r, (long)*sp); *(sp) = (void*)r;}
// 数論関数
void p_lis_prime(void **sp, int n) {int reps = (1<=1)?20:(int)(long)*(sp+1); *(sp) = (void*)(long)mpz_probab_prime_p((mpz_ptr)*sp, reps);}
void p_lnext_prime(void **sp, int n) {mpz_ptr r=(mpz_ptr)malloc(sizeof(MP_INT));mpz_nextprime(r,(mpz_ptr)*sp); *(sp) = (void*)r;}
// 乱数
static unsigned long _X=123456789,_Y=362436069,_Z=521288629,_W=88675123;
//void p_init_irand(void **sp, int n) {_Z ^=(unsigned long)sp;_Z ^= _Z >>21;_Z ^= _Z<< 35; _Z ^= _Z >>4; _Z *=2685821657736338717L;return (void*)0;}
void p_init_irand(void **sp, int n) {_W ^=(unsigned long)*sp;_W ^= _W >>21;_W ^= _W<< 35; _W ^= _W >>4; _W *=2685821657736338717L;*sp = NULL;}
//void p_init_irand(void **sp, int n) {unsigned long t;_W ^=(unsigned long)sp;_W ^= _W >>21;_W ^= _W<< 35; _W ^= _W >>4; _W *=2685821657736338717L;
//    for(int i=0;i<3;i++) {t=(_X ^ (_X << 11));_X=_Y;_Y=_Z;_Z=_W;(_W = (_W ^ (_W >>19)) ^ (t ^ (t >>8)));};return (void*)0;}
void p_init_lrand(void **sp, int n) { gmp_randseed(RAND_STATE, (mpz_ptr)*sp); *sp = NULL;}
void p_irand(void **sp, int n) {unsigned long t=(_X ^ (_X << 11));_X=_Y;_Y=_Z;_Z=_W; *sp = (void*) (_W = (_W ^ (_W >>19)) ^ (t ^ (t >>8)));}
void p_lrand(void **sp, int n) {mpz_ptr r = (mpz_ptr)malloc(sizeof(MP_INT));mpz_init(r); mpz_urandomm(r, RAND_STATE, (mpz_ptr)*sp); *sp =(void *)r;}
#include "ntheory.c"
void p_trial_div(void **sp, int n) {*sp = (void*)trial_div((mpz_ptr)*sp);}
void p_pollard_rho(void **sp, int m) {
    mpz_ptr r= (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(r);
    void * n;
    if (mpz_probab_prime_p((mpz_ptr)(n=*sp),20)) {*(sp) = (void*)n; return;}
    if (pollard_rho((mpz_ptr)n, r, (mpz_ptr)*(sp+1), (mpz_ptr)*(sp+2), (long)*(sp+3), (long)*(sp+4))) {
        *(sp) = (void*)r; return;
    }
    mpz_ptr iz=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init_set_ui(iz,0);
    *(sp) = (void*)iz;
}
void p_pollard_pm1(void **sp, int m) {
    mpz_ptr r= (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(r); mpz_ptr n;
    if (mpz_probab_prime_p(n=(mpz_ptr)*sp,20)) {*(sp) = (void*)n; return;}
    if (pollard_pm1(n, r, (long)*(sp+1), (long)*(sp+2))) {
        *(sp) = (void*)r; return;
    }
    mpz_ptr iz = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init_set_ui(iz, 0);
    *(sp) = (void*)iz;
}
void p_fermat(void **sp, int m) {
    mpz_ptr r= (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(r); mpz_ptr n;
    if (mpz_probab_prime_p(n=(mpz_ptr)*sp,20)) {*sp = (void*)n; return;}
    fermat(n,r);
    *sp = (void*)r;
}
/*
#include "ecm.h"
void * p_factor(Vector *v) {
    void* n= vector_ref(v,0);
    //ecm_params p;
    mpz_ptr f = (mpz_ptr)malloc(sizeof(MP_INT));mpz_init(f);
    //ecm_init (p);
    long B1 = (long)vector_ref(v,1);
    int i;
    //if ((i=ecm_factor (f, (mpz_ptr)n, *(double*)&B1, p)) == 0) return n;
    if ((i=ecm_factor (f, (mpz_ptr)n, *(double*)&B1, NULL)) == 0) return n;
    if (i<0) {mpz_set_ui(f,0);return (void*)f;}
    return (void*)f;
}
*/
int lucasLehmerTest(int p) {
    mpz_t m, s, sq; mpz_init(sq);
    // m = 2^p -1　判定すべきメルセンヌ数
    mpz_init_set_ui(m, 1); mpz_mul_2exp(m, m, p); mpz_sub_ui(m, m, 1);
    // s = 4
    mpz_init_set_ui(s, 4);
    for (int i = 2; i < p; i ++ ) {
        // s = (s*s & m) + (s*s)>>p     として、 s >= m なら s = s - m、 そうでないなら s = s - 2 とすれば
        // 除算を使うことなく s = (s*s-2)%m が計算できる※mはpビットgべてが1dある数なので
        mpz_mul(sq, s, s);
        mpz_and(s, sq, m);
        mpz_div_2exp(sq, sq, p);
        mpz_add(s, s, sq);
        // if s >= m then s-=m else s-=2
        if (mpz_cmp(s, m)>=0) mpz_sub(s, s, m);
        mpz_sub_ui(s, s, 2);
    }
    if (mpz_cmp_ui(s, 0) == 0) return TRUE;
    return FALSE;
}
void p_lucas(void **sp, int n) {*sp = (void*)(long)lucasLehmerTest((int)(long)*sp);}
//
void p_str(void **sp, int n) {*sp = (void*)obj2symbol((object*)*sp);}
//void p_hex_str(void **sp, int n) {long d,rem,val=(long)*sp;char*s=(char*)malloc(18*sizeof(char));if (val<0) {val=-val;s[0]='-';} else s[0]=' ';int i;for(i=1;i<17;i++) {d=val/16;rem=val-d*16;s[17-i]= (rem<10) ? rem +'0' : rem -10+ 'A';val=d;}s[17]='\0';*sp = (void*)new_symbol(s,17);}
void p_hex_str(void **sp, int n) {unsigned long d,rem,val=(long)*sp;char*s=(char*)malloc(17*sizeof(char));int i;for(i=0;i<16;i++) {d=val/16;rem=val-d*16;s[15-i]= (rem<10) ? rem +'0' : rem -10+ 'A';val=d;}s[16]='\0';*sp = (void*)new_symbol(s,17);}
void p_as_float(void **sp, int n) {;}
void p_as_int(void **sp, int n) {;}
void p_type(void **sp, int n) {*sp = (void*)(long)((object *)*sp)->type;}
void p_str_search(void **sp, int n) {*(sp) = (void*)(long)symbol_search((Symbol*)*sp, (Symbol*)*(sp+1));}
void p_copy(void **sp, int n) {*sp = (void*)objcpy((object*)*sp);}
void p_system(void **sp, int n) { if (system(((Symbol*)*sp)->_table) != 0 ) perror("canot exec command"); *sp = NULL;}
void p_popen(void **sp, int n) {
    FILE * fp;
    char *buff,*dummy;
    char * cmdline = ((Symbol *)*sp)->_table;
    if ( (fp=popen(cmdline,"r")) ==NULL) {
		perror ("can not exec commad");
		*sp = NULL; return;
	}
    buff = (char*)malloc(8192*sizeof(char));
	//if (fgets(buff, sizeof(buff), fp) == NULL) {printf("FileErroe!\n");Throw(3);}
	dummy = fgets(buff, sizeof(buff), fp);
	Symbol * s = new_symbol(buff,strlen(buff));
	while (!feof(fp)) {
        buff = (char*)malloc(8192*sizeof(char));
		//if (fgets(buff, sizeof(buff), fp) == NULL) {printf("FileErroe!\n");Throw(3);};
		dummy = fgets(buff, sizeof(buff), fp);
		s = symbol_append(s, new_symbol(buff,strlen(buff)));
	}
    pclose(fp);
    *sp = (void*)s;
}
extern code_ret * str_compile(Symbol *s);
extern object * code_eval(code_ret *s);
extern void disassy(Vector * code, int indent, FILE*fp);
#ifndef VMTEST
extern void code_load(FILE *f);
void p_compile(void **sp, int n) {void * vv = str_compile((Symbol*)*sp);if (vv) {*sp = vv;return;} else {printf("Canot Compile!\n");Throw(3);}}
void p_dis_assy(void **sp, int n) {disassy(((code_ret *)*sp)->code, 0, stdout);*sp = NULL;}
void p_eval(void **sp, int n) {*sp = (void*)code_eval((code_ret *)*sp);}
void p_load(void **sp, int n) {FILE * f = fopen(((Symbol*)*sp)->_table,"r");code_load(f);fclose(f);*sp = NULL;}
#endif
void p_keys(void **sp, int n) {
    Hash *h = (Hash*)*sp;
    Vector *r = vector_init(10 );
    Symbol *key;
    for(int i=0; i < (h->size); i++) {
        key = h->hashTable[i].key;
        if (key != NULL) push(r, newOBJ(OBJ_SYM, (void*) key));
    }
    *sp = (void*)r;
}

void p_num(void **sp, int n) {mpz_ptr l=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(l);mpq_get_num(l,(mpq_ptr)*sp);*sp = (void*)l;}
void p_den(void **sp, int n) {mpz_ptr l=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(l);mpq_get_den(l,(mpq_ptr)*sp);*sp = (void*)l;}
void p_real(void **sp, int n) {double r=creal(*(complex*)*sp);*sp = (void*)(*(long*)&r);}
void p_imag(void **sp, int n) {double r=cimag(*(complex*)*sp);*sp = (void*)(*(long*)&r);}
void p_arg(void **sp, int n) {double r=carg(*(complex*)*sp);*sp = (void*)(*(long*)&r);}
//
void p_a2v(void **sp, int n) {int array_index=0;*sp = (void *)array2vector((array*)*sp, &array_index, 0);}
void p_v2a(void **sp, int n) {*sp = (void *)vector2array((Vector*)*sp);}
void p_solv_liner(void **sp, int n) {*(sp) = (void *)solv_liner((array*)*sp, (array*)*(sp+1));}
void p_make_zero(void **sp, int n) {int size[2];size[0]=(int)(long)*sp;size[1]=(int)(long)*(sp+1);*(sp) = (void*)array_init(4, 2, size);}
void p_array_inv(void **sp, int n) {*sp = (void*)array_inv((array*)*sp);}
void p_make_eye(void **sp, int n) {*sp = (void*)array_eye((int)(long)*sp);}
void p_eigen(void **sp, int n) {
    Vector *r = eigen((array*)*sp);
    vector_set(r,0,(void*)newOBJ(OBJ_ARRAY, vector_ref(r,0)));  // eigen value
    printf("eigenval OK!\n");
    Vector *l_ev = (Vector*)vector_ref(r,1);  // l eigen_vector
    for(int i=0; i<l_ev->_sp; i++) {vector_set(l_ev, i, (void*)newOBJ(OBJ_ARRAY, vector_ref(l_ev, i)));}
    vector_set(r,1,(void*)newOBJ(OBJ_VECT, l_ev));
    printf("eigenvect l OK!\n");

    Vector *r_ev = (Vector*)vector_ref(r,2);  // r eigen_vector
    for(int i=0; i<r_ev->_sp; i++) {vector_set(r_ev, i, (void*)newOBJ(OBJ_ARRAY, vector_ref(r_ev, i)));}
    vector_set(r,2,(void*)newOBJ(OBJ_VECT, r_ev));  
    printf("eigenvect r OK!\n");
    *sp = (void*)r;
    }
void p_array_type_change(void **sp, int n){array*a=(array*)*sp;change_array_type(a,(int)(long)*(sp+1));*(sp) = (void*)a;}
void p_sym2escsym(void **sp, int n) { *sp = (void*)symbol2escsymbol((Symbol *)*sp);}

void p_make_vector(void **sp, int n) {*sp = vector_init((long)*sp);}

Pfuncpointer primitive_func[]  = {p_exit,
#ifndef VMTEST 
                                 p_forget, 
#endif
                                 p_set_prec,p_get_prec,
                                 p_print, p_printf, p_open, p_close, p_gets, p_puts,p_getc, p_get_time, p_fsin, p_fcos, p_ftan, 
                                 p_fasin, p_facos, p_fatan, p_fsinh, p_fcosh, p_ftanh, p_fasinh, p_facosh, p_fatanh,
                                 p_flog10, p_flogE, p_flog, p_fexp, p_iabs, p_fabs, p_isqrt, p_fsqrt,
                                 p_labs, p_rabs, p_lfabs, p_cabs, p_lsqrt, p_lfsqrt, p_csqrt,
                                 p_lfsin, p_lfcos, p_lftan,p_lfasin, p_lfacos, p_lfatan,
                                 p_lfsinh, p_lfcosh, p_lftanh,p_lfasinh, p_lfacosh, p_lfatanh,
                                 p_lflog10, p_lflogE, p_lflog, p_lflog1p, p_lfexp, p_oabs, p_osqrt,
                                 p_osin, p_ocos, p_otan, p_oasin, p_oacos, p_oatan, p_osinh, p_ocosh, p_otanh, p_oasinh, p_oacosh, p_oatanh, 
                                 p_olog10, p_ologE, p_olog1p, p_olog, p_oexp, p_ofloor,
                                 p_lpi, p_llog2, p_fgamma, p_flgamma,p_ogamma, p_olgamma, p_sum, p_vsum, p_irange, p_vswap, p_sort, p_cmp, p_ddel, p_vdel, p_vins, p_fact, p_fib, p_lis_prime, p_lnext_prime,
                                 p_init_irand, p_init_lrand, p_irand, p_lrand, p_trial_div, p_pollard_rho, p_pollard_pm1, p_fermat,//p_factor, 
                                 p_hex_str, p_as_float, p_as_int, p_lucas, p_str, p_str_search, p_type, p_copy, p_system, p_popen,
#ifndef VMTEST
                                 p_compile, p_dis_assy, p_eval, p_load,
#endif 
                                 p_keys, p_num, p_den, p_real, p_imag, p_arg, p_a2v, p_v2a, p_solv_liner, p_array_inv, p_make_eye, p_make_zero,
                                 p_eigen, p_array_type_change, p_sym2escsym, p_make_vector, NULL };
char*primitive_function_name[]={"exit",
#ifndef VMTEST 
                                "forget",
#endif 
                                "set_prec","get_prec",
                                "print", "printf", "open", "close", "gets", "puts","getc", "get_time", "fsin", "fcos", "ftan", 
                                "fasin", "facos", "fatan", "fsinh", "fcosh","ftanh", "fasinh", "facosh", "fatanh",
                                "flog10", "flogE", "flog", "fexp", "iabs", "fabs", "isqrt", "fsqrt",
                                "labs", "rabs", "flabs", "cabs", "lsqrt", "lfsqrt","csqrt",
                                "lfsin","lfcos", "lftan","lfasin","lfacos","lfatan",
                                "lfsinh","lfcosh", "lftanh","lfasinh","lfacosh","lfatanh",
                                "lflog10", "lflogE", "lflog", "lflog1p", "lfexp", "abs", "sqrt", 
                                "sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh","tanh", "asinh", "acosh", "atanh", 
                                "log10", "logE", "log1p", "log", "exp", "floor",
                                "lpi", "llog2","fgamma", "flgamma", "gamma", "lgamma", "sum", "vsum", "irange", "vswap","qsort", "cmp", "ddel", "vdel", "vins", "fact", "fib", "lis_prime", "lnext_prime", 
                                "init_irand", "init_lrand", "irand", "lrand", "trial_div", "pollard_rho", "pollard_pm1", "fermat",//"factor", 
                                "hexstr", "asfloat", "asint", "lucas", "str", "str_search", "type", "copy", "system", "popen",
#ifndef VMTEST
                                "compile", "dis_assy", "eval", "load", 
#endif
                                "keys", "num", "den", "real", "imag", "arg", "a2v", "v2a", "solv_liner", "array_inv", "make_eye", "make_zero", 
                                "eigen", "array_type_change", "sym2escsym", "make_vector", NULL};
int primitive_function_arglisti[][6] = {//{OBJ_GEN},                                      // print
                                {OBJ_NONE},                                     // exit
                                {OBJ_SYM},                                      // forget
                                {OBJ_INT,OBJ_LFLT},                             // set_prec
                                {OBJ_INT},                                      // get_prec
                                {OBJ_GEN},                                      // print
                                {OBJ_SYM,OBJ_GEN},                              // printf
                                {OBJ_SYM,OBJ_SYM},                              // open
                                {OBJ_IO},                                       // close
                                {OBJ_IO},                                       // gets
                                {OBJ_SYM,OBJ_IO},                              // puts               
                                {OBJ_IO},                                       // getc
                                {OBJ_NONE},                                     // get_time
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
                                {OBJ_LFLT},                                     // lfsin
                                {OBJ_LFLT},                                     // lfcos
                                {OBJ_LFLT},                                     // lftan
                                {OBJ_LFLT},                                     // lfasin
                                {OBJ_LFLT},                                     // lfacos
                                {OBJ_LFLT},                                     // lfatan
                                {OBJ_LFLT},                                     // lfsinh
                                {OBJ_LFLT},                                     // lfcosh
                                {OBJ_LFLT},                                     // lftanh
                                {OBJ_LFLT},                                     // lfasinh
                                {OBJ_LFLT},                                     // lfacosh
                                {OBJ_LFLT},                                     // lfatanh
                                {OBJ_LFLT},                                     // lflog10
                                {OBJ_LFLT},                                     // lflogE
                                {OBJ_LFLT, OBJ_LFLT},                           // lflog
                                {OBJ_LFLT},                                     // lflog1p
                                {OBJ_LFLT},                                     // lfexp
                                {OBJ_GEN },                                     // abs
                                {OBJ_GEN},                                      // sqrt
                                {OBJ_GEN},                                      // sin
                                {OBJ_GEN},                                      // cos
                                {OBJ_GEN},                                      // tan
                                {OBJ_GEN},                                      // asin
                                {OBJ_GEN},                                      // acos
                                {OBJ_GEN},                                      // atan
                                {OBJ_GEN},                                      // sinh
                                {OBJ_GEN},                                      // cosh
                                {OBJ_GEN},                                      // tanh
                                {OBJ_GEN},                                      // asinh
                                {OBJ_GEN},                                      // acosh
                                {OBJ_GEN},                                      // atanh
                                {OBJ_GEN},                                      // log10
                                {OBJ_GEN},                                      // logE
                                {OBJ_GEN},                                      // log1p
                                {OBJ_GEN, OBJ_GEN},                             // log
                                {OBJ_GEN},                                      // exp 

                                {OBJ_GEN},                                      // floor
                                //{OBJ_GEN}                                     // sqrt
                                {OBJ_INT},                                      // pi
                                {OBJ_INT},                                      // log2
                                {OBJ_FLT},                                      // fgamma
                                {OBJ_FLT},                                      // flgamma
                                {OBJ_GEN},                                      // ogamma
                                {OBJ_GEN},                                      // olgamma
                                {OBJ_GEN},                                      // sum
                                {OBJ_VECT},                                     // vsum
                                {OBJ_INT},                                      // irange
                                {OBJ_VECT,OBJ_INT,OBJ_INT},                     // vswap
                                {OBJ_VECT},                                     // sort
                                {OBJ_GEN, OBJ_GEN},                             // cmp
                                {OBJ_DICT,OBJ_KEY},                             // ddel
                                {OBJ_VECT,OBJ_INT},                             // vdel
                                {OBJ_VECT,OBJ_INT,OBJ_GEN},                     // vins
                                {OBJ_INT},                                      // fact
                                {OBJ_INT},                                      // fib
                                {OBJ_LINT, OBJ_INT},                            // is_prime
                                {OBJ_LINT},                                     // next_prime
                                {OBJ_INT},                                      // init_irand
                                {OBJ_LINT},                                     // init_lint
                                {OBJ_NONE} ,                                    // irand
                                {OBJ_LINT},                                     // lrand
                                {OBJ_LINT},                                     // trial_div
                                {OBJ_LINT,OBJ_LINT,OBJ_LINT,OBJ_INT,OBJ_INT},   // pollard_rho
                                {OBJ_LINT,OBJ_INT,OBJ_INT},                     // pollard_pm1
                                {OBJ_LINT},                                     // fermat
                                //{OBJ_LINT,OBJ_FLT},                             // factor
                                {OBJ_INT},                                      // hexstr
                                {OBJ_INT},                                      // asfloat
                                {OBJ_FLT},                                      // asint
                                {OBJ_INT},                                      // lucas          
                                {OBJ_GEN},                                      // str
                                {OBJ_SYM, OBJ_SYM},                             // str_search
                                {OBJ_GEN},                                      // type
                                {OBJ_GEN},                                      // copy
                                {OBJ_SYM},                                      // system
                                {OBJ_SYM},                                      // popen
#ifndef VMTEST
                                {OBJ_SYM},                                      // compile
                                {OBJ_CNT},                                      // dis_assy
                                {OBJ_CNT},                                      // eval
                                {OBJ_SYM},                                      // load
#endif                                
                                {OBJ_DICT},                                     // keys
                                {OBJ_RAT},{OBJ_RAT},{OBJ_CMPLX},{OBJ_CMPLX},{OBJ_CMPLX},    // num,den,real,imag,arg
                                {OBJ_ARRAY},                                    // a2v
                                {OBJ_VECT},                                     // v2a
                                {OBJ_ARRAY, OBJ_ARRAY},                         // solv_liner
                                {OBJ_ARRAY},                                    // array_inv
                                {OBJ_INT},                                      // make_eye
                                {OBJ_INT, OBJ_INT},                             // make_zero
                                {OBJ_ARRAY},                                    // eigen
                                {OBJ_ARRAY, OBJ_INT},                           // array_type_change
                                {OBJ_SYM},
                                {OBJ_INT}                                       // make_vector
                                };

int primitive_function_ct[][3]  ={//{ return CT, # of parameters, 
                                {OBJ_NONE,0, FALSE},    // exit
                                {OBJ_NONE,1, FALSE},    // forget
                                {OBJ_NONE,2, TRUE},     // set_proc
                                {OBJ_INT, 1, TRUE},     // get_proc
                                {OBJ_NONE,1, TRUE},     // print
                                {OBJ_NONE,2, FALSE},    // printf
                                {OBJ_IO,  2, FALSE},    // open
                                {OBJ_NONE,1, FALSE},    // close
                                {OBJ_SYM, 1, FALSE},    // gets
                                {OBJ_NONE,2, FALSE},    // puts
                                {OBJ_SYM, 1, FALSE},    // getc
                                {OBJ_FLT, 0, FALSE},    // get_time
                                {OBJ_FLT, 1, FALSE},    // sin
                                {OBJ_FLT, 1, FALSE},    // cos
                                {OBJ_FLT, 1, FALSE},    // tan
                                {OBJ_FLT, 1, FALSE},    // asin
                                {OBJ_FLT, 1, FALSE},    // acos
                                {OBJ_FLT, 1, FALSE},    // atan
                                {OBJ_FLT, 1 ,FALSE},    // sinh
                                {OBJ_FLT, 1, FALSE},    // cosh
                                {OBJ_FLT, 1, FALSE},    // tanh
                                {OBJ_FLT, 1, FALSE},    // asinh
                                {OBJ_FLT, 1, FALSE},    // acosh
                                {OBJ_FLT, 1, FALSE},    // atanh
                                {OBJ_FLT, 1, FALSE},    // log10 
                                {OBJ_FLT, 1, FALSE},    // logE
                                {OBJ_FLT, 2, FALSE},    // log
                                {OBJ_FLT, 1, FALSE},    // exp
                                {OBJ_INT, 1, FALSE},    // iabs
                                {OBJ_FLT, 1, FALSE},    // fabs
                                {OBJ_INT, 1, FALSE},    // isqrt
                                {OBJ_FLT, 1, FALSE},    // fsqrt
                                {OBJ_LINT,1, FALSE},    // labs
                                {OBJ_RAT, 1, FALSE},    // rabs
                                {OBJ_LFLT,1, FALSE},    // lfabs
                                {OBJ_FLT,1, FALSE},     // cabs
                                {OBJ_LINT, 1, FALSE},   // lsqrt
                                {OBJ_LFLT, 1, FALSE},   // lfsqrt
                                {OBJ_CMPLX,1, FALSE},   // caqrt
                                {OBJ_LFLT, 1, FALSE},   // lfsin
                                {OBJ_LFLT, 1, FALSE},   // lfcos
                                {OBJ_LFLT, 1, FALSE},   // lftan
                                {OBJ_LFLT, 1, FALSE},   // lfasin
                                {OBJ_LFLT, 1, FALSE},   // lfacos
                                {OBJ_LFLT, 1, FALSE},   // lfatan
                                {OBJ_LFLT, 1, FALSE},   // lfsinh
                                {OBJ_LFLT, 1, FALSE},   // lfcosh
                                {OBJ_LFLT, 1, FALSE},   // lftanh
                                {OBJ_LFLT, 1, FALSE},   // lfasinh
                                {OBJ_LFLT, 1, FALSE},   // lfacosh
                                {OBJ_LFLT, 1, FALSE},   // lfatanh
                                {OBJ_LFLT, 1, FALSE},   // lflo10
                                {OBJ_LFLT, 1, FALSE},   // lflogE
                                {OBJ_LFLT, 2, FALSE},   // lflog
                                {OBJ_LFLT, 1, FALSE},   // lflog1p
                                {OBJ_LFLT, 1, FALSE},   // lfexp
                                {OBJ_GEN,  1, FALSE},   // abs
                                {OBJ_GEN,  1, FALSE},   // sqrt
                                {OBJ_GEN,  1, FALSE},   // sin
                                {OBJ_GEN,  1, FALSE},   // cos
                                {OBJ_GEN,  1, FALSE},   // tan
                                {OBJ_GEN,  1, FALSE},   // asin
                                {OBJ_GEN,  1, FALSE},   // acos
                                {OBJ_GEN,  1, FALSE},   // atan
                                {OBJ_GEN,  1, FALSE},   // sinh
                                {OBJ_GEN,  1, FALSE},   // cosh
                                {OBJ_GEN,  1, FALSE},   // tanh
                                {OBJ_GEN,  1, FALSE},   // asinh
                                {OBJ_GEN,  1, FALSE},   // acosh
                                {OBJ_GEN,  1, FALSE},   // atanh
                                {OBJ_GEN,  1, FALSE},   // log10 
                                {OBJ_GEN,  1, FALSE},   // logE
                                {OBJ_GEN,  1, FALSE},   // log1p
                                {OBJ_GEN,  2, FALSE},   // 1og
                                {OBJ_GEN,  1, FALSE},   // exp

                                {OBJ_GEN,  1, FALSE},   // floor
                                //{OBJ_GEN,  1, FALSE}  // sqrt
                                {OBJ_LFLT, 1, FALSE},   // pi
                                {OBJ_LFLT, 1, FALSE},   // log2
                                {OBJ_FLT,  1, FALSE},   // fgamma 
                                {OBJ_FLT,  1, FALSE},   // flgamma 
                                {OBJ_GEN,  1, FALSE},   // ogamma 
                                {OBJ_GEN,  1, FALSE},   // olgamma
                                {OBJ_GEN,  1, TRUE} ,   // sum
                                {OBJ_GEN,  1, TRUE} ,   // vsum
                                {OBJ_VECT, 1, FALSE},   // irange
                                {OBJ_NONE, 3, FALSE},   // vswap
                                {OBJ_NONE, 1, FALSE},   // sort
                                {OBJ_INT,  2, FALSE},   // cmp
                                {OBJ_NONE, 2, FALSE},   // ddel
                                {OBJ_NONE, 2, FALSE},   // vdel
                                {OBJ_NONE, 3, FALSE},   // vins
                                {OBJ_LINT, 1, FALSE},   // fact
                                {OBJ_LINT, 1, FALSE},   // fib 
                                {OBJ_INT,  2, TRUE},    // is_prime
                                {OBJ_LINT, 1, FALSE} ,  // next_prime
                                {OBJ_NONE, 1, FALSE},   // init_irand
                                {OBJ_NONE, 1, FALSE},   // init_lrand
                                {OBJ_INT,  0, FALSE},   // irand
                                {OBJ_LINT, 1, FALSE},   // lrand
                                {OBJ_VECT, 1, FALSE},   // trial_div
                                {OBJ_LINT, 5, FALSE},   // pollard_rho
                                {OBJ_LINT, 3, FALSE},   // pollard_pm1
                                {OBJ_LINT, 1, FALSE},   // fermat
                                //{OBJ_LINT, 2, FALSE},   // factor
                                {OBJ_SYM,  1, FALSE},   // hex_str
                                {OBJ_FLT,  1, FALSE},   // asfloat
                                {OBJ_INT,  1, FALSE},   // asint
                                {OBJ_INT,  1, FALSE},   // lucas
                                {OBJ_SYM,  1, FALSE},   // str
                                {OBJ_INT,  2, FALSE},   // str_search
                                {OBJ_INT,  1, FALSE},   // type
                                {OBJ_GEN,  1, FALSE},   // copy
                                {OBJ_NONE, 1, FALSE},   // system
                                {OBJ_SYM,  1, FALSE},   // popen
#ifndef VMTEST                                
                                {OBJ_CNT,  1, FALSE},   // compile
                                {OBJ_NONE, 1, FALSE},   // dis_assy
                                {OBJ_GEN,  1, FALSE},   // eval
                                {OBJ_NONE, 1, FALSE},   // load
#endif
                                {OBJ_VECT, 1, FALSE},   // keys
                                {OBJ_LINT,1,FALSE},{OBJ_LINT,1,FALSE},{OBJ_FLT,1,FALSE},{OBJ_FLT,1,FALSE},{OBJ_FLT,1,FALSE},    // num,den,real,imag,arg
                                {OBJ_VECT, 1, FALSE},   // atov
                                {OBJ_ARRAY,1, FALSE},   // vtoa
                                {OBJ_ARRAY,2, FALSE},   // solv_liner
                                {OBJ_ARRAY,1, FALSE},   // array_inv
                                {OBJ_ARRAY,1, FALSE},   // make_eye
                                {OBJ_ARRAY,2, FALSE},   // make_zero
                                {OBJ_VECT, 1, FALSE},   // eigen
                                {OBJ_ARRAY,2, FALSE},   // array_type_change
                                {OBJ_SYM,  1, FALSE},
                                {OBJ_VECT, 1, FALSE}    // make_vector
                                 };

void * make_primitive() {
    int i = 0,j;
    code_type *ct;
    Vector *v,*vv;
    char *s;
    complex *c=(complex*)malloc(sizeof(complex));
    *c=I;
    //mpfr_ptr mp_pai = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init(mp_pai);mpfr_const_pi(mp_pai,MPFR_RNDN);
    //mpfr_ptr mp_log2 = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init(mp_log2);mpfr_const_log2(mp_log2,MPFR_RNDN);
    gmp_randinit_default(RAND_STATE);


    PRIMITIVE_FUNC = Hash_init(256);
    Symbol *char_I=new_symbol("I",1);
    Symbol *char_NONE = new_symbol("None",4);
    Symbol *char_stdin = new_symbol("stdin",5);
    Symbol *char_stdout = new_symbol("stdout",6);
    //Symbol *char_PAI = new_symbol("PAI",3);
    //Symbol *char_LOG2 = new_symbol("LOG2",4);
    Hash_put(G,char_I,(void*)c);
    //Hash_put(G,char_PAI,(void*)mp_pai);
    //Hash_put(G,char_LOG2,(void*)mp_log2);
    //Hash_put(G,char_NONE,(void*)0);
    Hash_put(G,char_stdin,(void*)stdin);
    Hash_put(G,char_stdout,(void*)stdout);
#ifndef VMTEST 
    Hash_put(GLOBAL_VAR,char_I,new_ct(OBJ_CMPLX,OBJ_NONE,(void*)0,FALSE));
    Hash_put(GLOBAL_VAR,char_stdin,new_ct(OBJ_IO,OBJ_NONE,(void*)0,FALSE));
    Hash_put(GLOBAL_VAR,char_stdout,new_ct(OBJ_IO,OBJ_NONE,(void*)0,FALSE));
    //Hash_put(GLOBAL_VAR,char_PAI,new_ct(OBJ_LFLT,OBJ_NONE,(void*)0,FALSE));
    //Hash_put(GLOBAL_VAR,char_LOG2,new_ct(OBJ_LFLT,OBJ_NONE,(void*)0,FALSE));
    //Hash_put(GLOBAL_VAR,char_NONE,new_ct(OBJ_NONE,OBJ_NONE,(void*)0,FALSE));
#endif
    while (primitive_func[i] != NULL) {
        s=primitive_function_name[i];long n=strlen(s);
#ifndef VMTEST        
        v=vector_init(3);
        for (j=0;j<primitive_function_ct[i][1];j++) {
            push(v,(void*)new_ct(primitive_function_arglisti[i][j], NULL, NULL, FALSE));
        }
        ct = new_ct(OBJ_PFUNC, new_ct(primitive_function_ct[i][0], NULL, NULL, FALSE),v,primitive_function_ct[i][2]);
        //vv=vector_init(2);
        //push(vv,(void*)primitive_func[i]);push(vv,(void*)ct);
        Hash_put(PRIMITIVE_FUNC,new_symbol(s,n),(void*)ct);
#endif
        vv=vector_init(3);
        push(vv,(void*)FUNC_PRIM);push(vv,(void*)primitive_func[i]);
        //Hash_put(G,new_symbol(s,n),(void*)primitive_func[i]);
        Hash_put(G,new_symbol(s,n),(void*)vv);
        i++;
    }
#ifndef VMTEST
    Hash_put(GLOBAL_VAR,new_symbol("__PRIMITIVE__",13),new_ct(OBJ_DICT,OBJ_NONE,(void*)0,FALSE));
    Hash_put(G,new_symbol("__PRIMITIVE__", 13),(void*)PRIMITIVE_FUNC);
    Hash_put(GLOBAL_VAR,new_symbol("__GLOBAL__",10),new_ct(OBJ_DICT,OBJ_NONE,(void*)0,FALSE));
#endif
    Hash_put(G,new_symbol("__GLOBAL__", 10),(void*)G);
    //print_hashTable(PRIMITIVE_FUNC);print_hashTable(G);
}
