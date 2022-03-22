#include "object.h"
#include "hash.h"
//#include "array.h"

void none_error() {
    printf("RuntimeError:'None'type not calculate!\n");
    Throw(3);
}
mpz_ptr itol(long n) {
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init_set_si(L, n);
    return L;
}

mpq_ptr itor(long n) {
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q); mpq_set_si(Q, n, 1);
    return Q;
}

double itof(long n) {
    return (double)n;
}

mpfr_ptr itolf(long n) {
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F); mpfr_set_si(F, n, MPFR_RNDN);
    return F;
}

complex *itoc(long n) {
    complex *c=malloc(sizeof(complex));
    *c=n+0.0*I;
    return c;
}

long litoi(mpz_ptr L) {
    if (L==NULL) none_error();
    return mpz_get_si(L);
}
mpq_ptr litor(mpz_ptr L) {
    if (L==NULL) none_error();
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q); mpq_set_z(Q, L);
    return Q;
}

double litof(mpz_ptr L) {
    if (L==NULL) none_error();
    return mpz_get_d(L);
}

mpfr_ptr litolf(mpz_ptr L) {
    if (L==NULL) none_error();
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F); mpfr_set_z(F, L, MPFR_RNDN);
    return F;
}

complex *litoc(mpz_ptr L) {
    if (L==NULL) none_error();
    complex*c=(complex*)malloc(sizeof(complex));
    *c=mpz_get_d(L)+0.0*I;
    return c;
}

long rtoi(mpq_ptr Q) {
    if (Q==NULL) none_error();
    mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_RAT));
    mpz_tdiv_q(L,mpq_numref(Q),mpq_denref(Q));
    return mpz_get_si(L);
}

mpz_ptr rtoli(mpq_ptr Q) {
    if (Q==NULL) none_error();
    mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_RAT));
    mpz_tdiv_q(L,mpq_numref(Q),mpq_denref(Q));
    return L;
}

double rtof(mpq_ptr Q) {
    if (Q==NULL) none_error();
    return mpq_get_d(Q);
}

mpfr_ptr rtolf(mpq_ptr Q) {
    if (Q==NULL) none_error();
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F); mpfr_set_q(F, Q, MPFR_RNDN);
    return F;
}

complex *rtoc(mpq_ptr Q) {
    if (Q==NULL) none_error();
    complex *c=(complex*)malloc(sizeof(complex));
    *c=mpq_get_d(Q)+0.0*I;
    return c;
}

long ftoi(double d) {return (long)d;}

mpz_ptr ftoli(double d) {
    mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init_set_d(L, d);
    return L;
}

mpq_ptr ftor(double d) {
    mpq_ptr Q=(mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q);
    mpq_set_d(Q,d);
    return Q;
}

mpfr_ptr ftolf(double f) {
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init_set_d(F, f, MPFR_RNDN);
    return F;
}

complex *ftoc(double f) {
    complex *c = (complex*)malloc(sizeof(complex));
    *c = f+0.0*I;
    return c;
}

long lftoi(mpfr_ptr F) {
    if (F==NULL) none_error();
    return mpfr_get_si(F,MPFR_RNDN);
}

mpz_ptr lftol(mpfr_ptr F) {
   mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_INT));
   mpz_init(L);
   mpfr_get_z(L,F,MPFR_RNDN);
   return L;
}
double lftof(mpfr_ptr F) {
    if (F==NULL) none_error();
    return mpfr_get_d(F,MPFR_RNDN);
}

mpq_ptr lftor(mpfr_ptr F) {
   mpq_ptr Q=(mpq_ptr)malloc(sizeof(MP_RAT));
   mpq_init(Q);
   mpfr_get_q(Q,F);
   return Q;
}

complex *lftoc(mpfr_ptr F) {
    if (F==NULL) none_error();
    complex *c = (complex*)malloc(sizeof(complex));
    *c=mpfr_get_d(F,MPFR_RNDN)+0.0*I;
}
//#define newINT(n)   newOBJ(OBJ_INT,  n)
#define newLINT(n)  newOBJ(OBJ_LINT, (void*)n)
#define newRAT(n)   newOBJ(OBJ_RAT,  (void*)n)
//#define newFLT(n)   newOBJ(OBJ_FLT,  n)
#define newLFLT(n)  newOBJ(OBJ_LFLT, (void*)n)
#define newCMPLX(n) newOBJ(OBJ_CMPLX,(void*)n)
#define newSTR(n)   newOBJ(OBJ_SYM,  (void*)n)
#define newVECT(n)  newOBJ(OBJ_VECT, (void*)n)
#define newDICT(n)  newOBJ(OBJ_DICT, (void*)n)

object * newINT(long n) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_INT;
    o -> data.intg = n;
    return o;
}
/*
object * newLINT(mpz_ptr L) {
    if (L==NULL) none_error();
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_LINT;
    o ->data.ptr = (void * )L;
    return o;
}
*/
object * newLINT_i(long n) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_LINT;
    o ->data.ptr = (void * )itol(n);
    return o;
}
object * newLINT_s(char * s) {
    if (s==NULL) none_error();
    object * o = (object * )malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_set_str(L, s, 10);
    o -> type = OBJ_LINT;
    o ->data.ptr = (void * )L;
    return o;
}
/*
object * newRAT(mpq_ptr Q) {
    if (Q==NULL) none_error();
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_RAT;
    o ->data.ptr = (void * )Q;
    return o;
}
*/
object * newRAT_i(long i, long j) {
    object * o = (object * )malloc(sizeof(object));
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q);
    if (j<0) {j =  - j; i =  - i;}
    mpq_set_si(Q, i, j);mpq_canonicalize(Q);
    o -> type = OBJ_RAT;
    o ->data.ptr = (void * )Q;
    return o;
}

object*newFLT(double d) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_FLT;
    o ->data.flt = d;
    return o;
}
/*
object * newLFLT(mpfr_ptr F) {
    if (F==NULL) none_error();
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_LFLT;
    o ->data.ptr = (void * )F;
    return o;
}
*/
object * newLFFT_f(double f) {
    object * o = (object * )malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init_set_d(F, f, MPFR_RNDN);
    o -> type = OBJ_LFLT;
    o ->data.ptr = (void * )F;
    return o;
}
/*
object *newCMPLX(complex*c) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_CMPLX;
    o ->data.ptr = (void * )c;
    return o;

}
object *newSTR(Symbol*s) {
    if (s==NULL) none_error();
    object*o=(object*)malloc(sizeof(object));
    o->type=OBJ_SYM;
    o->data.ptr=s;
    return o;
}

object*newVECT(Vector*v) {
    if (v==NULL) none_error();
    object*o=(object*)malloc(sizeof(object));
    o->type=OBJ_VECT;
    o->data.ptr=(void*)v;
    return o;
}

object*newDICT(Hash * h) {
    if (h==NULL) none_error();
    object*o=(object*)malloc(sizeof(object));
    o->type=OBJ_DICT;
    o->data.ptr=(void*)h;
    return o;
}
*/
/*
object*newOBJ(obj_type t, void* v) {
    long l=(long)v;
    if (v==NULL) none_error();
    switch(t) {
        case OBJ_INT:return newINT((long)v);
        case OBJ_FLT:return newFLT(*(double*)(&l));
        case OBJ_LFLT:return newLFLT((mpfr_ptr)v);
        case OBJ_LINT:return newLINT((mpz_ptr)v);
        case OBJ_RAT:return newRAT((mpq_ptr)v);
        case OBJ_SYM:return newSTR((Symbol*)v);
        case OBJ_CMPLX:return newCMPLX((complex*)v);
        case OBJ_VECT:return newVECT((Vector*)v);
        case OBJ_DICT:return newDICT((Hash*)v);
        case OBJ_GEN:return (object*)v;
        default:printf("RuntimeError:Illegal Object Type!\n");Throw(3);
    }
}
*/
object * newOBJ(obj_type t, void *v) {
    if (t==OBJ_GEN) return (object *) v;
    object * o = (object *)malloc(sizeof(object));
    o->type = t;
    long l = (long)v;
    switch(t) {
        case OBJ_INT:o->data.intg = l;return o;
        case OBJ_FLT:o->data.flt = *(double *)&l;
        //case OBJ_GEN: return (object*)v;
        default: o->data.ptr = v;return o;
    }
}
long obj2int(object*o) {
    if (o==NULL) none_error();
    switch(o->type) {
        case OBJ_INT:  return o->data.intg;
        case OBJ_LINT: return litoi((mpz_ptr)o->data.ptr);
        case OBJ_RAT:  return rtoi((mpq_ptr)o->data.ptr); 
        case OBJ_FLT:  return ftoi(o->data.flt);
        case OBJ_LFLT: return lftoi((mpfr_ptr)o->data.ptr);
        case OBJ_SYM:  return (long)symbol2objtype((Symbol*)(o->data.ptr), OBJ_INT);
    }
    printf("RuntimeErroe:CanotConverToInteger!\n");Throw(3);
}

 mpz_ptr obj2long(object*o) {
    if (o==NULL) none_error();
    switch(o->type) {
        case OBJ_INT:  return itol(o->data.intg);
        case OBJ_LINT: return (mpz_ptr)o->data.ptr;
        case OBJ_RAT:  return rtoli((mpq_ptr)o->data.ptr); 
        case OBJ_FLT:  return ftoli(o->data.flt);
        case OBJ_LFLT: return lftol((mpfr_ptr)o->data.ptr);
        case OBJ_SYM:  return (mpz_ptr)symbol2objtype((Symbol*)(o->data.ptr), OBJ_LINT);
    }
    printf("RuntimeErroe:CanotConverToLONGInteger!\n");Throw(3);
}

 mpq_ptr obj2rat(object*o) {
    if (o==NULL) none_error();
    switch(o->type) {
        case OBJ_INT:  return itor(o->data.intg);
        case OBJ_LINT: return litor((mpz_ptr)o->data.ptr);
        case OBJ_RAT:  return (mpq_ptr)o->data.ptr; 
        case OBJ_FLT:  return ftor(o->data.flt);
        case OBJ_LFLT: return lftor((mpfr_ptr)o->data.ptr);
        case OBJ_SYM:  return (mpq_ptr)symbol2objtype((Symbol*)(o->data.ptr), OBJ_RAT);
    }
    printf("RuntimeErroe:CanotConverToRational!\n");Throw(3);
}

 double obj2flt(object*o) {
    long l;
    if (o==NULL) none_error();
    switch(o->type) {
        case OBJ_INT:  return itof(o->data.intg);
        case OBJ_LINT: return litof((mpz_ptr)o->data.ptr);
        case OBJ_RAT:  return rtof((mpq_ptr)o->data.ptr); 
        case OBJ_FLT:  return o->data.flt;
        case OBJ_LFLT: return lftof((mpfr_ptr)o->data.ptr);
        case OBJ_SYM:  l = (long)symbol2objtype((Symbol*)(o->data.ptr), OBJ_FLT);return *(double*)(&l);
    }
    printf("RuntimeErroe:CanotConverToFloat!\n");Throw(3);
}

 mpfr_ptr obj2lflt(object*o) {
    if (o==NULL) none_error();
    switch(o->type) {
        case OBJ_INT:  return itolf(o->data.intg);
        case OBJ_LINT: return litolf((mpz_ptr)o->data.ptr);
        case OBJ_RAT:  return rtolf((mpq_ptr)o->data.ptr); 
        case OBJ_FLT:  return ftolf(o->data.flt);
        case OBJ_LFLT: return (mpfr_ptr)o->data.ptr;
        case OBJ_SYM:  return (mpfr_ptr)symbol2objtype((Symbol*)(o->data.ptr), OBJ_LFLT);
    }
    printf("RuntimeErroe:CanotConverToLFloat!\n");Throw(3);
 }

