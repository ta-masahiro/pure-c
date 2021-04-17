#include "object.h"
#include "hash.h"

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

mpf_ptr itolf(long n) {
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F); mpf_set_si(F, n);
    return F;
}

long litoi(mpz_ptr L) {
    return mpz_get_si(L);
}
mpq_ptr litor(mpz_ptr L) {
    mpq_ptr Q = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(Q); mpq_set_z(Q, L);
    return Q;
}

double litof(mpz_ptr L) {
    return mpz_get_d(L);
}

mpf_ptr litolf(mpz_ptr L) {
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F); mpf_set_z(F, L);
    return F;
}

long rtoi(mpq_ptr Q) {
    mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_RAT));
    mpz_tdiv_q(L,mpq_numref(Q),mpq_denref(Q));
    return mpz_get_si(L);
}

mpz_ptr rtoli(mpq_ptr Q) {
    mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_RAT));
    mpz_tdiv_q(L,mpq_numref(Q),mpq_denref(Q));
    return L;
}

double rtof(mpq_ptr Q) {
    return mpq_get_d(Q);
}

mpf_ptr rtolf(mpq_ptr Q) {
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F); mpf_set_q(F, Q);
    return F;
}
long ftoi(double d) {return (long)d;}

mpz_ptr ftoli(double d) {
    mpz_ptr L=(mpz_ptr)malloc(sizeof(mpz_ptr));
    mpz_init_set_d(L, d);
    return L;
}

mpf_ptr ftolf(double f) {
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F); mpf_set_d(F, f);
    return F;
}

long lftoi(mpf_ptr F) {return mpf_get_si(F);}
/*
   mpz_ptr lftol(mpf_ptr F) {
   mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_INT));
   mpf_get_z(L,F);
   return L;
   }
   */
object * newINT(long n) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_INT;
    o -> data.intg = n;
    return o;
}
object * newLINT(mpz_ptr L) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_LINT;
    o ->data.ptr = (void * )L;
    return o;
}
object * newLINT_i(long n) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_LINT;
    o ->data.ptr = (void * )itol(n);
    return o;
}
object * newLINT_s(char * s) {
    object * o = (object * )malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_set_str(L, s, 10);
    o -> type = OBJ_LINT;
    o ->data.ptr = (void * )L;
    return o;
}
object * newRAT(mpq_ptr Q) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_RAT;
    o ->data.ptr = (void * )Q;
    return o;
}

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
object * newLFLT(mpf_ptr F) {
    object * o = (object * )malloc(sizeof(object));
    o -> type = OBJ_LFLT;
    o ->data.ptr = (void * )F;
    return o;
}
object * newLFFT_f(double f) {
    object * o = (object * )malloc(sizeof(object));
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F);
    mpf_set_d(F, f);
    o -> type = OBJ_LFLT;
    o ->data.ptr = (void * )F;
    return o;
}

object *newSTR(Symbol*s) {
    object*o=(object*)malloc(sizeof(object));
    o->type=OBJ_SYM;
    o->data.ptr=s;
    return o;
}

object*newVECT(Vector*v) {
    object*o=(object*)malloc(sizeof(object));
    o->type=OBJ_VECT;
    o->data.ptr=(void*)v;
    return o;
}

object*newOBJ(obj_type t, void* v) {
    switch(t) {
        case OBJ_INT:return newINT((long)v);
        case OBJ_FLT:return newFLT(*(double*)v);
        case OBJ_LINT:return newLINT((mpz_ptr)v);
        case OBJ_RAT:return newRAT((mpq_ptr)v);
        case OBJ_SYM:return newSTR((Symbol*)v);
    }
}

long obj2int(object*o) {
    if (o->type != OBJ_INT) {printf("RuntimeErroe:Must be integer!\n");return 0;}
    return o->data.intg;
}

 mpz_ptr obj2long(object*o) {
    if (o->type != OBJ_LINT) {printf("RuntimeErroe:Must be LongInteger!\n");return NULL;}
    return (mpz_ptr)o->data.ptr;
}

 mpq_ptr obj2rat(object*o) {
    if (o->type != OBJ_RAT) {printf("RuntimeErroe:Must be Rational!\n");return NULL;}
    return (mpq_ptr)o->data.ptr;
}

 double obj2flt(object*o) {
    if (o->type != OBJ_FLT) {printf("RuntimeErroe:Must be Float!\n");return 0;}
    return o->data.flt;
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
    //if (y<0 ? x>LONG_MAX+y :x<LONG_MIN+y) {
    //    mpz_ptr Lx=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Lx,x);   
    //    mpz_ptr Ly=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Ly,y);
    //    mpz_sub(Lx,Lx,Ly);
    //    o->data.ptr=(void*)Lx;o->type=OBJ_LINT;
    //}
    o -> data.intg = x  -  y;
    o -> type = OBJ_INT;
    return o;
}