complex *obj2c(object *o) {
    if (o==NULL) none_error();
    switch(o->type) {
        case OBJ_INT:  return itoc(o->data.intg);
        case OBJ_LINT: return litoc((mpz_ptr)o->data.ptr);
        case OBJ_RAT:  return rtoc((mpq_ptr)o->data.ptr); 
        case OBJ_FLT:  return ftoc(o->data.flt);
        case OBJ_LFLT: return lftoc((mpfr_ptr)o->data.ptr);
        case OBJ_CMPLX:return (complex*)o->data.ptr;
        case OBJ_SYM:  return (complex*)symbol2objtype((Symbol*)(o->data.ptr), OBJ_CMPLX);
    }
    printf("RuntimeErroe:CanotConverToComplex!\n");Throw(3);
}


object * objIADD(long x, long y) {
    long z;
    object * o = (object * )malloc(sizeof(object));
    if (y > 0 ? x > LONG_MAX - y : x < LONG_MIN - y) { //オーバーフロー
        mpz_ptr Lx=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Lx,x);   
        mpz_ptr Ly=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Ly,y);
        mpz_add(Lx,Lx,Ly);
        o->data.ptr=(void*)Lx;o->type=OBJ_LINT;
        return o;   
    }
    z=x+y;
    o -> data.intg = x + y;
    o -> type = OBJ_INT;
    return o;
}

object * objISUB(long x, long y) {
    object * o = (object * )malloc(sizeof(object));
    if (y<0 ? x>LONG_MAX+y :x<LONG_MIN+y) {
        mpz_ptr Lx=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Lx,x);   
        mpz_ptr Ly=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Ly,y);
        mpz_sub(Lx,Lx,Ly);
        o->data.ptr=(void*)Lx;o->type=OBJ_LINT;
        return o;
    }
    o -> data.intg = x  -  y;
    o -> type = OBJ_INT;
    return o;
}

object * objIMUL(long x, long y) {
    object * o = (object * )malloc(sizeof(object));
    if ((y != 0) && labs(x)>labs(LONG_MAX/y)) {
        mpz_ptr Lx=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Lx,x);   
        mpz_ptr Ly=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Ly,y);
        mpz_mul(Lx,Lx,Ly);
        o->data.ptr=(void*)Lx;o->type=OBJ_LINT;
        return o;
    }
    o -> data.intg = x * y;
    o -> type = OBJ_INT;
    return o;
}

void zero_division_error() {
    printf("RuntimeError:ZeroDivision!\n");Throw(3);
}

object * objIDIV(long x, long y) {
    object * o = (object*)malloc(sizeof(object));
    if (y==0) zero_division_error();
    o -> data.intg = x / y;
    o -> type = OBJ_INT;
    return o;
}

object*objIMOD(long x,long y) {
    object*o=(object*)malloc(sizeof(object));
    o->data.intg=x%y;
    o->type=OBJ_INT;
    return o;
}

object * objIPOW(long x, long y) {
    object*o=(object*)malloc(sizeof(object));
    mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init_set_si(L,x);
    if (y>=0) mpz_pow_ui(L,L,y);
    else mpz_set_ui(L,0);
    if (!mpz_fits_slong_p(L)) {o->data.ptr=(object*)L; o->type=OBJ_LINT;}
    else {o->data.intg=mpz_get_si(L);o->type=OBJ_INT;}
    return o;
}
object * objLADD(mpz_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    mpz_add(L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLADD_i(mpz_ptr x, long y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    if (y > 0) mpz_add_ui(L, x, y);
    else mpz_sub_ui(L, x, - y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLSUB(mpz_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    mpz_sub(L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLSUB_i(mpz_ptr x, long y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    if (y > 0) mpz_sub_ui(L, x, y);
    else mpz_add_ui(L, x, - y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objiSUBL(long x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    if (y > 0) mpz_ui_sub(L, x, y);
    else mpz_add_ui(L, y,-x);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}
object * objLMUL(mpz_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    mpz_mul(L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLMUL_i(mpz_ptr x, long y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    mpz_mul_si(L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}
object * objLDIV(mpz_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    if (mpz_sgn(y) == 0) zero_division_error();
    mpz_tdiv_q (L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLDIV_i(mpz_ptr x, long y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    if (y==0) zero_division_error();
    mpz_tdiv_q_ui (L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}
object * objLMOD(mpz_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    if (mpz_sgn(y) == 0) zero_division_error();
    mpz_tdiv_r (L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLPOW(mpz_ptr x, long y) {
    object*o=(object*)malloc(sizeof(object));
    mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(L);
    if (y>=0) mpz_pow_ui(L,x,y);
    else mpz_set_si(L,0);
    o->data.ptr=(object*)L; o->type=OBJ_LINT;
    return o;
}

object * objRADD(mpq_ptr x, mpq_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q);
    mpq_add(Q, x, y);
    o -> data.ptr = (void * )Q;
    o -> type = OBJ_RAT;
    return o;
}

object * objRSUB(mpq_ptr x, mpq_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q);
    mpq_sub(Q, x, y);
    o -> data.ptr = (void * )Q;
    o -> type = OBJ_RAT;
    return o;
}

object * objRMUL(mpq_ptr x, mpq_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q);
    mpq_mul(Q, x, y);
    o -> data.ptr = (void * )Q;
    o -> type = OBJ_RAT;
    return o;
}

object * objRDIV(mpq_ptr x, mpq_ptr y) {
    object * o = (object*)malloc(sizeof(object) );
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q);
    if (!mpq_sgn(y)) zero_division_error();
    mpq_div(Q, x, y);
    o -> data.ptr = (void * )Q;
    o -> type = OBJ_RAT;
    return o;
}

object*objRMOD(mpq_ptr x, mpq_ptr y) {
    object * o = (object*)malloc(sizeof(object) );
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpq_init(Q);mpz_init(L);
    mpq_div(Q, x, y);
    mpz_tdiv_q(L, mpq_numref(Q),mpq_denref(Q));
    mpq_set_z(Q,L);
    mpq_mul(Q,Q,y);
    mpq_sub(Q,x,Q);
    o->data.ptr=(void*)Q;
    o->type=OBJ_RAT;
    return o;
}

object * objRPOW(mpq_ptr x,long y) {
    object * o = (object*)malloc(sizeof(object) );
    mpq_ptr z =(mpq_ptr)malloc(sizeof(MP_RAT));mpq_init(z);
    mpz_ptr n=mpq_numref(x);mpz_pow_ui(n,n,y);
    mpz_ptr d=mpq_denref(x);mpz_pow_ui(d,d,y);
    mpq_set_num(z,n);mpq_set_den(z,d);
    o->data.ptr=(void*)z;
    o->type=OBJ_RAT;
    return o;
}

object * objFADD(double x, double y) {
    object * o = (object*)malloc(sizeof(object));
    o -> data.flt = x + y;
    o -> type = OBJ_FLT;
    return o;
}

object * objFSUB(double x, double y) {
    object * o = (object*)malloc(sizeof(object));
    o -> data.flt = x - y;
    o -> type = OBJ_FLT;
    return o;
}

object * objFMUL(double x, double y) {
    object * o = (object*)malloc(sizeof(object));
    o -> data.flt = x * y;
    o -> type = OBJ_FLT;
    return o;
}

object * objFDIV(double x,double y) {
    object * o = (object*)malloc(sizeof(object));
    if (y==0.0) zero_division_error();
    o -> data.flt = x / y;
    o -> type = OBJ_FLT;
    return o;
}

object * objFMOD(double x,double y){
    object * o = (object*)malloc(sizeof(object));
    long l;
    if (y==0.0) zero_division_error();
    l=(long)(x/y);
    o->data.flt = x - (double)l*y;
    o->type=OBJ_FLT;
    return o;
}

object * objFPOW(double x,double y){
    object * o = (object*)malloc(sizeof(object));
    o->data.flt = pow(x, y);
    o->type=OBJ_FLT;
    return o;
}

object * objLFADD(mpfr_ptr x, mpfr_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_add(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object *objLFADD_i(mpfr_ptr x, long y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_add_si(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object *objLFADD_L(mpfr_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_add_z(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}
object *objLFADD_R(mpfr_ptr x, mpq_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_add_q(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}
object *objLFADD_f(mpfr_ptr x, double y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_add_d(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object * objLFMUL(mpfr_ptr x, mpfr_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_mul(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object *objLFMUL_i(mpfr_ptr x, long y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_mul_si(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}
object *objLFMUL_L(mpfr_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_mul_z(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}
object *objLFMUL_R(mpfr_ptr x, mpq_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_mul_q(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}
object *objLFMULL_f(mpfr_ptr x, double y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_mul_d(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}
object * objLFSUB(mpfr_ptr x, mpfr_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_sub(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object * objLFDIV(mpfr_ptr x, mpfr_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    if (mpfr_sgn(y)) zero_division_error();
    mpfr_div(F, x, y, MPFR_RNDN);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object*objLFMOD(mpfr_ptr x, mpfr_ptr y) {
    object * o = (object*)malloc(sizeof(object) );
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    if (mpfr_sgn(y)) zero_division_error();
    //mpfr_div(F, x, y);
    //mpfr_trunc(F,F);
    //mpfr_mul(F,F,y);
    //mpfr_sub(F,x,F);
    mpfr_fmod(F, x, y, MPFR_RNDN);
    o->data.ptr=(void*)F;
    o->type=OBJ_LFLT;
    return o;
}

object*objLFPOW_i(mpfr_ptr x, long y) {
    object * o = (object*)malloc(sizeof(object) );
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_pow_ui(F, x, y, MPFR_RNDN);
    o->type=OBJ_LFLT;o->data.ptr=(void*)F;
    return o;
}

object*objLFPOW(mpfr_ptr x, mpfr_ptr y) {
    object * o = (object*)malloc(sizeof(object) );
    mpfr_ptr F = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(F);
    mpfr_pow(F, x, y, MPFR_RNDN);
    o->type=OBJ_LFLT;o->data.ptr=(void*)F;
    return o;
}

object *objCADD(complex *x, complex *y) {
    object * o = (object*)malloc(sizeof(object) );
    complex * c=(complex*)malloc(sizeof(complex));
    *c=(*x)+(*y);
    o->type=OBJ_CMPLX;o->data.ptr=(void*)c;
    return o;
}
object *objCSUB(complex *x, complex *y) {
    object * o = (object*)malloc(sizeof(object) );
    complex * c=(complex*)malloc(sizeof(complex));
    *c=(*x)-(*y);
    o->type=OBJ_CMPLX;o->data.ptr=(void*)c;
    return o;
}
object *objCMUL(complex *x, complex *y) {
    object * o = (object*)malloc(sizeof(object) );
    complex * c=(complex*)malloc(sizeof(complex));
    *c=(*x)*(*y);
    o->type=OBJ_CMPLX;o->data.ptr=(void*)c;
    return o;
}
object *objCDIV(complex *x, complex *y) {
    object * o = (object*)malloc(sizeof(object) );
    complex * c=(complex*)malloc(sizeof(complex));
    if (*y == 0) zero_division_error();
    *c=(*x)/(*y);
    o->type=OBJ_CMPLX;o->data.ptr=(void*)c;
    return o;
}
object *objCPOW(complex *x, complex *y) {
    object * o = (object*)malloc(sizeof(object) );
    complex * c=(complex*)malloc(sizeof(complex));
    *c=cpow((*x),(*y));
    o->type=OBJ_CMPLX;o->data.ptr=(void*)c;
    return o;
}

int objICMP(long x, long y)         {return (x>y)?1:((x == y)?0: - 1);}
int objLCMP(mpz_ptr x, mpz_ptr y)   {return mpz_cmp(x, y);}
int objRCMP(mpq_ptr x, mpq_ptr y)   {return mpq_cmp(x, y);}
int objFCMP(double x, double y)     {return (x>y)?1:((x == y)?0: - 1);}
int objLFCMP(mpfr_ptr x, mpfr_ptr y)        {return mpfr_cmp(x, y);}

object * objadd(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIADD(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLADD_i((mpz_ptr)y -> data.ptr, x->data.intg);
                case OBJ_RAT:   return objRADD(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFADD(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  //return objLFADD(itolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                                return objLFADD_i((mpfr_ptr)y->data.ptr,x->data.intg);
                case OBJ_CMPLX: return objCADD(itoc(x->data.intg), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLADD_i((mpz_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLADD((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRADD(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFADD(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  //return objLFADD(litolf((mpz_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                                return objLFADD_L((mpfr_ptr)y->data.ptr, (mpz_ptr)x->data.ptr);
                case OBJ_CMPLX: return objCADD(litoc((mpz_ptr)x->data.ptr), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRADD((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRADD((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRADD((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFADD(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  //return objLFADD(rtolf((mpq_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                                return objLFADD_R((mpfr_ptr)y->data.ptr, (mpq_ptr)x->data.ptr);
                case OBJ_CMPLX: return objCADD(rtoc((mpq_ptr)x->data.ptr), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFADD(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFADD(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFADD(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFADD(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  //return objLFADD(ftolf(x -> data.flt), (mpfr_ptr)y -> data.ptr);
                                return objLFADD_f((mpfr_ptr)y->data.ptr, x->data.flt);
                case OBJ_CMPLX: return objCADD(ftoc(x->data.flt), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFADD_i((mpfr_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLFADD_L((mpfr_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objLFADD_R((mpfr_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objLFADD_f((mpfr_ptr)x -> data.ptr, y -> data.flt);
                case OBJ_LFLT:  return objLFADD((mpfr_ptr)x -> data.ptr, (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCADD(lftoc((mpfr_ptr)x->data.ptr), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_CMPLX:
            switch(type_y) {
                case OBJ_INT:   return objCADD((complex*)x -> data.ptr, itoc(y -> data.intg));
                case OBJ_LINT:  return objCADD((complex*)x -> data.ptr, litoc((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objCADD((complex*)x -> data.ptr, rtoc((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objCADD((complex*)x -> data.ptr, ftoc(y -> data.flt));
                case OBJ_LFLT:  return objCADD((complex*)x -> data.ptr, lftoc((mpfr_ptr)y -> data.ptr));
                case OBJ_CMPLX: return objCADD((complex*)x -> data.ptr, (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_VECT:
            if (type_y == OBJ_VECT) {
                return newVECT(vector_append((Vector*)x->data.ptr,(Vector*)y->data.ptr));
            }
            break;
        case OBJ_SYM:
            if (type_y == OBJ_SYM) {
                return newSTR(symbol_append((Symbol*)x->data.ptr,(Symbol*)y->data.ptr));
            }
        default:printf("runtime error illegal add op\n");Throw(3);
    }
}

object * objsub(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objISUB(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objiSUBL(x -> data.intg, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRSUB(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFSUB(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFSUB(itolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCSUB(itoc(x->data.intg), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLSUB_i((mpz_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLSUB((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRSUB(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFSUB(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFSUB(litolf((mpz_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCSUB(litoc((mpz_ptr)x->data.ptr), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRSUB((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRSUB((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRSUB((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFSUB(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFSUB(rtolf((mpq_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCSUB(rtoc((mpq_ptr)x->data.ptr), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFSUB(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFSUB(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFSUB(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFSUB(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFSUB(ftolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCSUB(ftoc(x->data.flt), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFSUB((mpfr_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFSUB((mpfr_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFSUB((mpfr_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFSUB((mpfr_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFSUB((mpfr_ptr)x -> data.ptr, (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCSUB(lftoc((mpfr_ptr)x->data.ptr), (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_CMPLX:
            switch(type_y) {
                case OBJ_INT:   return objCSUB((complex*)x -> data.ptr, itoc(y -> data.intg));
                case OBJ_LINT:  return objCSUB((complex*)x -> data.ptr, litoc((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objCSUB((complex*)x -> data.ptr, rtoc((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objCSUB((complex*)x -> data.ptr, ftoc(y -> data.flt));
                case OBJ_LFLT:  return objCSUB((complex*)x -> data.ptr, lftoc((mpfr_ptr)y -> data.ptr));
                case OBJ_CMPLX: return objCSUB((complex*)x -> data.ptr, (complex*)x->data.ptr);
                default:break;
            }
        default:printf("runtime error illegal add op\n");Throw(3);
    }
}
object * objmul(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    long i,n;
    Vector*v,*v1; Symbol* s,*s1;

    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIMUL(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLMUL(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRMUL(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMUL(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFMUL(itolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCMUL(itoc(x->data.intg), (complex*)x->data.ptr);
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLMUL((mpz_ptr)x -> data.ptr, itol(y -> data.intg));
                case OBJ_LINT:  return objLMUL((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRMUL(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMUL(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFMUL(litolf((mpz_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCMUL(litoc((mpz_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRMUL((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRMUL((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRMUL((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMUL(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFMUL(rtolf((mpq_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCMUL(rtoc((mpq_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFMUL(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFMUL(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFMUL(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFMUL(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFMUL(ftolf(x -> data.flt), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCMUL(ftoc(x->data.flt), (complex*)x->data.ptr);
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFMUL((mpfr_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFMUL((mpfr_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFMUL((mpfr_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFMUL((mpfr_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFMUL((mpfr_ptr)x -> data.ptr, (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCMUL(lftoc((mpfr_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_CMPLX:
            switch(type_y) {
                case OBJ_INT:   return objCMUL((complex*)x -> data.ptr, itoc(y -> data.intg));
                case OBJ_LINT:  return objCMUL((complex*)x -> data.ptr, litoc((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objCMUL((complex*)x -> data.ptr, rtoc((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objCMUL((complex*)x -> data.ptr, ftoc(y -> data.flt));
                case OBJ_LFLT:  return objCMUL((complex*)x -> data.ptr, lftoc((mpfr_ptr)y -> data.ptr));
                case OBJ_CMPLX: return objCMUL((complex*)x -> data.ptr, (complex*)x->data.ptr);
                default:break;
            }
        case OBJ_VECT:
            switch (type_y) {
                case OBJ_INT:  n=y->data.intg;break;
                case OBJ_LINT: n=litoi((mpz_ptr)y->data.ptr);break;
                case OBJ_RAT:  n=rtoi((mpq_ptr)y->data.ptr);break;
                case OBJ_FLT:  n=ftoi((double)y->data.flt);break;
                case OBJ_LFLT: n=lftoi((mpfr_ptr)y->data.ptr);break;
                default:printf("runtime error illegal mul op\n");Throw(3);
            }
            v1=(Vector*)x->data.ptr;
            v=vector_copy0(v1);
            for(i=1;i<n;i++) v=vector_append(v,v1);// memcpyで書き直すこと
            return newVECT(v); 
        case OBJ_SYM:
            switch (type_y) {
                case OBJ_INT:  n=y->data.intg;break;
                case OBJ_LINT: n=litoi((mpz_ptr)y->data.ptr);break;
                case OBJ_RAT:  n=rtoi((mpq_ptr)y->data.ptr);break;
                case OBJ_FLT:  n=ftoi((double)y->data.flt);break;
                case OBJ_LFLT: n=lftoi((mpfr_ptr)y->data.ptr);break;
                default:printf("Runtime error illegal mul op\n");Throw(3);
            }
            s1=(Symbol*)x->data.ptr;
            s=symbol_cpy(s1);
            for(i=1;i<n;i++) s=symbol_append(s,s1);// memcpyで書き直すこと
            return newSTR(s); 
        default:printf("Runtime Error Illegal mul op\n");Throw(3);
    }
}

object * objdiv(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIDIV(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLDIV(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRDIV(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFDIV(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFDIV(itolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCDIV(itoc(x->data.intg), (complex*)x->data.ptr);
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLDIV((mpz_ptr)x -> data.ptr, itol(y -> data.intg));
                case OBJ_LINT:  return objLDIV((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRDIV(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFDIV(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFDIV(litolf((mpz_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCDIV(litoc((mpz_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRDIV((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRDIV((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRDIV((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFDIV(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFDIV(rtolf((mpq_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCDIV(rtoc((mpq_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFDIV(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFDIV(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFDIV(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFDIV(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFDIV(ftolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCDIV(ftoc(x->data.flt), (complex*)x->data.ptr);
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFDIV((mpfr_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFDIV((mpfr_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFDIV((mpfr_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFDIV((mpfr_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFDIV((mpfr_ptr)x -> data.ptr, (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCDIV(lftoc((mpfr_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_CMPLX:
            switch(type_y) {
                case OBJ_INT:   return objCDIV((complex*)x -> data.ptr, itoc(y -> data.intg));
                case OBJ_LINT:  return objCDIV((complex*)x -> data.ptr, litoc((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objCDIV((complex*)x -> data.ptr, rtoc((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objCDIV((complex*)x -> data.ptr, ftoc(y -> data.flt));
                case OBJ_LFLT:  return objCDIV((complex*)x -> data.ptr, lftoc((mpfr_ptr)y -> data.ptr));
                case OBJ_CMPLX: return objCDIV((complex*)x -> data.ptr, (complex*)x->data.ptr);
                default:break;
            }
        default:printf("runtime error illegal add op\n");Throw(3);
    }
}

object * objmod(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIMOD(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLMOD(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRMOD(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMOD(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFMOD(itolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLMOD((mpz_ptr)x -> data.ptr, itol(y -> data.intg));
                case OBJ_LINT:  return objLMOD((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRMOD(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMOD(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFMOD(litolf((mpz_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRMOD((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRMOD((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRMOD((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMOD(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFMOD(rtolf((mpq_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFMOD(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFMOD(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFMOD(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFMOD(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFMOD(ftolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFMOD((mpfr_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFMOD((mpfr_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFMOD((mpfr_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFMOD((mpfr_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFMOD((mpfr_ptr)x -> data.ptr, (mpfr_ptr)y -> data.ptr);
            }
        default:printf("runtime error illegal add op\n");Throw(3);
    }
}

object * objpow(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIPOW(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLPOW(itol(x -> data.intg), mpz_get_ui((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLPOW(itol(x -> data.intg), rtoi((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFPOW(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFPOW(itolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCPOW(itoc(x->data.intg), (complex*)x->data.ptr);
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLPOW((mpz_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLPOW((mpz_ptr)x -> data.ptr, mpz_get_ui((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLPOW((mpz_ptr)x -> data.ptr, rtoi((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFPOW(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFPOW(litolf((mpz_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCPOW(litoc((mpz_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRPOW((mpq_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objRPOW((mpq_ptr)x -> data.ptr, mpz_get_ui((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRPOW((mpq_ptr)x -> data.ptr, rtoi((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFPOW(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFPOW(rtolf((mpq_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCPOW(rtoc((mpq_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFPOW(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFPOW(x -> data.flt, mpz_get_d((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFPOW(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFPOW(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFPOW(ftolf(x -> data.flt), (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCPOW(ftoc(x->data.flt), (complex*)x->data.ptr);
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFPOW_i((mpfr_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLFPOW_i((mpfr_ptr)x -> data.ptr, mpz_get_ui((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFPOW_i((mpfr_ptr)x -> data.ptr, rtoi((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFPOW((mpfr_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFPOW((mpfr_ptr)x -> data.ptr, (mpfr_ptr)y -> data.ptr);
                case OBJ_CMPLX: return objCPOW(lftoc((mpfr_ptr)x->data.ptr), (complex*)x->data.ptr);
            }
        case OBJ_CMPLX:
            switch(type_y) {
                case OBJ_INT:   return objCPOW((complex*)x -> data.ptr, itoc(y -> data.intg));
                case OBJ_LINT:  return objCPOW((complex*)x -> data.ptr, litoc((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objCPOW((complex*)x -> data.ptr, rtoc((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objCPOW((complex*)x -> data.ptr, ftoc(y -> data.flt));
                case OBJ_LFLT:  return objCPOW((complex*)x -> data.ptr, lftoc((mpfr_ptr)y -> data.ptr));
                case OBJ_CMPLX: return objCPOW((complex*)x -> data.ptr, (complex*)x->data.ptr);
                default:break;
            }
        default:printf("runtime error illegal POW op\n");Throw(3);
    }
}

object * objor(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    object* o = (object*)malloc(sizeof(object));
    mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   o->data.intg = (x -> data.intg | y -> data.intg);o->type=OBJ_INT;return o;
                case OBJ_LINT:  //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init_set_si(z,x->data.intg);
                                mpz_ior(z,z,(mpz_ptr)y->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                  
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init_set_si(z,y->data.intg);
                                mpz_ior(z,z,(mpz_ptr)x->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                                                    
                case OBJ_LINT:  //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init(z);
                                mpz_ior(z,(mpz_ptr)x->data.ptr,(mpz_ptr)y->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                                 
            }
        default:printf("runtime error illegal add op\n");Throw(3);
    }
}
object * objand(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    object* o = (object*)malloc(sizeof(object));
    mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   o->data.intg = (x -> data.intg & y -> data.intg);o->type=OBJ_INT;return o;
                case OBJ_LINT:  //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init_set_si(z,x->data.intg);
                                mpz_and(z,z,(mpz_ptr)y->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                  
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init_set_si(z,y->data.intg);
                                mpz_and(z,z,(mpz_ptr)x->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                                                    
                case OBJ_LINT:  //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init(z);
                                mpz_and(z,(mpz_ptr)x->data.ptr,(mpz_ptr)y->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                                 
            }
        default:printf("runtime error illegal bor op\n");Throw(3);
    }
}
object * objxor(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    object* o = (object*)malloc(sizeof(object));
    mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   o->data.intg = (x -> data.intg ^ y -> data.intg);o->type=OBJ_INT;return o;
                case OBJ_LINT:  //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init_set_si(z,x->data.intg);
                                mpz_xor(z,z,(mpz_ptr)y->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                  
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init_set_si(z,y->data.intg);
                                mpz_xor(z,z,(mpz_ptr)x->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                                                    
                case OBJ_LINT:  //mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
                                mpz_init(z);
                                mpz_xor(z,(mpz_ptr)x->data.ptr,(mpz_ptr)y->data.ptr); 
                                o->data.ptr=(void*)z;o->type=OBJ_LINT;
                                return o;                                 
            }
        default:printf("runtime error illegal add op\n");Throw(3);
    }
}

object*objsr(object*x,object*y) {
    if (x==NULL || y==NULL) none_error();
    mpz_ptr lz;
    switch(x->type) {
        case OBJ_INT:
            switch(y->type) {
                case OBJ_INT: return newINT(x->data.intg >> y->data.intg);
                case OBJ_LINT:return newINT(x->data.intg >> mpz_get_si((mpz_ptr)y->data.ptr));
                case OBJ_RAT: return newINT(x->data.intg >> rtoi((mpq_ptr)y->data.ptr));
                case OBJ_FLT: return newINT(x->data.intg >> ftoi(y->data.flt));
                case OBJ_LFLT:return newINT(x->data.intg >> lftoi((mpfr_ptr)y->data.ptr));
                }
        case OBJ_LINT:
            lz=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(lz);
            switch(y->type) {
                case OBJ_INT:
                    mpz_cdiv_q_2exp(lz,(mpz_ptr)x->data.ptr,y->data.intg);
                    return newLINT(lz);
                case OBJ_LINT:
                    mpz_cdiv_q_2exp(lz,(mpz_ptr)x->data.ptr,mpz_get_si((mpz_ptr)y->data.ptr));
                    return newLINT(lz);
                case OBJ_RAT:
                    mpz_cdiv_q_2exp(lz,(mpz_ptr)x->data.ptr,rtoi((mpq_ptr)y->data.ptr));
                    return newLINT(lz);
                case OBJ_FLT:
                    mpz_cdiv_q_2exp(lz,(mpz_ptr)x->data.ptr,ftoi(y->data.flt));
                    return newLINT(lz);
                case OBJ_LFLT:
                    mpz_cdiv_q_2exp(lz,(mpz_ptr)x->data.ptr,lftoi((mpfr_ptr)y->data.ptr));
                    return newLINT(lz);
                }
        default:printf("runtime error illegal sr op\n");Throw(3);
    }
}

object*objsl(object*x,object*y) {
    if (x==NULL || y==NULL) none_error();
    mpz_ptr lz;
    switch(x->type) {
        case OBJ_INT:
            switch(y->type) {
                case OBJ_INT: return newINT(x->data.intg << y->data.intg);
                case OBJ_LINT:return newINT(x->data.intg << mpz_get_si((mpz_ptr)y->data.ptr));
                case OBJ_RAT: return newINT(x->data.intg << rtoi((mpq_ptr)y->data.ptr));
                case OBJ_FLT: return newINT(x->data.intg << ftoi(y->data.flt));
                case OBJ_LFLT:return newINT(x->data.intg << lftoi((mpfr_ptr)y->data.ptr));
            }                
        case OBJ_LINT:
            lz=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(lz);
            switch(y->type) {
                case OBJ_INT:
                    mpz_mul_2exp(lz,(mpz_ptr)x->data.ptr,y->data.intg);
                    return newLINT(lz);
                case OBJ_LINT:
                    mpz_mul_2exp(lz,(mpz_ptr)x->data.ptr,mpz_get_si((mpz_ptr)y->data.ptr));
                    return newLINT(lz);
                case OBJ_RAT:
                    mpz_mul_2exp(lz,(mpz_ptr)x->data.ptr,rtoi((mpq_ptr)y->data.ptr));
                    return newLINT(lz);
                case OBJ_FLT:
                    mpz_mul_2exp(lz,(mpz_ptr)x->data.ptr,ftoi(y->data.flt));
                    return newLINT(lz);
                case OBJ_LFLT:
                    mpz_mul_2exp(lz,(mpz_ptr)x->data.ptr,lftoi((mpfr_ptr)y->data.ptr));
                    return newLINT(lz);
            }
        default:printf("runtime error illegal sr op\n");Throw(3);
    }
}

int objcmp(object * x, object * y) {
    if (x==NULL || y==NULL) none_error();
    int type_x = x -> type;
    int type_y = y -> type;
    Vector*v1,*v2; Symbol* s1,*s2;
    int i,n;

    if ((long)(void*)x == (long)(void*)y) return 0;
    //if (x->type != y->type) return -1;
    if (x->type == y->type && x->data.ptr == y->data.ptr) return 0;

    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objICMP(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLCMP(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRCMP(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFCMP(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFCMP(itolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                default:return -2;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLCMP((mpz_ptr)x -> data.ptr, itol(y -> data.intg));
                case OBJ_LINT:  return objLCMP((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRCMP(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFCMP(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFCMP(litolf((mpz_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                default:return -2;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRCMP((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRCMP((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRCMP((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFCMP(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFCMP(rtolf((mpq_ptr)x -> data.ptr), (mpfr_ptr)y -> data.ptr);
                default:return -2;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFCMP(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFCMP(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFCMP(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFCMP(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFCMP(ftolf(x -> data.intg), (mpfr_ptr)y -> data.ptr);
                default:return -2;
            }
        case OBJ_CMPLX:
            if (type_y==OBJ_CMPLX && *((complex *)x->data.ptr) == *((complex *)y->data.ptr)) return 0;
            return -2;
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFCMP((mpfr_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFCMP((mpfr_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFCMP((mpfr_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFCMP((mpfr_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFCMP((mpfr_ptr)x -> data.ptr, (mpfr_ptr)y -> data.ptr);
                default:return -2;
            }
        //case OBJ_CMPLX:
        //    if (type_y == OBJ_CMPLX) {
        //        if ((complex*)x->data.ptr)
        //    }
        case OBJ_VECT:
            if (type_y == OBJ_VECT) {
                if ((n=((Vector*)(x->data.ptr))->_sp) != ((Vector*)(x->data.ptr))->_sp) return -2;
                v1=(Vector*)x->data.ptr; v2=(Vector*)y->data.ptr;
                if ((long)(void*)v1 == (long)(void*)v2) return 0 ;
                for(i=0;i<n;i++) {
                    if (objcmp(vector_ref(v1,i),vector_ref(v2,i)) != 0) return -2;
                }
                return 0;
            }
            return -2;
        case OBJ_SYM:
            if (type_y == OBJ_SYM) {
                if ((n=((Symbol*)(x->data.ptr))->_size) != ((Symbol*)(x->data.ptr))->_size) return -2;
                s1=(Symbol*)x->data.ptr; s2=(Symbol*)y->data.ptr;
                if ((long)(void*)s1 == (long)(void*)s2) return 0 ;
                return strcmp(s1->_table,s2->_table);
            }
            return -2;
    }
}

object * objneg(object *x) {
    if (x==NULL) none_error();
    mpz_ptr L; mpq_ptr Q; mpfr_ptr F;
    object * o = objcpy(x);complex *c;
    switch(x->type) {
        case OBJ_INT : o->data.intg = - (o->data.intg); return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_neg(L,L); o->data.ptr=(void*)L;return o;
        case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpq_neg(Q, Q); o->data.ptr=(void*)Q;return o;
        case OBJ_FLT : o->data.flt = - (o->data.flt); return o;
        case OBJ_LFLT: F=(mpfr_ptr)o->data.ptr; mpfr_neg(F,F,MPFR_RNDN); o->data.ptr=(void*)F;return o;
        case OBJ_CMPLX:c=(complex*)o->data.ptr;*c=-(*c);return o;
        default:printf("runtime error illegal add op\n");return NULL;
    }
}

object * objnot(object *x) {
    if (x==NULL) none_error();
    mpz_ptr L; mpq_ptr Q; mpfr_ptr F;
    object * o = objcpy(x);
    switch(x->type) {
        case OBJ_INT : o->data.intg = ~ (o->data.intg); return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_com(L,L); o->data.ptr=(void*)L;return o;
        default:printf("runtime error illegal NOT op\n");return NULL;
    }
}

object * objabs(object *x) {
    if (x==NULL) none_error();
    //mpz_ptr L; mpq_ptr Q; mpfr_ptr F;
    object * o = objcpy(x);//complex*c;
    switch(x->type) {
        case OBJ_INT : o->data.intg = labs(o->data.intg); return o;
        //case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_abs(L,L); o->data.ptr=(void*)L;return o;
        case OBJ_LINT: mpz_abs((mpz_ptr)o->data.ptr, (mpz_ptr)x->data.ptr); return o;
        case OBJ_RAT : mpq_abs((mpq_ptr)o->data.ptr, (mpq_ptr)x->data.ptr); return o;
        case OBJ_FLT : o->data.flt = fabs(o->data.flt); return o;
        case OBJ_LFLT: mpfr_abs((mpfr_ptr)o->data.ptr, (mpfr_ptr)x->data.ptr, MPFR_RNDN); return o;
        //case OBJ_CMPLX:*(complex*)o->data.ptr = cabs(*(complex*)x->data.ptr);return o;
        case OBJ_CMPLX:o->data.flt = cabs(*(complex*)x->data.ptr);o->type=OBJ_FLT;return o;
        default:printf("runtime error illegal ABS op\n");return NULL;
    }
}

object * objfloor(object *x) {
    if (x==NULL) none_error();
    object * o = objcpy(x);;
    void * v;
    switch(x->type) {
        case OBJ_INT :  case OBJ_LINT: return o;
        case OBJ_RAT : v = malloc(sizeof(MP_INT)); mpz_init((mpz_ptr)v);mpz_fdiv_q((mpz_ptr)v, mpq_numref((mpq_ptr)x->data.ptr), mpq_denref((mpq_ptr)x->data.ptr)); mpq_set_z((mpq_ptr)o->data.ptr,(mpz_ptr)v); return o;
        case OBJ_FLT : o->data.flt = floor(o->data.flt); return o;
        case OBJ_LFLT: v = malloc(sizeof(MP_INT)); mpz_init((mpz_ptr)v);mpfr_get_z((mpz_ptr)v,(mpfr_ptr)x->data.ptr,MPFR_RNDD); mpfr_set_z((mpfr_ptr)o->data.ptr, (mpz_ptr)v,MPFR_RNDD); return o;
        //case OBJ_CMPLX:c=(complex*)o->data.ptr;*c=cabs((*c));return o;
        default:printf("runtime error illegal FLOOR op\n");return NULL;
    }
}

object*objinc(object *x) {
    if (x==NULL) none_error();
    mpz_ptr L; mpq_ptr Q, QQ; mpfr_ptr F;
    object * o = objcpy(x);complex *c;
    switch(x->type) {
        case OBJ_INT : (o->data.intg)++; return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_add_ui(L,L,1); o->data.ptr=(void*)L;return o;
        case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpq_init(QQ); mpq_set_ui(QQ,1,1);mpq_add(Q, Q, QQ); o->data.ptr=(void*)Q;return o;
        case OBJ_FLT : o->data.flt += 1.0; return o;
        case OBJ_LFLT: F=(mpfr_ptr)o->data.ptr; mpfr_add_ui(F,F,1,MPFR_RNDN); o->data.ptr=(void*)F;return o;
        case OBJ_CMPLX:c=(complex*)o->data.ptr;*c=(*c)+1.0;return o;
        default:printf("runtime error illegal INC op\n");return NULL;
    }
}

object*objdec(object *x) {
    if (x==NULL) none_error();
    mpz_ptr L; mpq_ptr Q, QQ; mpfr_ptr F;
    object * o = objcpy(x);complex *c;
    switch(x->type) {
        case OBJ_INT : (o->data.intg)--; return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_sub_ui(L,L,1); o->data.ptr=(void*)L;return o;
        case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpq_init(QQ); mpq_set_ui(QQ,1,1);mpq_sub(Q, Q, QQ); o->data.ptr=(void*)Q;return o;
        case OBJ_FLT : o->data.flt -= 1.0; return o;
        case OBJ_LFLT: F=(mpfr_ptr)o->data.ptr; mpfr_sub_ui(F,F,1,MPFR_RNDN); o->data.ptr=(void*)F;return o;
        case OBJ_CMPLX:c=(complex*)o->data.ptr;*c=(*c)-1.0;return o;
        default:printf("runtime error illegal DEC op\n");return NULL;
    }
}
object * objsqrt(object *x) {
    if (x==NULL) none_error();
    //mpz_ptr L; mpq_ptr Q; mpfr_ptr F;
    void * v;
    object * o = objcpy(x);
    switch(x->type) {
        case OBJ_INT : o->data.intg = (long)sqrt((double)o->data.intg); return o;
        case OBJ_LINT: mpz_sqrt((mpz_ptr)o->data.ptr, (mpz_ptr)x->data.ptr); return o;
        //case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpfr_init_set_q(F,Q,MPFR_RNDN);mpfr_sqrt(F, F,MPFR_RNDN); o->data.ptr=(void*)F;o->type=OBJ_LFLT;return o; // BUG!!!!
        case OBJ_RAT : v = malloc(sizeof(__mpfr_struct)); mpfr_init_set_q((mpfr_ptr)v, (mpq_ptr)x->data.ptr, MPFR_RNDN); mpfr_sqrt((mpfr_ptr)v, (mpfr_ptr)v, MPFR_RNDN); o->data.ptr = v; o->type=OBJ_LFLT; return o; 
        case OBJ_FLT : o->data.flt = sqrt(o->data.flt); return o;
        case OBJ_LFLT: mpfr_sqrt((mpfr_ptr)o->data.ptr, (mpfr_ptr)x->data.ptr, MPFR_RNDN); return o;
        case OBJ_CMPLX:*(complex*)o->data.ptr = csqrt(*(complex*)x->data.ptr); return o;
        default:printf("runtime error illegal SQRT op\n");return NULL;
    }
}

int objlt(object*x,object*y){
    //int c;
    if (objcmp(x,y)==-1) return TRUE;
    //if (c==-2) {}
    return FALSE;
}
int objle(object*x,object*y){
    int c;
    if ((c=objcmp(x,y))==-1 || c==0 ) return TRUE;
    return FALSE;
}
int objgt(object*x,object*y){
    if (objcmp(x,y)>0) return TRUE;
    return FALSE;
}
int objge(object*x,object*y){
    if (objcmp(x,y)>=0) return TRUE;
    return FALSE;
}
int objeq(object*x,object*y){
    if (objcmp(x,y)==0) return TRUE;
    return FALSE;
}
int objneq(object*x,object*y){
    if (objcmp(x,y)!=0) return TRUE;
    return FALSE;
}

char*objtostr(object* o) {
    if (o==NULL) return "None";
    switch(o->type) {
        case OBJ_INT:return objtype2str(OBJ_INT,(void*)(o->data.intg));
        case OBJ_FLT:return objtype2str(OBJ_FLT,(void*)*(long*)(&(o->data.flt)));
        default:     return objtype2str(o->type,(void*)(o->data.ptr));
    }
}

Symbol *obj2symbol(object* o) {
    //if (o==NULL) return "None";
    switch(o->type) {
        case OBJ_INT:return objtype2symbol(OBJ_INT,(void*)(o->data.intg));
        case OBJ_FLT:return objtype2symbol(OBJ_FLT,(void*)*(long*)(&(o->data.flt)));
        default:     return objtype2symbol(o->type,(void*)(o->data.ptr));
    }
}

char * set_lf_format(mpfr_ptr x) {
    //char buff[256];
    char *form = (char*)malloc(256*sizeof(char));
    //form ="%.";
    int i=mpfr_get_prec(x)*log10(2.0)+1.0;
    //sprintf(buff,"%d",i);
    //strcat(form,buff);
    //strcat(form,"Rg");
    sprintf(form,"%s%d%s","%.",i,"Rg");
    return form;
}
#define NONE ""
//#define NONE "None"
char * objtype2str(obj_type type, void* value) {
    int new_size,buf_size=1024;
    char *str,*buf = (char*)malloc(buf_size*sizeof(char)); 
    mp_exp_t e;
    int i,n;
    long lval;
    unsigned long li;
    Symbol * key;
    if ((type != OBJ_INT && type != OBJ_FLT) && value == NULL) return NONE;
    switch(type){
        case OBJ_NONE:  return NONE;
        case OBJ_INT:   sprintf(buf, "%ld", (long)value); return buf;
        case OBJ_LINT:  return mpz_get_str(NULL, 10, (mpz_ptr)value);
        case OBJ_RAT:   return mpq_get_str(NULL, 10, (mpq_ptr)value);
        //case OBJ_FLT:   sprintf(buf,"%.16g",*(double*)value); return buf;
        //case OBJ_FLT:   lval=(long)value;sprintf(buf,"%.17g",*(double*)(&lval)); return buf;
        case OBJ_FLT:   lval=(long)value;sprintf(buf,"%.16g",*(double*)(&lval)); return buf;
        case OBJ_CMPLX: sprintf(buf,"%.16g%+.16gI",creal(*(complex*)value),cimag(*(complex*)value)); return buf;
        case OBJ_LFLT:  //mpfr_sprintf(buf,"%.Rg", (mpfr_ptr)value);return buf;
                        mpfr_sprintf(buf,set_lf_format((mpfr_ptr)value),(mpfr_ptr)value);return buf;
        case OBJ_GEN:   return objtostr((object*)value);
                        //return objtype2str(((object*)value)->type,((object*)value)->data.ptr);
        case OBJ_SYM:   return ((Symbol*)value)->_table;                //
        case OBJ_VECT://printf("vectorsize:%d",((Vector*)value)->_sp);
                        n=((Vector*)value)->_sp;
                        strcpy(buf,"[");
                        if (n > 0) {
                            //結構無駄なことをしている気がする
                            for(i=0;i<n-1;i++) {//printf("##%s\n",objtostr((object*)vector_ref(((Vector*)value),i)));
                                str=objtostr((object*)vector_ref(((Vector*)value),i));
                                if ((new_size=strlen(buf)+strlen(str)+1)>=buf_size) {buf=(char*)realloc(buf,new_size*2);buf_size=new_size*2;}
                                strcat(buf,str);
                                strcat(buf,", ");
                            }
                            str=objtostr((object*)vector_ref(((Vector*)value),n-1));
                            if ((new_size=strlen(buf)+strlen(str)+1)>=buf_size) {buf=(char*)realloc(buf,new_size*2);buf_size=new_size*2;}
                            strcat(buf,str);
                        }
                        strcat(buf,"]");
                        return buf;
        case OBJ_UFUNC: sprintf(buf,"<UserFunction: %lx>",(long)value);return buf;
        case OBJ_PFUNC: sprintf(buf,"<PrimitiveFunction: %lx>",(long)value);return buf;
        case OBJ_CNT:   sprintf(buf,"<UserCode: %lx>",(long)value);return buf;
        case OBJ_IO   : sprintf(buf,"<I/O_file: %lx>",(long)value);return buf;
        case OBJ_DICT : //sprintf(buf,"<Dictionary: %lx>",(long)value);return buf;
                        strcpy(buf, "{ ");
                        for(li = 0;  li < (((Hash*)value)->size); li ++ ) {
                            key = ((Hash*)value)->hashTable[li].key;
                            if (key != NULL) { 
                                //printf("i:%ld key:%s hash:%ld, val:%ld\n",i , key ->_table, hash(key->_table,key->_size, h->initval) & (h->size -1), (long)(h ->hashTable[i].val));  
                                strcat(buf,key->_table);strcat(buf," :");strcat(buf,objtostr((object*)(((Hash*)value) ->hashTable[li].val)));
                                strcat(buf,", ");
                            }
                        }
                        strcat(buf,"}");
                        return buf;
        default:printf("RntimeError:Illegal print args!\n");Throw(3);
    }
}

Symbol*objtype2symbol(obj_type t,void*value) {
    char*c=objtype2str(t,value);
    return new_symbol(c,strlen(c));  
}

object*symbol2obj(Symbol*s,obj_type t) {
    return newOBJ(t,symbol2objtype(s,t));
}

mpq_ptr decsym2rat(Symbol * S) {
    char * s = (char *)malloc((S->_size +1)*sizeof(char));
    strcpy(s, S->_table);
    int minus = FALSE, p_minus = FALSE;
    char c, *intp, *decp, *powp;
    mpz_ptr i_part = (mpz_ptr)malloc(sizeof(MP_INT)), d_part=(mpz_ptr)malloc(sizeof(MP_INT)), powz=(mpz_ptr)malloc(sizeof(MP_INT)), work=(mpz_ptr)malloc(sizeof(MP_INT));
    mpq_ptr r = (mpq_ptr)malloc(sizeof(MP_RAT)); 
    long p_part;int phase = 0;
    mpz_t z10;
    mpz_init_set_ui(z10, 10);
    //
    mpq_init(r);
    if (*s == '-') {
        s++; minus = TRUE;
    }
    intp = s;
    while ((c=*s) != '\0') {
        if isdigit(c) {
            s++;
        } else if (phase == 0 && c=='.') {
            *(s++) = '\0';
            decp =s;
            phase =1;
            if (decp == intp) mpz_init_set_ui(i_part, 0);
            else mpz_init_set_str(i_part,intp,10);
        } else if (phase == 1 && (c == 'e' || c == 'E' || c == 'f' || c == 'F')) {
            *s = '\0';
            if (*(++s) == '-')  {p_minus = TRUE; s++;}
            powp = s;
            phase =2;
            if (powp == decp) mpz_init_set_ui(d_part, 0);
            else mpz_init_set_str(d_part,decp,10);
        } else {
            mpq_set_ui(r,0,1);
            return r;
        }
    }
    if (phase == 2) {p_part = strtol(powp,NULL,10);}
    else if (phase == 1) {mpz_init_set_str(d_part,decp,10); p_part = 0;}
    else if (phase == 0) {mpz_init_set_str(i_part,intp,10); mpz_init_set_ui(d_part,0);p_part = 0;}
    // return minus*(i_part+d_part/(10^(decp-intp)))*10^p_part
    //printf("A.B 10^C: A=%s B=%s C=%ld\n",mpz_get_str(NULL,10,i_part), mpz_get_str(NULL,10,d_part), p_part);
    mpz_init(powz); mpz_pow_ui(powz, z10, p_part);mpz_mul_si(powz, powz, minus ?-1:1);
    mpz_init(work); mpz_pow_ui(work, z10, strlen(decp));
    mpz_mul(i_part, i_part, work);mpz_add(i_part, i_part, d_part);
    if (p_minus) mpz_mul(work, work, powz);
    else mpz_mul(i_part, i_part, powz);
    mpq_set_num(r, i_part); mpq_set_den(r, work); mpq_canonicalize(r);
    return r;
}

void*symbol2objtype(Symbol*s,obj_type t){
    if (s==NULL) none_error();
    int mflg=FALSE;
    void*w;
    double d,q;
    char *endp1, *endp2;
    complex c, *cp;
    //complex c;

    switch (t) {
        case OBJ_INT:
            //w=malloc(sizeof(long));
            //sscanf(s->_table,"%ld",(long*)w);
            //return w;
            return (void*)strtol(s->_table,NULL,10);
        case OBJ_LINT:
            w=malloc(sizeof(MP_INT));
            mpz_init_set_str((mpz_ptr)w,s->_table,10);
            return w;
        case OBJ_RAT:// 許容されるのは "整数/整数" "浮動小数点表記"のいずれか
            w=malloc(sizeof(MP_RAT));
            mpq_init((mpq_ptr)w);
            if (mpq_set_str((mpq_ptr)w,s->_table,10) == 0) {
                if (mpz_sgn(mpq_denref((mpq_ptr)w)) == 0) {printf("RuntimeError:IllegalRationalNumber!\n");Throw(3);}
                mpq_canonicalize((mpq_ptr)w);return w;} // "整数/整数"表記を変換
            else return (void*)decsym2rat(s);   // 浮動小数表記を分数に変換
        case OBJ_FLT:
            //w=malloc(sizeof(double));
            //sscanf(s->_table,"%lg",(double*)w);
            d=strtod(s->_table,NULL);
            //return w;
            return (void*)*(long*)(&d);
        case OBJ_LFLT:
            w = malloc(sizeof(__mpfr_struct));
            mpfr_init_set_str((mpfr_ptr)w,s->_table,10,MPFR_RNDN);
            return w;
        case OBJ_CMPLX:
            cp = (complex*)malloc(sizeof(complex));
            //w = (void*)malloc(sizeof(complex));
            d =strtod(s->_table,&endp1);
            if (*endp1 != '\0') {
                if (*endp1 == 'i' || *endp1 == 'I') {   
                    *cp = d*I;
                    return (void*)cp;
                } 
                q=strtod(endp1,&endp2);
                if (*endp2 =='i' || *endp2 == 'I') {
                    *cp     = d+q*I;
                    //*(complex*)w     = d+q*I;
                    //return w;
                    return (void*)cp;
                } 
            }
            *cp = d+0*I;
            //*(complex*)w = d+0*I;
            return (void*)cp;
            //return w;
        default:printf("RntimeError:CanotConvert Symbol to Complex! %s\n",s->_table);Throw(3);
    }

}

object * objcpy(object * s) {
    if (s==NULL) none_error();
    mpz_ptr L; mpq_ptr Q; mpfr_ptr F;complex *c;// Vector * v; Symbol * sym;
    object * t = (object * )malloc(sizeof(object));
    int type = (t -> type = s -> type);
    switch(type) {
        case OBJ_INT:case OBJ_FLT: t -> data = s -> data; return t;
        case OBJ_LINT: L=(mpz_ptr)malloc(sizeof(MP_INT)); mpz_init_set(L, (mpz_ptr)s ->data.ptr); t -> data.ptr = (void * )L; return t;
        case OBJ_RAT: Q=(mpq_ptr)malloc(sizeof(MP_RAT));  mpq_init(Q); mpq_set(Q, (mpq_ptr)s ->data.ptr); t -> data.ptr = (void * )Q; return t;
        case OBJ_LFLT: F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));
                       mpfr_init_set(F, (mpfr_ptr)s ->data.ptr,MPFR_RNDN); t -> data.ptr = (void * )F; return t;
        case OBJ_CMPLX: c=(complex *)malloc(sizeof(complex));*c=*(complex *)s->data.ptr;t->data.ptr=c;t->type=OBJ_CMPLX;return t;
        case OBJ_VECT:t->data.ptr=(void*)vector_copy0((Vector*)s->data.ptr);t->type=OBJ_VECT;return t;
        case OBJ_SYM: t->data.ptr=(void*)symbol_cpy((Symbol*)t->data.ptr);t->type=OBJ_SYM;return t;
        default:printf("RntimeError:Illegal copy Method!\n");Throw(3);
    }
    return NULL;
}

long objlen(object* o) {
    if (o==NULL) none_error();
    switch(o->type) {
        case OBJ_VECT:return ((Vector*)o->data.ptr)->_sp;
        case OBJ_SYM :return ((Symbol*)o->data.ptr)->_size;
        default:printf("RntimeError:Illegal length Method!%d\n",o->type);Throw(3);
    }
    return 0;
}
object*objref(object*t,long i) {
    if (t==NULL) none_error();
    object*o;
    if (t->type==OBJ_VECT) return vector_ref((Vector*)t->data.ptr,i);
    if (t->type==OBJ_SYM)  {o=(object*)malloc(sizeof(object));o->data.ptr=(void*)symbol_ref((Symbol*)t->data.ptr,i);o->type=OBJ_SYM;return o;}
    printf("RntimeError:Illegal ref Method!\n");Throw(3);
}
void objset(object*t,long i,object*v) {
    //printf("type:%d\n",t->type);
    if (t==NULL) none_error();
    if (t->type==OBJ_VECT) {vector_set((Vector*)t->data.ptr,i,(void*)v);return;}
    if (t->type==OBJ_SYM)  {symbol_set((Symbol*)t->data.ptr,i,(Symbol*)v->data.ptr);return;}
    if (t->type==OBJ_GEN)  {printf("!!!!!!objset!!!\n");objset((object*)t->data.ptr,i,v);return;}
    printf("RntimeError:Illegal set Method!\n");Throw(3);
}

object* objpop(object*t) {
    if (t==NULL) none_error();
    object*o=(object*)malloc(sizeof(object));
    if (t->type==OBJ_VECT) return (object*)pop((Vector*)t->data.ptr);
    if (t->type==OBJ_SYM) {
        o->data.ptr=(void*)symbol_pop((Symbol*)t->data.ptr);
        o->type=OBJ_SYM;
        return o;
    }
    printf("RntimeError:Illegal pop Method!\n");
    Throw(3);
}

void objpush(object *o,object* value) {
    if (o==NULL) none_error();
    if (o->type == OBJ_VECT) push((Vector*)o->data.ptr,(void*)value);
    else {printf("RntimeError:Illegal push Method!\n");Throw(3);}
}

object * objslice(object* o,long start,long end) {
    long sp;
    if (o==NULL) none_error();
    Vector * l,*ll;Symbol * sym;
    if (o->type == OBJ_VECT) {
        sp=((Vector*)(o->data.ptr))->_sp;
        if (start<0) start=sp+end;
        if (end<=0) end=sp+end;
        if (end>sp) end=sp;
        if (end<start) start=end;
        l=(Vector*)malloc(sizeof(Vector));
        ll=(Vector*)o->data.ptr;
        l->_table=&ll->_table[start];
        l->_cp=0;
        l->_sp=(end-start);
        l->_size=ll->_size-start;
        return newVECT(l);
    } else if (o->type == OBJ_SYM) {
        sym=symbol_cpy_n((Symbol*)o->data.ptr,start,end-start);
        return newSTR(sym);
    }else {
        printf("RntimeError:Illegal push Method!\n");
        Throw(3);
    }
}
object *objlogE(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(log(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_log(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_log(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(log(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_log(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=clog(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objsin(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(sin(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_sin(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_sin(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(sin(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_sin(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=csin(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objcos(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(cos(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_cos(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_cos(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(cos(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_cos(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=ccos(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objtan(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(tan(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_tan(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_tan(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(tan(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_tan(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=ctan(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objasin(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(asin(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_asin(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_asin(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(asin(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_asin(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=casin(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objacos(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(acos(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_acos(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_acos(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(acos(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_acos(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=cacos(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objatan(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(atan(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_atan(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_atan(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(atan(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_atan(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=catan(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objsinh(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(sinh(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_sinh(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_sinh(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(sinh(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_sinh(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=csinh(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objcosh(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(cosh(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_cosh(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_cosh(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(cosh(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_cosh(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=ccosh(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objtanh(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(tanh(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_tanh(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_tanh(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(tanh(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_tanh(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=ctanh(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objasinh(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(asinh(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_asinh(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_asinh(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(asinh(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_asinh(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=casinh(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objacosh(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(acosh(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_acosh(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_acosh(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(acosh(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_acosh(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=cacosh(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}
object *objatanh(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(atanh(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_atanh(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_atanh(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(atanh(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_atanh(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        case OBJ_CMPLX:*c=catanh(*(complex*)x->data.ptr);return newCMPLX(c);
    }
}

object *objgamma(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(tgamma(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_gamma(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_gamma(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(tgamma(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_gamma(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        //case OBJ_CMPLX:*c=catanh(*(complex*)x->data.ptr);return newCMPLX(c);
        default:printf("RntimeError:Illegal argument!\n");Throw(3);
    }
}
object *objlgamma(object *x) {
    mpfr_ptr F=(mpfr_ptr)malloc(sizeof(__mpfr_struct));complex *c=(complex*)malloc(sizeof(complex));
    switch(x->type) {
        case OBJ_INT:  return newFLT(lgamma(itof(x->data.intg)));
        case OBJ_LINT: mpfr_init(F);mpfr_lngamma(F,litolf((mpz_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_RAT:  mpfr_init(F);mpfr_lngamma(F,rtolf((mpq_ptr)x->data.ptr),MPFR_RNDN);return newLFLT(F) ;
        case OBJ_FLT:  return newFLT(lgamma(x->data.flt));
        case OBJ_LFLT: mpfr_init(F);mpfr_lngamma(F,(mpfr_ptr)x->data.ptr,MPFR_RNDN);return newLFLT(F) ;
        //case OBJ_CMPLX:*c=catanh(*(complex*)x->data.ptr);return newCMPLX(c);
        default:printf("RntimeError:Illegal argument!\n");Throw(3);
    }
}

Symbol * objtype2hashkeySymbol(obj_type t, void *o) {
    void ** v = (void **)malloc(sizeof(void*));
    switch(t) {
        case OBJ_INT: case OBJ_FLT:
            v[0] = (void*)o;
            return new_symbol((char*)v, 8);
        case OBJ_LINT:
        default:printf("RntimeError:Illegal argument!\n");Throw(3);
    }
}
/*
void get_vector_dims(Vector * v, int *dim, int * sizes, int *total_size) {
    object * o;
    int s = v->_cp;
    if ((o = (object *)vector_ref(v,0))->type == OBJ_VECT) ;
}
void vector2array2(Vector * v, void ** a, int *ind) {
    object *data;
    for(int i=0;i<v->_sp;i++) {
        if ((data = (object*)vector_ref(v,i))->type == OBJ_VECT) {
            vector2array2((Vector *)data->data.ptr, a, ind);
        }else {
            a[(*ind)++] = data;
        }
    }
}

array * vector2array(Vector *v) {
    object * data = (object*)vector_ref(v,0);
    Vector * vv;
    int i=0, size[10];
    size[i++]=v->_sp;
    while (data->type == OBJ_VECT) {
       size[i++]=(vv = (Vector *)(data->data.ptr))->_sp;
       data = (object *)vector_ref(vv, 0);
    }
    int * ss = (int *)malloc(i*sizeof(int));
    for(int j=0;j<=i;j++) ss[j] = size[i-j-1];
    array * r = array_init(data->type, i, ss);
    //
    int ind;
    vector2array2(v, r->table, &ind);
    return r;
}

Vector * array2vector(array *a) {

}
*/
/*
   void * _realloc(void * ptr, size_t old_size, size_t new_size) {
   return GC_realloc(ptr, new_size);
   }

   int main(int argc, char * argv[]) {
   mp_set_memory_functions((void *)GC_malloc, (void * )_realloc, (void * ) GC_free);
   object *x, *y, *z;

   mpz_ptr Xl = (mpz_ptr)malloc(sizeof(MP_INT));
   mpz_ptr Yl = (mpz_ptr)malloc(sizeof(MP_INT));
   mpq_ptr Xq = (mpq_ptr)malloc(sizeof(MP_RAT));
   mpq_ptr Yq = (mpq_ptr)malloc(sizeof(MP_RAT));

   mpfr_set_default_prec(16);
   mpfr_ptr Xf = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
   mpfr_ptr Yf = (mpfr_ptr)malloc(sizeof(__mpfr_struct));

   mpz_init(Xl);
   if (mpz_set_str(Xl,argv[1],10)) {
   mpq_init(Xq);
   if (mpq_set_str(Xq,argv[1],10)) {
   mpfr_init(Xf);
   if (mpfr_set_str(Xf,argv[1],10)) {
   printf("ng input of x\n"); return 0;
   } else {
   if (mpfr_cmp_d(Xf,DBL_MAX)>0 || mpfr_cmp_d(Xf,-DBL_MAX)<0) x=newLFLT(Xf);
   else if ((mpfr_cmp_d(Xf,0)>0 && mpfr_cmp_d(Xf,DBL_MIN)<0)|| (mpfr_cmp_d(Xf,0)<0 && mpfr_cmp_d(Xf,-DBL_MIN)>0)) x=newLFLT(Xf);
   else x=newFLT(mpfr_get_d(Xf));
   }
   } else {
  x = newRAT(Xq);
   }
   } else {
   if (mpz_cmp_si(Xl,2147483647)>0 || mpz_cmp_si(Xl,-2147483648)<0) x=newOBJ_LINT(Xl);
   else x=newINT(mpz_get_si(Xl));
   }

   mpz_init(Yl);
   if (mpz_set_str(Yl,argv[2],10)) {
   mpq_init(Yq);
   if (mpq_set_str(Yq,argv[2],10)) {
   mpfr_init(Yf);
   if (mpfr_set_str(Yf,argv[2],10)) {
   printf("ng input of y\n"); return 0;
   } else {
   if (mpfr_cmp_d(Yf,DBL_MAX)>0 || mpfr_cmp_d(Yf,-DBL_MAX)<0) y=newLFLT(Yf);
   else if ((mpfr_cmp_d(Yf,0)>0 && mpfr_cmp_d(Yf,DBL_MIN)<0)|| (mpfr_cmp_d(Yf,0)<0 && mpfr_cmp_d(Yf,-DBL_MIN)>0)) y=newLFLT(Yf);
   else y=newFLT(mpfr_get_d(Yf));
   }
   } else {
   y = newRAT(Yq);
   }
   } else {
   if (mpz_cmp_si(Yl,2147483647)>0 || mpz_cmp_si(Yl,-2147483648)<0) y=newLINT(Yl);
   else y=newINT(mpz_get_si(Yl));
   }

   printf("L:type %d %s\n", x->type, objtostr(x));
   printf("R:type %d %s\n", y->type, objtostr(y));
   printf("ADD:%s\n",objtostr(objadd(x,y)));
   printf("SUB:%s\n",objtostr(objsub(x,y)));
   printf("MUL:%s\n",objtostr(objmul(x,y)));
   printf("DIV:%s\n",objtostr(objdiv(x,y)));
   printf("MOD:%s\n",objtostr(objmod(x,y)));
   printf("NEG x:%s\n",objtostr(objneg(x)));
   printf("NEG y:%s\n",objtostr(objneg(y)));
   printf("ABS x:%s\n",objtostr(objabs(x)));
   printf("ABS y:%s\n",objtostr(objabs(y)));
   printf("SQRT x:%s\n",objtostr(objsqrt(x)));
   printf("SQRT y:%s\n",objtostr(objsqrt(y)));
printf("INC x:%s\n",objtostr(objinc(x)));
printf("INC y:%s\n",objtostr(objinc(y)));
printf("CMP:%d\n",objcmp(x,y));

}
*/