object * objIMUL(long x, long y) {
    object * o = (object * )malloc(sizeof(object));
    //if (labs(x)>labs(LONG_MAX/y)) {
    //    mpz_ptr Lx=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Lx,x);   
    //    mpz_ptr Ly=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init_set_si(Ly,y);
    //    mpz_mul(Lx,Lx,Ly);
    //    o->data.ptr=(void*)Lx;o->type=OBJ_LINT;
    //}
    o -> data.intg = x * y;
    o -> type = OBJ_INT;
    return o;
}

object * objIDIV(long x, long y) {
    object * o = (object*)malloc(sizeof(object));
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
    if (y>0) mpz_pow_ui(L,L,y);
    else mpz_root(L,L,y);
    if (mpz_cmp_si(L,2147483647)>0 || mpz_cmp_si(L,-2147483648)<0) {o->data.ptr=(object*)L; o->type=OBJ_LINT;}
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

object * objLMUL(mpz_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    mpz_mul(L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLDIV(mpz_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    mpz_tdiv_q (L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLMOD(mpz_ptr x, mpz_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpz_ptr L = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(L);
    mpz_tdiv_r (L, x, y);
    o -> data.ptr = (void * )L;
    o -> type = OBJ_LINT;
    return o;
}

object * objLPOW(mpz_ptr x, long y) {
    object*o=(object*)malloc(sizeof(object));
    mpz_ptr L=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(L);
    if (y>0) mpz_pow_ui(L,x,y);
    else mpz_root(L,x,y);
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
    mpz_ptr n=mpq_numref(x);
    mpz_ptr d=mpq_denref(x);
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
    o -> data.flt = x / y;
    o -> type = OBJ_FLT;
    return o;
}

object * objFMOD(double x,double y){
    object * o = (object*)malloc(sizeof(object));
    long l;
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

object * objLFADD(mpf_ptr x, mpf_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F);
    mpf_add(F, x, y);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object * objLFMUL(mpf_ptr x, mpf_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F);
    mpf_mul(F, x, y);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object * objLFSUB(mpf_ptr x, mpf_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F);
    mpf_sub(F, x, y);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object * objLFDIV(mpf_ptr x, mpf_ptr y) {
    object * o = (object*)malloc(sizeof(object));
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F);
    mpf_div(F, x, y);
    o -> data.ptr = (void * )F;
    o -> type = OBJ_LFLT;
    return o;
}

object*objLFMOD(mpf_ptr x, mpf_ptr y) {
    object * o = (object*)malloc(sizeof(object) );
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F);
    mpf_div(F, x, y);
    mpf_trunc(F,F);
    mpf_mul(F,F,y);
    mpf_sub(F,x,F);
    o->data.ptr=(void*)F;
    o->type=OBJ_LFLT;
    return o;
}

object*objLFPOW_i(mpf_ptr x, long y) {
    object * o = (object*)malloc(sizeof(object) );
    mpf_ptr F = (mpf_ptr)malloc(sizeof(__mpf_struct));
    mpf_init(F);
    mpf_pow_ui(F, x, y);
    o->type=OBJ_LFLT;o->data.ptr=(void*)F;
    return o;
}

int objICMP(long x, long y)         {return (x>y)?1:((x == y)?0: - 1);}
int objLCMP(mpz_ptr x, mpz_ptr y)   {return mpz_cmp(x, y);}
int objRCMP(mpq_ptr x, mpq_ptr y)   {return mpq_cmp(x, y);}
int objFCMP(double x, double y)     {return (x>y)?1:((x == y)?0: - 1);}
int objLFCMP(mpf_ptr x, mpf_ptr y)        {return mpf_cmp(x, y);}

object * objadd(object * x, object * y) {
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIADD(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLADD_i((mpz_ptr)y -> data.ptr, x->data.intg);
                case OBJ_RAT:   return objRADD(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFADD(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFADD(itolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLADD_i((mpz_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLADD((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRADD(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFADD(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFADD(litolf((mpz_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRADD((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRADD((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRADD((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFADD(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFADD(rtolf((mpq_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFADD(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFADD(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFADD(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFADD(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFADD(itolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFADD((mpf_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFADD((mpf_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFADD((mpf_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFADD((mpf_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFADD((mpf_ptr)x -> data.ptr, (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
    }
}

object * objsub(object * x, object * y) {
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objISUB(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLSUB(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRSUB(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFSUB(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFSUB(itolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLSUB_i((mpz_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLSUB((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRSUB(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFSUB(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFSUB(litolf((mpz_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRSUB((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRSUB((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRSUB((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFSUB(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFSUB(rtolf((mpq_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFSUB(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFSUB(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFSUB(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFSUB(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFSUB(ftolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFSUB((mpf_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFSUB((mpf_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFSUB((mpf_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFSUB((mpf_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFSUB((mpf_ptr)x -> data.ptr, (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
    }
}
object * objmul(object * x, object * y) {
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIMUL(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLMUL(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRMUL(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMUL(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFMUL(itolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLMUL((mpz_ptr)x -> data.ptr, itol(y -> data.intg));
                case OBJ_LINT:  return objLMUL((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRMUL(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMUL(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFMUL(litolf((mpz_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRMUL((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRMUL((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRMUL((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMUL(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFMUL(rtolf((mpq_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFMUL(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFMUL(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFMUL(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFMUL(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFMUL(ftolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFMUL((mpf_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFMUL((mpf_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFMUL((mpf_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFMUL((mpf_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFMUL((mpf_ptr)x -> data.ptr, (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
    }
}

object * objdiv(object * x, object * y) {
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIDIV(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLDIV(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRDIV(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFDIV(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFDIV(itolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLDIV((mpz_ptr)x -> data.ptr, itol(y -> data.intg));
                case OBJ_LINT:  return objLDIV((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRDIV(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFDIV(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFDIV(litolf((mpz_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRDIV((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRDIV((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRDIV((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFDIV(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFDIV(rtolf((mpq_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFDIV(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFDIV(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFDIV(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFDIV(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFDIV(ftolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFDIV((mpf_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFDIV((mpf_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFDIV((mpf_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFDIV((mpf_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFDIV((mpf_ptr)x -> data.ptr, (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
    }
}

object * objmod(object * x, object * y) {
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIMOD(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLMOD(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRMOD(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMOD(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFMOD(itolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLMOD((mpz_ptr)x -> data.ptr, itol(y -> data.intg));
                case OBJ_LINT:  return objLMOD((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRMOD(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMOD(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFMOD(litolf((mpz_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRMOD((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRMOD((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRMOD((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFMOD(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFMOD(rtolf((mpq_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFMOD(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFMOD(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFMOD(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFMOD(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFMOD(ftolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFMOD((mpf_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFMOD((mpf_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFMOD((mpf_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFMOD((mpf_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFMOD((mpf_ptr)x -> data.ptr, (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return NULL;
            }
    }
}

object * objpow(object * x, object * y) {
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objIPOW(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLPOW(itol(x -> data.intg), mpz_get_ui((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLPOW(itol(x -> data.intg), rtoi((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFPOW(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFPOW_i(itolf(x -> data.intg), mpf_get_ui((mpf_ptr)y -> data.ptr));
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLPOW((mpz_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLPOW((mpz_ptr)x -> data.ptr, mpz_get_ui((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLPOW((mpz_ptr)x -> data.ptr, rtoi((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFPOW(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFPOW_i(litolf((mpz_ptr)x -> data.ptr), mpf_get_ui((mpf_ptr)y -> data.ptr));
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRPOW((mpq_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objRPOW((mpq_ptr)x -> data.ptr, mpz_get_ui((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRPOW((mpq_ptr)x -> data.ptr, rtoi((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFPOW(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFPOW_i(rtolf((mpq_ptr)x -> data.ptr), mpf_get_ui((mpf_ptr)y -> data.ptr));
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFPOW(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFPOW(x -> data.flt, mpz_get_d((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFPOW(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFMOD(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFPOW_i(ftolf(x -> data.flt), mpf_get_ui((mpf_ptr)y -> data.ptr));
                default:printf("runtime error illegal add op\n");return NULL;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFPOW_i((mpf_ptr)x -> data.ptr, y -> data.intg);
                case OBJ_LINT:  return objLFPOW_i((mpf_ptr)x -> data.ptr, mpz_get_ui((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFPOW_i((mpf_ptr)x -> data.ptr, rtoi((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFPOW_i((mpf_ptr)x -> data.ptr, (long)(y -> data.flt));
                case OBJ_LFLT:  return objLFPOW_i((mpf_ptr)x -> data.ptr, mpf_get_ui((mpf_ptr)y -> data.ptr));
                default:printf("runtime error illegal add op\n");return NULL;
            }
    }
}

object * objor(object * x, object * y) {
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
                default:printf("runtime error illegal bor op\n");return NULL;
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
                default:printf("runtime error illegal add op\n");return NULL;
            }
    }
}
object * objand(object * x, object * y) {
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
                default:printf("runtime error illegal bor op\n");return NULL;
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
                default:printf("runtime error illegal add op\n");return NULL;
            }
    }
}
int objcmp(object * x, object * y) {
    int type_x = x -> type;
    int type_y = y -> type;
    switch(type_x) {
        case OBJ_INT:
            switch(type_y) {
                case OBJ_INT:   return objICMP(x -> data.intg, y -> data.intg);
                case OBJ_LINT:  return objLCMP(itol(x -> data.intg), (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRCMP(itor(x -> data.intg), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFCMP(itof(x -> data.intg), y -> data.flt);
                case OBJ_LFLT:  return objLFCMP(itolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return 0;
            }
        case OBJ_LINT:
            switch(type_y) {
                case OBJ_INT:   return objLCMP((mpz_ptr)x -> data.ptr, itol(y -> data.intg));
                case OBJ_LINT:  return objLCMP((mpz_ptr)x -> data.ptr, (mpz_ptr)y -> data.ptr);
                case OBJ_RAT:   return objRCMP(litor((mpz_ptr)x -> data.ptr), (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFCMP(litof((mpz_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFCMP(litolf((mpz_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return 0;
            }
        case OBJ_RAT:
            switch(type_y) {
                case OBJ_INT:   return objRCMP((mpq_ptr)x -> data.ptr, itor(y -> data.intg));
                case OBJ_LINT:  return objRCMP((mpq_ptr)x -> data.ptr, litor((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objRCMP((mpq_ptr)x -> data.ptr, (mpq_ptr)y -> data.ptr);
                case OBJ_FLT:   return objFCMP(rtof((mpq_ptr)x -> data.ptr), y -> data.flt);
                case OBJ_LFLT:  return objLFCMP(rtolf((mpq_ptr)x -> data.ptr), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return 0;
            }
        case OBJ_FLT:
            switch(type_y) {
                case OBJ_INT:   return objFCMP(x -> data.flt, itof(y -> data.intg));
                case OBJ_LINT:  return objFCMP(x -> data.flt, litof((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objFCMP(x -> data.flt, rtof((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objFCMP(x -> data.flt, y -> data.flt);
                case OBJ_LFLT:  return objLFCMP(ftolf(x -> data.intg), (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return 0;
            }
        case OBJ_LFLT:
            switch(type_y) {
                case OBJ_INT:   return objLFCMP((mpf_ptr)x -> data.ptr, itolf(y -> data.intg));
                case OBJ_LINT:  return objLFCMP((mpf_ptr)x -> data.ptr, litolf((mpz_ptr)y -> data.ptr));
                case OBJ_RAT:   return objLFCMP((mpf_ptr)x -> data.ptr, rtolf((mpq_ptr)y -> data.ptr));
                case OBJ_FLT:   return objLFCMP((mpf_ptr)x -> data.ptr, ftolf(y -> data.flt));
                case OBJ_LFLT:  return objLFCMP((mpf_ptr)x -> data.ptr, (mpf_ptr)y -> data.ptr);
                default:printf("runtime error illegal add op\n");return 0;
            }
    }
}

object * objneg(object *x) {
    mpz_ptr L; mpq_ptr Q; mpf_ptr F;
    object * o = objcpy(x);
    switch(x->type) {
        case OBJ_INT : o->data.intg = - (o->data.intg); return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_neg(L,L); o->data.ptr=(void*)L;return o;
        case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpq_neg(Q, Q); o->data.ptr=(void*)Q;return o;
        case OBJ_FLT : o->data.flt = - (o->data.flt); return o;
        case OBJ_LFLT: F=(mpf_ptr)o->data.ptr; mpf_neg(F,F); o->data.ptr=(void*)F;return o;
        default:printf("runtime error illegal add op\n");return NULL;
    }
}

object * objnot(object *x) {
    mpz_ptr L; mpq_ptr Q; mpf_ptr F;
    object * o = objcpy(x);
    switch(x->type) {
        case OBJ_INT : o->data.intg = ~ (o->data.intg); return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_com(L,L); o->data.ptr=(void*)L;return o;
        default:printf("runtime error illegal add op\n");return NULL;
    }
}

object * objabs(object *x) {
    mpz_ptr L; mpq_ptr Q; mpf_ptr F;
    object * o = objcpy(x);
    switch(x->type) {
        case OBJ_INT : o->data.intg = labs(o->data.intg); return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_abs(L,L); o->data.ptr=(void*)L;return o;
        case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpq_abs(Q, Q); o->data.ptr=(void*)Q;return o;
        case OBJ_FLT : o->data.flt = fabs(o->data.flt); return o;
        case OBJ_LFLT: F=(mpf_ptr)o->data.ptr; mpf_abs(F,F); o->data.ptr=(void*)F;return o;
        default:printf("runtime error illegal add op\n");return NULL;
    }
}

object*objinc(object *x) {
    mpz_ptr L; mpq_ptr Q, QQ; mpf_ptr F;
    object * o = objcpy(x);
    switch(x->type) {
        case OBJ_INT : (o->data.intg)++; return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_add_ui(L,L,1); o->data.ptr=(void*)L;return o;
        case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpq_init(QQ); mpq_set_ui(QQ,1,1);mpq_add(Q, Q, QQ); o->data.ptr=(void*)Q;return o;
        case OBJ_FLT : o->data.flt += 1.0; return o;
        case OBJ_LFLT: F=(mpf_ptr)o->data.ptr; mpf_add_ui(F,F,1); o->data.ptr=(void*)F;return o;
        default:printf("runtime error illegal add op\n");return NULL;
    }
}

object*objdec(object *x) {
    mpz_ptr L; mpq_ptr Q, QQ; mpf_ptr F;
    object * o = objcpy(x);
    switch(x->type) {
        case OBJ_INT : (o->data.intg)--; return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_sub_ui(L,L,1); o->data.ptr=(void*)L;return o;
        case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpq_init(QQ); mpq_set_ui(QQ,1,1);mpq_sub(Q, Q, QQ); o->data.ptr=(void*)Q;return o;
        case OBJ_FLT : o->data.flt -= 1.0; return o;
        case OBJ_LFLT: F=(mpf_ptr)o->data.ptr; mpf_sub_ui(F,F,1); o->data.ptr=(void*)F;return o;
        default:printf("runtime error illegal add op\n");return NULL;
    }
}
object * objsqrt(object *x) {
    mpz_ptr L; mpq_ptr Q; mpf_ptr F;
    object * o = objcpy(x);
    switch(x->type) {
        case OBJ_INT : o->data.intg = (long)sqrt((double)o->data.intg); return o;
        case OBJ_LINT: L=(mpz_ptr)o->data.ptr; mpz_sqrt(L,L); o->data.ptr=(void*)L;return o;
        case OBJ_RAT : Q=(mpq_ptr)o->data.ptr; mpf_init(F);mpf_set_q(F,Q);mpf_sqrt(F, F); o->data.ptr=(void*)F;o->type=OBJ_LFLT;return o; case OBJ_FLT : o->data.flt = sqrt(o->data.flt); return o;
        case OBJ_LFLT: F=(mpf_ptr)o->data.ptr; mpf_sqrt(F,F); o->data.ptr=(void*)F;return o;
        default:printf("runtime error illegal add op\n");return NULL;
    }
}

int objlt(object*x,object*y){
    if (objcmp(x,y)<0) return TRUE;
    return FALSE;
}
int objle(object*x,object*y){
    if (objcmp(x,y)>0) return FALSE;
    return TRUE;
}
int objgt(object*x,object*y){
    if (objcmp(x,y)<0) return TRUE;
    return FALSE;
}
int objge(object*x,object*y){
    if (objcmp(x,y)<0) return FALSE;
    return TRUE;
}
int objeq(object*x,object*y){
    if (objcmp(x,y)==0) return TRUE;
    return FALSE;
}
int objneq(object*x,object*y){
    if (objcmp(x,y)!=0) return TRUE;
    return FALSE;
}

char * objtostr(object * o) {
    char ret[4096]="[ ";
    char *buf = (char*)malloc(1024*sizeof(char));   // オーバーフローの可能性ありあとで見直すこと
    mp_exp_t e;
    int i,n;
    switch(o -> type){
        case OBJ_INT:   sprintf(buf, "%ld", o -> data.intg); return buf;
        case OBJ_LINT:  return mpz_get_str(NULL, 10, (mpz_ptr)o -> data.ptr);
        case OBJ_RAT:   return mpq_get_str(NULL, 10, (mpq_ptr)o -> data.ptr);
        case OBJ_FLT:   sprintf(buf,"%g",o -> data.flt); return buf;
        case OBJ_LFLT:  gmp_snprintf(buf, 1024, "%.Fe",(mpf_ptr)o->data.ptr);return buf;
                        //
        case OBJ_SYM:   return ((Symbol*)(o->data.ptr))->_table;                //
        case OBJ_VECT:
                        n=((Vector*)o->data.ptr)->_sp;
                        strcpy(buf, "[ ");
                        if (n>0) {
                            for(i=0;i<n-1;i++) {
                                strcat(buf,objtostr((object*)vector_ref(((Vector*)o->data.ptr),i)));
                                strcat(buf,", ");
                            }
                            strcat(buf,objtostr((object*)vector_ref(((Vector*)o->data.ptr),n-1)));
                        }
                        strcat(buf," ]");
                        return buf;
        case OBJ_UFUNC: sprintf(buf,"<UserFunction: %lx>",(long)o->data.ptr);return buf;
        default:printf("RntimeError:Illegal print args!\n");
    }
    return NULL;
}

//char*objtostr(object* o) {
//    return objtype2str(o->type,(void*)o->data);
//}
char * objtype2str(obj_type type, void* value) {
    char *buf = (char*)malloc(1024*sizeof(char));   /* オーバーフローの可能性ありあとで見直すこと */
    mp_exp_t e;
    int i,n;
    //if (type != OBJ_NONE && value==NULL) return "NULL";
    switch(type){
        case OBJ_NONE:
        case OBJ_INT:   sprintf(buf, "%ld", (long)value); return buf;
        case OBJ_LINT:  return mpz_get_str(NULL, 10, (mpz_ptr)value);
        case OBJ_RAT:   return mpq_get_str(NULL, 10, (mpq_ptr)value);
        case OBJ_FLT:   sprintf(buf,"%g",*(double*)value); return buf;
        case OBJ_LFLT:  gmp_snprintf(buf, 1024, "%.Fe",(mpf_ptr)value);return buf;
        case OBJ_GEN:   return objtostr((object*)value);
                        //return objtype2str(((object*)value)->type,((object*)value)->data.ptr);
        case OBJ_SYM:   return ((Symbol*)value)->_table;                //
        case OBJ_VECT://printf("vectorsize:%d",((Vector*)value)->_sp);
                        n=((Vector*)value)->_sp;
                        strcpy(buf,"[ ");
                        if (n > 0) {
                            for(i=0;i<n-1;i++) {//printf("##%s\n",objtostr((object*)vector_ref(((Vector*)value),i)));
                                strcat(buf,objtostr((object*)vector_ref(((Vector*)value),i)));
                                strcat(buf,", ");
                            }
                            strcat(buf,objtostr((object*)vector_ref(((Vector*)value),n-1)));
                        }
                        strcat(buf," ]");
                        return buf;
        case OBJ_UFUNC:PR(13); sprintf(buf,"<UserFunction: %lx>",(long)value);return buf;
        default:printf("RntimeError:Illegal print args!\n");
    }
}

Symbol*objtype2symbol(obj_type t,void*value) {
    char*c=objtype2str(t,value);
    return new_symbol(c,strlen(c));  
}

object*symbol2obj(Symbol*s,obj_type t) {

}
void*symbol2objtype(Symbol*s,obj_type t){
    void*w;
    long num;
    switch (t) {
        case OBJ_INT:
            sscanf(s->_table,"%ld",&num);
            return (void*)num;
        case OBJ_LINT:
            mpz_init_set_str((mpz_ptr)w,s->_table,10);
            return w;
        case OBJ_RAT:
            mpq_init((mpq_ptr)w);mpq_set_str((mpq_ptr)w,s->_table,10);mpq_canonicalize((mpq_ptr)w);
            return w;
        case OBJ_FLT:
            sscanf(s->_table,"%lg",(double*)w);
            return w;
    }

}

object * objcpy(object * s) {
    mpz_ptr L; mpq_ptr Q; mpf_ptr F;// Vector * v; Symbol * sym;
    object * t = (object * )malloc(sizeof(object));
    int type = (t -> type = s -> type);
    switch(type) {
        case OBJ_INT:case OBJ_FLT: t -> data = s -> data; return t;
        case OBJ_LINT: L=(mpz_ptr)malloc(sizeof(MP_INT)); mpz_init_set(L, (mpz_ptr)s ->data.ptr); t -> data.ptr = (void * )L; return t;
        case OBJ_RAT: Q=(mpq_ptr)malloc(sizeof(MP_RAT));  mpq_init(Q); mpq_set(Q, (mpq_ptr)s ->data.ptr); t -> data.ptr = (void * )Q; return t;
        case OBJ_LFLT: F=(mpf_ptr)malloc(sizeof(__mpf_struct));
                       mpf_init(F); mpf_set(F, (mpf_ptr)s ->data.ptr); t -> data.ptr = (void * )F; return t;
        case OBJ_VECT:t->data.ptr=(void*)vector_copy0((Vector*)t->data.ptr);t->type=OBJ_VECT;return t;
        case OBJ_SYM: t->data.ptr=(void*)symbol_cpy((Symbol*)t->data.ptr);t->type=OBJ_SYM;return t;
        default:printf("RntimeError:Illegal copy Method!\n");
    }
    return NULL;
}

int objlen(object* o) {
    switch(o->type) {
        case OBJ_VECT:return ((Vector*)o->data.ptr)->_sp;
        case OBJ_SYM :return ((Symbol*)o->data.ptr)->_size;
        default:printf("RntimeError:Illegal length Method!\n");
    }
    return 0;
}
object*objref(object*t,long i) {
    object*o;
    if (t->type==OBJ_VECT) return vector_ref((Vector*)t->data.ptr,i);
    if (t->type==OBJ_SYM)  {o=(object*)malloc(sizeof(object));o->data.ptr=(void*)symbol_ref((Symbol*)t->data.ptr,i);o->type=OBJ_SYM;return o;}
}
void objset(object*t,long i,object*v) {
    object*o;
    if (t->type==OBJ_VECT) vector_set((Vector*)t->data.ptr,i,(void*)v);
    if (t->type==OBJ_SYM)  symbol_set((Symbol*)t->data.ptr,i,(Symbol*)v->data.ptr);
}
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

   mpf_set_default_prec(16);
   mpf_ptr Xf = (mpf_ptr)malloc(sizeof(__mpf_struct));
   mpf_ptr Yf = (mpf_ptr)malloc(sizeof(__mpf_struct));

   mpz_init(Xl);
   if (mpz_set_str(Xl,argv[1],10)) {
   mpq_init(Xq);
   if (mpq_set_str(Xq,argv[1],10)) {
   mpf_init(Xf);
   if (mpf_set_str(Xf,argv[1],10)) {
   printf("ng input of x\n"); return 0;
   } else {
   if (mpf_cmp_d(Xf,DBL_MAX)>0 || mpf_cmp_d(Xf,-DBL_MAX)<0) x=newLFLT(Xf);
   else if ((mpf_cmp_d(Xf,0)>0 && mpf_cmp_d(Xf,DBL_MIN)<0)|| (mpf_cmp_d(Xf,0)<0 && mpf_cmp_d(Xf,-DBL_MIN)>0)) x=newLFLT(Xf);
   else x=newFLT(mpf_get_d(Xf));
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
   mpf_init(Yf);
   if (mpf_set_str(Yf,argv[2],10)) {
   printf("ng input of y\n"); return 0;
   } else {
   if (mpf_cmp_d(Yf,DBL_MAX)>0 || mpf_cmp_d(Yf,-DBL_MAX)<0) y=newLFLT(Yf);
   else if ((mpf_cmp_d(Yf,0)>0 && mpf_cmp_d(Yf,DBL_MIN)<0)|| (mpf_cmp_d(Yf,0)<0 && mpf_cmp_d(Yf,-DBL_MIN)>0)) y=newLFLT(Yf);
   else y=newFLT(mpf_get_d(Yf));
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