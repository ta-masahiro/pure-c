#include "vm.h"
//#include "fast_memcpy.h"
//#define memcpy fast_memcpy
//typedef void*(*Funcpointer)(Vector*);
void disassy(Vector * code, int indent, FILE*fp);

char * code_name[] = 
    {"STOP",  "LDC",  "LD",   "ADD",  "CALL", "RTN",  "SEL",  "JOIN", "LDF",  "SET",  "LEQ",  "LDG",  "GSET", "SUB",
     "DEC",   "TCALL","TSEL", "DROP", "EQ",   "INC",  "MUL",  "DIV",  "VEC",  "REF",  "VSET", "HASH", "LDH",  "HSET",
     "VPUSH", "VPOP", "LADD", "LSUB", "LMUL", "ITOL", "LPR",  "PCALL","LDM",  "DUP",  "SWAP", "ROT",  "_2ROT","CALLS",
     "TCALLS","RTNS", "LDP",  "LDL",  "FADD", "FSUB", "FMUL", "FDIV", "FPR",  "ITOF", "LCPY", "OADD", "OSUB", "OMUL",
     "ODIV",  "OEQ",  "OLEQ", "ITOO", "OPR",  "ODEC", "OINC", "IADD", "ISUB", "IMUL", "IDIV", "IEQ",  "ILEQ", "IDEC",
     "IINC",  "LTOO", "FTOO", "IJTOO","SPR",  "LDIV", "OLT",  "LT"  , "ILT",  "GT",   "IGT",  "OGT",  "GEQ",  "IGEQ",
     "OGEQ",  "NEG",  "INEG", "ONEG", "BNOT", "APL",  "TAPL", "FEQ",  "FLEQ", "FGEQ", "FLT",  "FGT",  "LEQ",  "LLEQ",
     "LGEQ",  "LLT",  "LGT",  "RADD", "RSUB", "RMUL", "RDIV", "REQ",  "RLEQ", "RGEQ", "RLT",  "RGT",  "ITOR", "OTOF",
     "LTOR",  "LTOF", "RTOF", "RTOO", "LTOI", "RTOI", "RTOL", "FTOI", "FTOL", "FTOR", "LNEG", "RNEG", "FNEG", "LINC",
     "LDEC",  "NEQ",  "INEQ", "LNEQ", "RNEQ", "FNEQ", "ONEQ", "OTOI", "OTOL", "OTOR", "VTOO", "STOO", "IPOW", "LPOW",
     "RPOW",  "FPOW", "OPOW", "IMOD", "LMOD", "RMOD", "FMOD", "OMOD", "IBOR", "LBOR", "OBOR", "IBNOT","LBNOT","OBNOT",
     "IBAND", "LBAND","OBAND","VLEN", "SLEN", "OLEN", "VAPP", "SAPP", "VREF", "SREF", "OREF", "SSET", "OSET", "STOI",
     "STOL",  "STOR", "STOF", "ITOS", "LTOS", "RTOS", "FTOS", "OTOS", "VTOS", "SPOP", "OPOP", "OPUSH","SMUL", "VMUL",
     "VEQ",   "SEQ",  "ISR",  "ISL",  "LSR",  "LSL",  "OSR",  "OSL",  "OTOV", "VSLS", "SSLS", "OSLS", "LD00", "LD01",
     "LD02",  "LD03", "LD10", "LD11", "LD12", "LD13", "SET00","SET01","SET02","SET03","SET10","SET11","SET12","SET13",
     "LFADD", "LFSUB","LFMUL","LFDIV","LFMOD","LFPOW","LFGT", "LFLT", "LFEQ", "LFNEQ","LFGEQ","LFLEQ","LFNEG","ITOLF",
     "LTOLF", "RTOLF","FTOLF","OTOLF","LFTOI","LFTOL","LFTOR","LFTOF","LFTOO","CADD", "CSUB", "CMUL", "CDIV", "CPOW",
     "CNEG",  "LFTOS","ITOC", "LTOC", "RTOC", "FTOC", "LFTOC","CTOO", "OTOC", "CTOS", "CEQ",  "CNEQ", "WHILE", "LOOP_SET",
     "LOOP",  "STOLF","STOC", "$$$" };

int op_size[] = \
    {   0,    1,     1,    0,    1,    0,   2,   0,    1,   1,   0,    1,    1,    0,    \
        0,    1,     2,    0,    0,    0,   0,   0,    1,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   1,    0,   0,   0,    0,    0,    1,    \
        1,    0,     1,    1,    0,    0,   0,   0,    0,   0,   0 ,   0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0 ,  0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    1,   1 ,  0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0 ,   0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    0,    0,    \
        0,    0,     0,    0,    0,    0,   0,   0,    0,   0,   0,    0,    2,    1,    \
        1,    0,     0,    0     };

Vector *tosqs(Vector*code, const void** table) {
    enum CODE op;
    Vector *C = vector_copy0(code);
    while (code->_cp < code->_sp) {
        op=(enum CODE)dequeue(code); //printf("%d ",op);
        vector_set(C, (C->_cp)++, (void*)table[op]); //printf("%ld\n",(long)table[op]);
        if (op == LDF || op== LDP || op == LOOP) {
            vector_set(C,(C->_cp)++,(void*)tosqs(dequeue(code),table));
        } else if (op == SEL || op == TSEL) {
            vector_set(C,(C->_cp)++,(void*)tosqs(dequeue(code),table));
            vector_set(C,(C->_cp)++,(void*)tosqs(dequeue(code),table));
        } else if (op == WHILE) {
            vector_set(C, (C->_cp)++, dequeue(code));
            vector_set(C, (C->_cp)++, (void*)tosqs(dequeue(code),table));
        } else {
            (code->_cp)+=op_size[op];
            (C->_cp)+=op_size[op];
        }
    }
    C->_cp=0;
    return C;
}


void * eval(Vector * S, Vector * E, Vector * Code, Vector * R, Vector * EE, Hash * G) {
    Symbol * sym,*sym1;
    long inst, ff, i, j, n, p, SSP=S->_sp;
    Vector * fn, * keys, * t_exp, * f_exp, * code, * args, * cl, * ref, * Es, * l, *ll, *lll;
    void ** g, * v;
    Funcpointer func;
    Hash * h;
    mpz_ptr x, y, z, w;
    mpq_ptr qx,qy,qz;
    mpfr_ptr lfx,lfy,lfz;
    complex *cx,*cy,*cz;
    enum CODE op;
    Vector *C = vector_copy0(Code),*ssp=vector_init(200);
    double* fx,*fy,*fz;
    double xx,yy,zz;
    object*o;
    char*ch;
    long LOOP_COUNTER = 0;
    static const void * table[] = {
            &&_STOP,  &&_LDC,  &&_LD,   &&_ADD,  &&_CALL, &&_RTN,  &&_SEL,  &&_JOIN, &&_LDF,  &&_SET,  &&_LEQ,  &&_LDG,  &&_GSET, &&_SUB,  \
            &&_DEC,   &&_TCALL,&&_TSEL, &&_DROP, &&_EQ,   &&_INC,  &&_MUL,  &&_DIV,  &&_VEC,  &&_REF,  &&_VSET, &&_HASH, &&_LDH,  &&_HSET, \
            &&_VPUSH, &&_VPOP, &&_LADD, &&_LSUB, &&_LMUL, &&_ITOL, &&_LPR,  &&_PCALL,&&_LDM,  &&_DUP,  &&_SWAP, &&_ROT,  &&_2ROT, &&_CALLS,\
            &&_TCALLS,&&_RTNS, &&_LDP,  &&_LDL,  &&_FADD, &&_FSUB, &&_FMUL, &&_FDIV ,&&_FPR,  &&_ITOF, &&_LCPY, &&_OADD, &&_OSUB, &&_OMUL, \
            &&_ODIV,  &&_OEQ,  &&_OLEQ, &&_ITOO, &&_OPR , &&_ODEC, &&_OINC, &&_IADD, &&_ISUB, &&_IMUL, &&_IDIV, &&_IEQ,  &&_ILEQ, &&_IDEC, \
            &&_IINC,  &&_LTOO, &&_FTOO, &&_IJTOO,&&_SPR , &&_LDIV, &&_OLT,  &&_LT,   &&_ILT , &&_GT,   &&_IGT,  &&_OGT,  &&_GEQ,  &&_IGEQ, \
            &&_OGEQ,  &&_NEG,  &&_INEG, &&_ONEG, &&_BNOT, &&_APL,  &&_TAPL, &&_FEQ,  &&_FLEQ, &&_FGEQ, &&_FLT,  &&_FGT,  &&_LEQ,  &&_LLEQ, \
            &&_LGEQ,  &&_LLT,  &&_LGT,  &&_RADD, &&_RSUB, &&_RMUL, &&_RDIV, &&_REQ,  &&_RLEQ, &&_RGEQ, &&_RLT,  &&_RGT,  &&_ITOR, &&_OTOF, \
            &&_LTOR,  &&_LTOF, &&_RTOF, &&_RTOO, &&_LTOI, &&_RTOI, &&_RTOL, &&_FTOI, &&_FTOL, &&_FTOR, &&_LNEG, &&_RNEG, &&_FNEG, &&_LINC, \
            &&_LDEC,  &&_NEQ,  &&_INEQ, &&_LNEQ, &&_RNEQ, &&_FNEQ, &&_ONEQ, &&_OTOI, &&_OTOL, &&_OTOR, &&_VTOO, &&_STOO, &&_IPOW, &&_LPOW, \
            &&_RPOW,  &&_FPOW, &&_OPOW, &&_IMOD, &&_LMOD, &&_RMOD, &&_FMOD, &&_OMOD ,&&_IBOR, &&_LBOR, &&_OBOR, &&_IBNOT,&&_LBNOT,&&_OBNOT,\
            &&_IBAND, &&_LBAND,&&_OBAND,&&_VLEN, &&_SLEN, &&_OLEN, &&_VAPP, &&_SAPP, &&_VREF, &&_SREF, &&_OREF, &&_SSET, &&_OSET ,&&_STOI, \
            &&_STOL,  &&_STOR, &&_STOF, &&_ITOS, &&_LTOS, &&_RTOS, &&_FTOS, &&_OTOS, &&_VTOS, &&_SPOP, &&_OPOP, &&_OPUSH,&&_SMUL, &&_VMUL, \
            &&_VEQ,   &&_SEQ,  &&_ISR,  &&_ISL,  &&_LSR,  &&_LSL,  &&_OSR,  &&_OSL,  &&_OTOV, &&_VSLS, &&_SSLS, &&_OSLS ,&&_LD00, &&_LD01, \
            &&_LD02,  &&_LD03, &&_LD10, &&_LD11, &&_LD12, &&_LD13, &&_SET00,&&_SET01,&&_SET02,&&_SET03,&&_SET10,&&_SET11,&&_SET12,&&_SET13,\
            &&_LFADD, &&_LFSUB,&&_LFMUL,&&_LFDIV,&&_LFMOD,&&_LFPOW,&&_LFGT, &&_LFLT, &&_LFEQ, &&_LFNEQ,&&_LFGEQ,&&_LFLEQ,&&_LFNEG,&&_ITOLF,\
            &&_LTOLF, &&_RTOLF,&&_FTOLF,&&_OTOLF,&&_LFTOI,&&_LFTOL,&&_LFTOR,&&_LFTOF,&&_LFTOO,&&_CADD, &&_CSUB, &&_CMUL, &&_CDIV ,&&_CPOW, \
            &&_CNEG,  &&_LFTOS,&&_ITOC, &&_LTOC, &&_RTOC, &&_FTOC, &&_LFTOC,&&_CTOO, &&_OTOC, &&_CTOS, &&_CEQ,  &&_CNEQ, &&_WHILE,&&_LOOP_SET,\
            &&_LOOP,  &&_STOLF,&&_STOC   };
 
    C = tosqs(Code,table);//vector_print(C);
    w = (mpz_ptr)malloc(sizeof(MP_INT)); mpz_init(w);
_STARt:
    C->_cp = 0;
    goto * dequeue(C);
    //goto * table[(int)dequeue(C)];
_STOP:
    return pop(S);
_LDC:
    push(S, dequeue(C));
    goto * dequeue(C);
    //goto * table[(int)dequeue(C)];
_LD:
    ref = (Vector * )dequeue(C);
    i = (long)vector_ref(ref, 0); // printf("i = %d\n", i);
    j = (long)vector_ref(ref, 1); // printf("j = %d\n", j);
    Es = (Vector * )vector_ref(E, E ->_sp - i - 1); // printf("%d\n", E ->_sp); vector_print(Es);
    if ( j >= 0 ) {
        push(S, (void * )vector_ref(Es, j)); // printf("data = %d\n", (int)vector_ref(Es, j));
        goto * dequeue(C);
    }
    Es->_cp =  - j - 1; push(S, (void * )vector_copy(Es));
    goto * dequeue(C);
_LD00:
    push(S,(void*)vector_ref((Vector*)vector_ref(E,E->_sp-1),0));
    goto * dequeue(C);
_LD01:
    push(S,(void*)vector_ref((Vector*)vector_ref(E,E->_sp-1),1));
    goto * dequeue(C);
_LD02:
    push(S,(void*)vector_ref((Vector*)vector_ref(E,E->_sp-1),2));
    goto * dequeue(C);
_LD03:
    push(S,(void*)vector_ref((Vector*)vector_ref(E,E->_sp-1),3));
    goto * dequeue(C);
_LD10:
    push(S,(void*)vector_ref((Vector*)vector_ref(E,E->_sp-2),0));
    goto * dequeue(C);
_LD11:
    push(S,(void*)vector_ref((Vector*)vector_ref(E,E->_sp-2),1));
    goto * dequeue(C);
_LD12:
    push(S,(void*)vector_ref((Vector*)vector_ref(E,E->_sp-2),2));
    goto * dequeue(C);
_LD13:
    push(S,(void*)vector_ref((Vector*)vector_ref(E,E->_sp-1),3));
    goto * dequeue(C);
_LDG:
    sym = (Symbol *)dequeue(C);
    if ((g = Hash_get(G, sym)) == NULL) printf("Unknown Key: %s\n", sym -> _table);
    else push(S, (void * )( * g));
    goto * dequeue(C);
_LDM:
    i = * (long * )pop(S); push(S, (void * )i);
    goto * dequeue(C);
_SET:
    v = vector_ref(S, S ->_sp - 1);
    ref = (Vector * )dequeue(C);
    i = (long)vector_ref(ref, 0);
    j = (long)vector_ref(ref, 1);
    Es = (Vector * )vector_ref(E, E ->_sp - i - 1); // printf("%d\n", E ->_sp); vector_print(Es);
    vector_set(Es,j , v);
    goto * dequeue(C);
_SET00:
    vector_set((Vector*)vector_ref(E,E->_sp-1),0,vector_ref(S,S->_sp-1));
    goto *dequeue(C);
_SET01:
    vector_set((Vector*)vector_ref(E,E->_sp-1),1,vector_ref(S,S->_sp-1));
    goto *dequeue(C);
_SET02:
    vector_set((Vector*)vector_ref(E,E->_sp-1),2,vector_ref(S,S->_sp-1));
    goto *dequeue(C);
_SET03:
    vector_set((Vector*)vector_ref(E,E->_sp-1),3,vector_ref(S,S->_sp-1));
    goto *dequeue(C);
_SET10:
    vector_set((Vector*)vector_ref(E,E->_sp-2),0,vector_ref(S,S->_sp-1));
    goto *dequeue(C);
_SET11:
    vector_set((Vector*)vector_ref(E,E->_sp-2),1,vector_ref(S,S->_sp-1));
    goto *dequeue(C);
_SET12:
    vector_set((Vector*)vector_ref(E,E->_sp-2),2,vector_ref(S,S->_sp-1));
    goto *dequeue(C);
_SET13:
    vector_set((Vector*)vector_ref(E,E->_sp-2),3,vector_ref(S,S->_sp-1));
    goto *dequeue(C);
_GSET:
    v = vector_ref(S, S ->_sp - 1);
    sym = (Symbol *)dequeue(C);
    Hash_put(G, sym, v);
    goto * dequeue(C);
_OADD:
    push(S, (void * )objadd((object*)pop(S),(object*)pop(S)));
    goto * dequeue(C);
_ADD:
_IADD:
    push(S, (void * )((long)pop(S) + (long)pop(S)));
    goto * dequeue(C);
//_FADD:
//    fx=(double*)pop(S);fy=(double*)pop(S);
//    fz = (double * )malloc(sizeof(double)); *fz=*fx+(*fy);
//    push(S,(void*)fz);
//    goto * dequeue(C);
_FADD:
    i = (long)pop(S);j=(long)pop(S);
    zz = *(double*)(&i) + *(double*)(&j);
    push(S, (void*)(*(long*)(&zz)));
    goto * dequeue(C);
_LADD:
    y = (mpz_ptr)pop(S); x = (mpz_ptr)pop(S);
    z = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(z);
    mpz_add(z,x,y);
    push(S, (void * )z);
    goto * dequeue(C);
_RADD:
    qy = (mpq_ptr)pop(S); qx = (mpq_ptr)pop(S);
    qz = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(qz);
    mpq_add(qz,qx,qy);
    push(S, (void * )qz);
    goto * dequeue(C);
_LCPY:
    z = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init_set(z, (mpz_ptr)pop(S));
    push(S, (void * )z);
    goto * dequeue(C);
_ITOL:
    z = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init_set_si(z, (long)pop(S));
    push(S, (void * )z);
    goto * dequeue(C);
_ITOR:
    qz=(mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(qz);
    mpq_set_si(qz,(long)pop(S),1);
    push(S,(void*)qz);
    goto * dequeue(C);    
_ITOF:
    //fz = (double*)malloc(sizeof(double));
    //*fz=(double)((long)pop(S));
    //push(S, (void * )fz);
    zz=(double)(long)pop(S);
    push(S, (void*)*(long*)&zz);
    goto * dequeue(C);
_ITOO:
    push(S,(void*)newINT((long)pop(S)));
    goto*dequeue(C);
_LTOI:
    push(S,(void*)mpz_get_si((mpz_ptr)pop(S)));
    goto*dequeue(C);
_LTOR:
    qz=(mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(qz);
    mpq_set_z(qz,(mpz_ptr)pop(S));
    push(S,(void*)qz);
    goto * dequeue(C);    
_LTOF:
    //fz = (double*)malloc(sizeof(double));
    //*fz=mpz_get_d((mpz_ptr)pop(S));
    zz=mpz_get_d((mpz_ptr)pop(S));
    push(S, (void * )*(long*)&zz);
    goto * dequeue(C);
_LTOO:
    push(S,(void*)newLINT((mpz_ptr)pop(S)));
    goto*dequeue(C);
_RTOI:
    z=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(z);
    qz=(mpq_ptr)pop(S);
    mpz_div(z,mpq_numref(qz),mpq_denref(qz));
    push(S,(void*)mpz_get_si(z));
    goto*dequeue(C);
_RTOL:
    z=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(z);
    qz=(mpq_ptr)pop(S);
    mpz_div(z,mpq_numref(qz),mpq_denref(qz));
    push(S,(void*)z);
    goto*dequeue(C);
_RTOF:
    //fz=(double*)malloc(sizeof(double));
    //*fz=mpq_get_d((mpq_ptr)pop(S));
    zz=mpq_get_d((mpq_ptr)pop(S));
    push(S,(void*)*(long*)&zz);
    goto * dequeue(C);
_RTOO:
    push(S,(void*)newRAT((mpq_ptr)pop(S)));
    goto * dequeue(C);
//_FTOI:
//    fz=(double*)pop(S);
//    push(S,(void*)(long)(*fz));
//    goto * dequeue(C);
_FTOI:
    i = (long)pop(S);push(S, (void*)(long)*(double*)(&i));
    goto * dequeue(C);
//_FTOL:
//    fz=(double*)pop(S);
//    mpz_init_set_d(z,*fz);
//    push(S,z);
//    goto * dequeue(C);
_FTOL:
    i = (long)pop(S);
    z = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init_set_d(z,*(double*)(&i));
    push(S,z);
    goto * dequeue(C);
_FTOR:
    //fz=(double*)pop(S);
    i = (long)pop(S);
    qz=(mpq_ptr)malloc(sizeof(MP_RAT));
    //mpq_init(qz);mpq_set_d(qz,*fz);
    mpq_init(qz);mpq_set_d(qz,*(double*)(&i));
    push(S,(void*)qz);
    goto * dequeue(C);
_FTOO:
    //push(S,(void*)newFLT(*(double*)pop(S)));
    i = (long)pop(S);
    push(S,(void*)newFLT(*(double*)(&i)));
    goto*dequeue(C);
_OTOI:
    push(S,(void*)obj2int((object*)pop(S)));
    goto*dequeue(C);
_OTOL:
    push(S,(void*)obj2long((object*)pop(S)));
    goto*dequeue(C);
_OTOR:
    push(S,(void*)obj2rat((object*)pop(S)));
    goto*dequeue(C);
_OTOF:
    //fz=(double*)malloc(sizeof(double));
    //*fz=obj2flt((object*)pop(S));
    //push(S,(void*)fz);
    zz=obj2flt((object*)pop(S));
    push(S, (void*)*((long*)(&zz)));
    goto*dequeue(C);
_IJTOO://rational number
    push(S,(void*)newRAT_i((long)pop(S),(long)pop(S)));
    goto*dequeue(C);
_OSUB:
    push(S, (void * )objsub((object*)pop(S),(object*)pop(S)));
    goto * dequeue(C);
_SUB:
_ISUB:
    i = (long)pop(S);
    push(S, (void * )((long)pop(S) - i));
    goto * dequeue(C);
_FSUB:
    //fy=(double*)pop(S);fx=(double*)pop(S);
    //fz = (double * )malloc(sizeof(double)); *fz=*fx-(*fy);
    //push(S,(void*)fz);
    i = (long)pop(S);j=(long)pop(S);
    zz = *(double*)(&j) - *(double*)(&i);
    push(S, (void*)(*(long*)(&zz)));
    goto * dequeue(C);
_LSUB:
    y = (mpz_ptr)(S->_table[--(S->_sp)]); x = (mpz_ptr)(S->_table[(S->_sp)-1]);
    mpz_sub(x,x,y);
    goto * dequeue(C);
_RSUB:
    qy = (mpq_ptr)pop(S); qx = (mpq_ptr)pop(S);
    qz = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(qz);
    mpq_sub(qz,qx,qy);
    push(S, (void * )qz);
    goto * dequeue(C);
_DEC:
_IDEC:
    S->_table[S ->_sp - 1] = (void * )((long)(S ->_table[S -> _sp - 1]) - 1);
    // (long)(S->_table[S ->_sp - 1]) ++ ;
    goto * dequeue(C);
_LDEC:
    z=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(z);
    mpz_sub_ui(z,(mpz_ptr)pop(S),1);
    push(S,z);
    goto *dequeue(C);
_ODEC:
    push(S,(void*)objdec((object*)pop(S)));
    goto*dequeue(C);
_INC:
_IINC:
    push(S, (void * )((long)(pop(S)) + 1)) ;
    goto * dequeue(C);
_LINC:
    z=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(z);
    mpz_add_ui(z,(mpz_ptr)pop(S),1);
    push(S,z);
    goto *dequeue(C);
_OINC:
    push(S,(void*)objinc((object*)pop(S)));
    goto*dequeue(C);
_OMUL:
    push(S, (void * )objmul((object*)pop(S), (object*)pop(S)));
    goto * dequeue(C);
_MUL:
_IMUL:
    push(S, (void * )((long)pop(S) * (long)pop(S)));
    goto * dequeue(C);
_FMUL:
    //fx=(double*)pop(S);fy=(double*)pop(S);
    //fz = (double * )malloc(sizeof(double)); *fz=*fx*(*fy);
    //push(S,(void*)fz);
    i = (long)pop(S);j=(long)pop(S);
    zz = *(double*)(&j) * (*(double*)(&i));
    push(S, (void*)(*(long*)(&zz)));
    goto * dequeue(C);
_LMUL:
    y = (mpz_ptr)pop(S); x = (mpz_ptr)pop(S);
    z = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(z);
    mpz_mul(z,x,y);
    push(S, (void * )z);
    goto * dequeue(C);
_RMUL:
    qy = (mpq_ptr)pop(S); qx = (mpq_ptr)pop(S);
    qz = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(qz);
    mpq_mul(qz,qx,qy);
    push(S, (void * )qz);
    goto * dequeue(C);
_ODIV:
    o = (object*)pop(S);
    push(S, (void * )objdiv((object*)pop(S),o));
    goto * dequeue(C);
_DIV:
_IDIV:
    if ((p = (long)pop(S))==0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);};
    push(S, (void * )((long)pop(S) / p));
    goto * dequeue(C);
_FDIV:
    //if(*(fy=(double*)pop(S))==0.0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);}
    //fx=(double*)pop(S);
    //fz = (double * )malloc(sizeof(double)); *fz=*fx/(*fy);
    //push(S,(void*)fz);
    i = (long)pop(S);j=(long)pop(S);if (i==0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);}
    zz = *(double*)(&j) / (*(double*)(&i));
    push(S, (void*)(*(long*)(&zz)));
    goto * dequeue(C);
_LDIV:
    y = (mpz_ptr)pop(S); 
    if (mpz_sgn(y)==0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);} 
    x = (mpz_ptr)pop(S);
    z = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(z);
    mpz_div(z,x,y);
    push(S, (void * )z);
    goto * dequeue(C);
_RDIV:
    qy = (mpq_ptr)pop(S);
    if (mpq_sgn(qy)==0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);}
    qx = (mpq_ptr)pop(S);
    qz = (mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(qz);
    mpq_div(qz,qx,qy);
    push(S, (void * )qz);
    goto * dequeue(C);
_OEQ:
    push(S, (void * )(long)objeq((object*)pop(S), (object*)pop(S)));
    goto * dequeue(C);
_EQ:
_IEQ:
    push(S, (void * )(long)((long)pop(S) == (long)pop(S)));
    goto * dequeue(C);
_FEQ:
    fx=(double*)pop(S);fy=(double*)pop(S);
    //fz = (double * )malloc(sizeof(double)); *fz=(*fx)==(*fy);
    push(S,(void*)(long)((*fx)==(*fy)));
    goto * dequeue(C);
_L_EQ:
    x = (mpz_ptr)pop(S); y = (mpz_ptr)pop(S);
    push(S,(void*)(long)(mpz_cmp(x, y)==0));
    goto*dequeue(C);
_REQ:
    qx = (mpq_ptr)pop(S); qy = (mpq_ptr)pop(S);
    push(S,(void*)(long)(mpq_cmp(qx, qy)==0));
    goto*dequeue(C);
// Not Equal
_ONEQ:
    push(S, (void * )(long)objneq((object*)pop(S), (object*)pop(S)));
    goto * dequeue(C);
_NEQ:
_INEQ:
    push(S, (void * )(long)((long)pop(S) != (long)pop(S)));
    goto * dequeue(C);
_FNEQ:
    fx=(double*)pop(S);fy=(double*)pop(S);
    push(S,(void*)(long)((*fx)!=(*fy)));
    goto * dequeue(C);
_LNEQ:
    x = (mpz_ptr)pop(S); y = (mpz_ptr)pop(S);
    push(S,(void*)(long)(mpz_cmp(x, y)!=0));
    goto*dequeue(C);
_RNEQ:
    qx = (mpq_ptr)pop(S); qy = (mpq_ptr)pop(S);
    push(S,(void*)(long)(mpq_cmp(qx, qy)!=0));
    goto*dequeue(C);
_OLEQ:
    push(S, (void * )(long)objle((object*)pop(S), (object*)pop(S)));
    goto * dequeue(C);
_LEQ:
_ILEQ:
    push(S, (void * )(long)((long)pop(S) >= (long)pop(S)));
    goto * dequeue(C);
_FLEQ:
    fx=(double*)pop(S);fy=(double*)pop(S);
    push(S,(void*)(long)((*fx)>=(*fy)));
    goto * dequeue(C);
_LLEQ:
    x = (mpz_ptr)pop(S); y = (mpz_ptr)pop(S);
    push(S,(void*)(long)(mpz_cmp(x, y)>=0));
    goto*dequeue(C);
_RLEQ:
    qx = (mpq_ptr)pop(S); qy = (mpq_ptr)pop(S);
    push(S,(void*)(long)(mpq_cmp(qx, qy)>=0));
    goto*dequeue(C);
_OLT:
    push(S, (void * )(long)objlt((object*)pop(S), (object*)pop(S)));
    goto * dequeue(C);
_LT:
_ILT:
    push(S, (void * )(long)((long)pop(S) > (long)pop(S)));
    goto * dequeue(C);
_FLT:
    fx=(double*)pop(S);fy=(double*)pop(S);
    push(S,(void*)(long)((*fx)>(*fy)));
    goto * dequeue(C);
_LLT:
    x = (mpz_ptr)pop(S); y = (mpz_ptr)pop(S);
    push(S,(void*)(long)(mpz_cmp(x, y)>0));
    goto*dequeue(C);
_RLT:
    qx = (mpq_ptr)pop(S); qy = (mpq_ptr)pop(S);
    push(S,(void*)(long)(mpq_cmp(qx, qy)>0));
    goto*dequeue(C);
_OGT:
    push(S, (void * )(long)objgt((object*)pop(S), (object*)pop(S)));
    goto * dequeue(C);
_GT:
_IGT:
    push(S, (void * )(long)((long)pop(S) < (long)pop(S)));
    goto * dequeue(C);
_FGT:
    fx=(double*)pop(S);fy=(double*)pop(S);
    push(S,(void*)(long)((*fx)<(*fy)));
    goto * dequeue(C);
_LGT:
    x = (mpz_ptr)pop(S); y = (mpz_ptr)pop(S);
    push(S,(void*)(long)(mpz_cmp(x, y)<0));
    goto*dequeue(C);
_RGT:
    qx = (mpq_ptr)pop(S); qy = (mpq_ptr)pop(S);
    push(S,(void*)(long)(mpq_cmp(qx, qy)<0));
    goto*dequeue(C);
_OGEQ:
    push(S, (void * )(long)objge((object*)pop(S), (object*)pop(S)));
    goto * dequeue(C);
_GEQ:
_IGEQ:
    push(S, (void * )(long)((long)pop(S) <= (long)pop(S)));
    goto * dequeue(C);
_FGEQ:
    fx=(double*)pop(S);fy=(double*)pop(S);
    push(S,(void*)(long)((*fx)<=(*fy)));
    goto * dequeue(C);
_LGEQ:
    x = (mpz_ptr)pop(S); y = (mpz_ptr)pop(S);
    push(S,(void*)(long)(mpz_cmp(x, y)<=0));
    goto*dequeue(C);
_RGEQ:
    qx = (mpq_ptr)pop(S); qy = (mpq_ptr)pop(S);
    push(S,(void*)(long)(mpq_cmp(qx, qy)<=0));
    goto*dequeue(C);
_ONEG:
    push(S, (void * )(long)objneg((object*)pop(S)));
    goto * dequeue(C);
_NEG:
_INEG:
    push(S, (void * )(long)(-(long)pop(S)));
    goto * dequeue(C);
_LNEG:
    z=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_neg(z,(mpz_ptr)pop(S));
    push(S,(void*)z);
    goto * dequeue(C);
_RNEG:
    qz=(mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_neg(qz,(mpq_ptr)pop(S));
    push(S,(void*)qz);
    goto * dequeue(C);
_FNEG:
    fz=(double*)malloc(sizeof(double));
    *fz=-(*(double*)pop(S));
    push(S,(void*)fz);
    goto * dequeue(C);
_BNOT:
_IBNOT:
    push(S, (void * )(long)(~(long)pop(S)));
    goto * dequeue(C);
_LBNOT:
    z=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_com(z, (mpz_ptr)pop(S));
    push(S,(void*)z);
    goto*dequeue(C);
_OBNOT:
    push(S,(void*)objneg((object*)pop(S)));
_CALL:
    n = (long)dequeue(C);
    fn = (Vector * )pop(S);
    l = vector_init(n);
    memcpy(l ->_table, (S ->_table) +(S ->_sp - n) , n * (sizeof(void * )) );
    l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);
    if ((long)vector_ref(fn,0)==FUNC_PRIM) goto __PCALL_S;
    push(R, (void * )C);
    push(EE, (void * )E);
    E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l);
    //E = (Vector * )vector_ref(fn, 2); push(E,l);
    C = vector_copy1((Vector * )vector_ref(fn, 1));
    goto * dequeue(C);
_TCALL:
    n = (long)dequeue(C);
    fn = (Vector * )pop(S);
    l = vector_init(n);
    memcpy(l ->_table, (S ->_table) +(S ->_sp - n) , n * (sizeof(void * )) );
    l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);
    if ((long)vector_ref(fn,0)==FUNC_PRIM) goto __PCALL_S;
    E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l);
    C = vector_copy1((Vector * )vector_ref(fn, 1));
    goto * dequeue(C);
_APL:
    n = (long)dequeue(C);//printf("%ld\n",n);
    fn = (Vector * )vector_ref(S, S->_sp-n);
    ll=(Vector*)vector_ref(S,S->_sp-1);
    l = vector_init(n+ll->_sp-1);
    memcpy(l ->_table, (S ->_table) +(S ->_sp - n+1) , (n-2) * (sizeof(void * )) );
    memcpy(l->_table+n-2,ll->_table,(ll->_sp)*(sizeof(void*)));
    //l ->_sp = n+(ll->_sp)-1; S ->_sp = S ->_sp - n; //vector_print(l);
    l ->_sp = n+(ll->_sp)-2; S ->_sp = S ->_sp - n; //vector_print(l);
    push(R, (void * )C);
    push(EE, (void * )E);
    E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l);
    C = vector_copy1((Vector * )vector_ref(fn, 1));
    goto * dequeue(C);
_TAPL:
    n = (long)dequeue(C);//printf("%ld\n",n);
    fn = (Vector * )vector_ref(S, S->_sp-n);
    ll=(Vector*)vector_ref(S,S->_sp-1);
    l = vector_init(n+ll->_sp-1);
    memcpy(l ->_table, (S ->_table) +(S ->_sp - n+1) , (n-2) * (sizeof(void * )) );
    memcpy(l->_table+n-2,ll->_table,(ll->_sp)*(sizeof(void*)));
    l ->_sp = n+(ll->_sp)-1; S ->_sp = S ->_sp - n; //vector_print(l);
    //push(R, (void * )C);
    //push(EE, (void * )E);
    E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l);
    C = vector_copy1((Vector * )vector_ref(fn, 1));
    goto * dequeue(C);
_PCALL: // primitive function call
    n = (long)dequeue(C);
    fn = (Vector*)pop(S);
    l = vector_init(n);
    memcpy(l->_table, (S->_table) + (S->_sp - n) , n * (sizeof(void * )));
    l->_sp = n; S->_sp = S->_sp - n;  // vector_print(l);
__PCALL_S:   
    func=(Funcpointer)vector_ref(fn,1); 
    push(S, func(l));
    goto * dequeue(C);
_RTN:
    E = (Vector * )pop(EE);
    C = (Vector * )pop(R);
    goto * dequeue(C);
_SEL:
    p = (long)pop(S);
    t_exp = (Vector * )dequeue(C);
    f_exp = (Vector * )dequeue(C);
    push(R, (void * )C);
    if (p) {C = t_exp;C ->_cp = 0; }
    else   {C = f_exp;C ->_cp = 0; }
    goto * dequeue(C);
_TSEL:
    p = (long)pop(S);
    t_exp = (Vector * )dequeue(C);
    f_exp = (Vector * )dequeue(C);
    if (p) C = vector_copy1(t_exp);
    else   C = vector_copy1(f_exp);
    goto * dequeue(C);
_JOIN:
    C = (Vector * )pop(R);
    goto * dequeue(C);
_LDF:
    code = (Vector * )dequeue(C);
    cl = vector_init(4);
    push(cl, (void * )FUNC_USER); push(cl, (void * )code); push(cl, (void * )E);
    push(S, (void * )cl);
    goto * dequeue(C);
_VEC:
    n = (long)dequeue(C);
    if (n==0) {l=vector_init(100);push(S,(void*)l);goto *dequeue(C);}
    l = vector_init(n>100 ? n : 100);
    memcpy(l ->_table, (S ->_table) +(S ->_sp - n), n * (sizeof(void * )) );
    l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);
    push(S, (void * )l);
    //push(S,(void*)newVECT(l));
    goto * dequeue(C);
_REF:
_VREF:
    n = (long)pop(S);
    l = (Vector*)pop(S);if (n<0) n=l->_sp+n;
    push(S, (void * )vector_ref(l,n));
    goto * dequeue(C);
_SREF:
    n=(long)pop(S);
    push(S,(void*)symbol_ref((Symbol*)pop(S),n));
    goto*dequeue(C);
_OREF:
    n=(long)pop(S);
    push(S,(void*)objref((object*)pop(S),n));
    goto*dequeue(C);
_VSET: // うまく動いていない！！
    n = (long)pop(S);
    l = (Vector * )pop(S);
    v = (void * )pop(S);
    vector_set(l, n, v);
    push(S, v);
    goto * dequeue(C);
_SSET:
    n=(long)pop(S);sym=(Symbol*)pop(S);
    push(S,(void*)symbol_set(sym,n,(Symbol*)pop(S)));
    goto*dequeue(C);
_OSET:
    n=(long)pop(S);o=(object*)pop(S);
    objset(o,n,(object*)vector_ref(S,S->_sp-1));
    goto*dequeue(C);
_HASH:
    n = (long)pop(S);
    h = Hash_init(n);
    push(S, (void * )h);
    goto * dequeue(C);
_LDH:
    sym = (Symbol *)pop(S);
    h = (Hash * )pop(S);
    if ((g = Hash_get(h, sym)) == NULL) printf("Unknown Key: %s\n", sym -> _table);
    else push(S, (void * )( * g));
    goto * dequeue(C);
_HSET:
    v = pop(S);
    sym = (Symbol *)pop(S);
    h = (Hash * )pop(S);
    Hash_put(h, sym, v);
    push(S, v);
    goto * dequeue(C);
_VPUSH:
    v = pop(S);
    l = (Vector * )pop(S);
    //l = (Vector * )vector_ref(S,S->_sp-1);
    push(l, v);
    push(S,(void*)l);
    goto * dequeue(C);
//_SPUSH:
_OPUSH:
    //v=vector_ref(S,S->_sp-1);
    v=pop(S);
    //objpush((object*)vector_ref(S, S->_sp-2),v);
    //w=pop(S);
    w=vector_ref(S,S->_sp-1);
    objpush((object*)w,v);
    //push(S,w);
    goto*dequeue(C);
_VPOP:
    push(S, pop((Vector * )pop(S)));
    goto * dequeue(C);
_SPOP:
    push(S,symbol_pop((Symbol*)pop(S)));
    goto * dequeue(C);
_OPOP:
    push(S,objpop((object*)pop(S)));
    goto *dequeue(C);
_DROP:
    pop(S);
    goto * dequeue(C);
_FPR:
    //printf("%f\n", *(double*)(S->_table[(S->_sp) - 1]));
    i = (long)(S->_table[(S->_sp) - 1]);
    printf("%f\n", *(double*)&i);
    goto * dequeue(C);
_LPR:
    gmp_printf("%Zd\n", (mpz_ptr)(S->_table[(S->_sp) -1]));
    goto * dequeue(C);
_SPR:
    printf("%s\n",((Symbol*)pop(S))->_table);
    goto*dequeue(C);
_OPR:
    printf("%s\n",objtostr((object*)(S->_table[(S->_sp)-1])));
    goto*dequeue(C);
_DUP:
    push(S, S->_table[S->_sp - 1 ]);
    goto * dequeue(C);
_SWAP:
    v = S->_table[S->_sp - 1];
    S->_table[S->_sp - 1] = S->_table[S->_sp - 2];
    S->_table[S->_sp - 2] = v;
    goto * dequeue(C);
_ROT:
    v = S->_table[S->_sp - 1];
    S->_table[S->_sp - 1] = S->_table[S->_sp - 2];
    S->_table[S->_sp - 2] = S->_table[S->_sp - 3];
    S->_table[S->_sp - 3] = v;
    goto * dequeue(C);
_2ROT:
    v = S->_table[S->_sp - 2];
    S->_table[S->_sp -2] = S->_table[S->_sp - 3];
    S->_table[S->_sp - 3] = S->_table[S->_sp - 4];
    S->_table[S->_sp - 4] = v;
    goto * dequeue(C);
_CALLS: // small call Eレジスタを使用せず、スタックのみをローカル変数として用いる
    n = (long)dequeue(C);
    push(R, (void * )C);
    // C = vector_copy1((Vector * )pop(S));
    C = (Vector * )pop(S); C -> _cp = 0;
    push(ssp,(void*)SSP);
    push(ssp,(void*)(long)S->_sp-n);
    SSP = S->_sp;//for(i=SSP-n;i<SSP;i++) printf(" %ld",(long)S->_table[i]);printf(":S%ld\n",SSP);vector_print(S);
    goto * dequeue(C);
_TCALLS:// tail small call
    n=(long)dequeue(C);
    // push(R, (void * )C);
    // C = vector_copy1((Vector * )pop(S));
    C = (Vector * )pop(S); C -> _cp = 0;
    //push(ssp,(void*)SSP);
    //push(ssp,(void*)(long)S->_sp-n);
    SSP=S->_sp;//for(i=SSP-n;i<SSP;i++) printf(" %ld",(long)S->_table[i]);printf(":TS%ld\n",SSP);vector_print(S);
    goto * dequeue(C);
_RTNS: //small call用のRTN
    //E = (Vector * )pop(EE);
    C = (Vector * )pop(R);
    v = S->_table[S->_sp-1];//printf("RTNS:%ld %ld", (long)S->_table[S->_sp-1],SSP);
    S->_sp=(long)pop(ssp);
    SSP=(long)pop(ssp);//printf("->%ld\n",SSP);
    //S->_table[SSP]=v;
    //S->_sp=SSP+1;
    push(S,v);
    //vector_print(S);
    goto * dequeue(C);
_LDP://small call用の関数をロードする
    push(S, dequeue(C));
    goto * dequeue(C);
_LDL://small call 内のローカル変数ロード
    n=(long)dequeue(C);
    push(S, S->_table[SSP-n-1]);//nがマイナスの場合の庶路追加要！
    goto * dequeue(C);
_VTOO:
    push(S,(void*)newVECT((Vector*)pop(S)));
    goto*dequeue(C);
_OTOV:
    push(S,(void*)((object*)pop(S))->data.ptr);
    goto*dequeue(C);
_STOO:
    push(S,(void*)newSTR((Symbol*)pop(S)));
    goto*dequeue(C);
_IPOW:
    n=1;p=1;j=(long)pop(S);i=(long)pop(S);
    if (j<0) {push(S,(void*)0);goto *dequeue(C);}
    if (i<0) {i=-i; if (j%2) p=-1;} 
    while (j>0) {
        if (j & 1) n*=i;
        i*=i;j/=2;
    }
    push(S,(void*)(p*n));
    goto *dequeue(C);
_LPOW:
    j=mpz_get_si((mpz_ptr)pop(S));
    x=(mpz_ptr)pop(S);
    z=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(z);
    mpz_pow_ui(z,x,j);
    push(S,(void*)z);
    goto * dequeue(C);
_RPOW:
    j=rtoi((mpq_ptr)pop(S));
    qz=(mpq_ptr)pop(S);
    x=mpq_numref(qz);mpz_pow_ui(x,x,j);
    y=mpq_denref(qz);mpz_pow_ui(y,y,j);
    //mpq_init(qz);
    mpq_set_num(qz,x);mpq_set_den(qz,y);
    push(S,(void*)qz);
    goto *dequeue(C);
_FPOW:
    //fy=pop(S);fx=pop(S);
    //fz=(double*)malloc(sizeof(double));
    //*fz=pow((*fx),(*fy));
    //push(S,fz);
    i = (long)pop(S);j=(long)pop(S);//if (i==0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);}
    zz = pow(*(double*)(&j), *(double*)(&i));
    push(S, (void*)(*(long*)(&zz)));
    goto *dequeue(C);
_OPOW:
    o=(object*)pop(S);
    push(S,objpow((object*)pop(S),o));
    goto*dequeue(C);
_IMOD:
    j=(long)pop(S);i=(long)(pop(S));
    push(S,(void*)(i % j));
    goto*dequeue(C);
_LMOD:
    y=(mpz_ptr)pop(S);x=(mpz_ptr)pop(S);
    z = (mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(z);
    mpz_tdiv_r (z, x, y);
    push(S,(void * )z);
    goto *dequeue(C);
_RMOD:
    qy=(mpq_ptr)pop(S);qx=(mpq_ptr)pop(S);
    qz = (mpq_ptr)malloc(sizeof(MP_RAT));
    w = (mpz_ptr)malloc(sizeof(MP_INT));
    mpq_init(qz);mpz_init(w);
    mpq_div(qz, qx, qy);
    mpz_tdiv_q(w, mpq_numref(qz),mpq_denref(qz));
    mpq_set_z(qz,w);
    mpq_mul(qz,qz,qy);
    mpq_sub(qz,qx,qz);
    push(S,(void*)qz);
    goto*dequeue(C);
_FMOD:
    //fy=(double*)pop(S);fx=(double*)pop(S);
    //fz=(double*)malloc(sizeof(double));
    //(*fz)=(*fx)-(*fy)*(double)(long)((*fx)/(*fy));
    i = (long)pop(S);j=(long)pop(S);if (i==0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);}
    zz = fmod(*(double*)(&j), *(double*)(&i));
    push(S, (void*)(*(long*)(&zz)));
    //push(S,(void*)fz);
    goto*dequeue(C);
_OMOD:
    o=(object*)pop(S);
    push(S,objmod(o,(object*)pop(S)));
    goto*dequeue(C);
_IBOR:
    push(S,(void*)((long)pop(S) | (long)pop(S)));
    goto*dequeue(C);
_LBOR:
    z=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_ior(z,(mpz_ptr)pop(S),(mpz_ptr)pop(S));
    push(S,(void*)z);
    goto*dequeue(C);
_OBOR:
    o=(object*)pop(S);
    push(S,(void*)objor(o,(object*)pop(S)));    
    goto *dequeue(S);
_IBAND:
    push(S,(void*)((long)pop(S) & (long)pop(S)));
    goto*dequeue(C);
_LBAND:
    z=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_and(z,(mpz_ptr)pop(S),(mpz_ptr)pop(S));
    push(S,(void*)z);
    goto*dequeue(C);
_OBAND:
    o=(object*)pop(S);
    push(S,(void*)objand(o,(object*)pop(S)));    
    goto *dequeue(S);
_VLEN:
    push(S,(void*)(long)((Vector*)pop(S))->_sp);
    goto*dequeue(C);
_SLEN:
    push(S,(void*)((Symbol*)pop(S))->_size);
    goto*dequeue(C);
_OLEN:
    push(S,(void*)objlen((object*)pop(S)));
    goto*dequeue(C);
_VAPP:
    v=pop(S);
    push(S,(void*)vector_append((Vector*)pop(S),(Vector*)v));
    goto*dequeue(C);
_SAPP:
    v=pop(S);
    push(S,(void*)symbol_append((Symbol*)pop(S),(Symbol*)v));
    goto*dequeue(C);
_STOI:
    push(S,symbol2objtype((Symbol*)pop(S),OBJ_INT));
    goto*dequeue(C);
_STOL:
    push(S,symbol2objtype((Symbol*)pop(S),OBJ_LINT));
    goto*dequeue(C);
_STOR:
    push(S,symbol2objtype((Symbol*)pop(S),OBJ_RAT));
    goto*dequeue(C);
_STOF:
    push(S,symbol2objtype((Symbol*)pop(S),OBJ_FLT));
    goto*dequeue(C);
_STOLF:
    push(S,symbol2objtype((Symbol*)pop(S),OBJ_LFLT));
    goto*dequeue(C);
_STOC:
    push(S,symbol2objtype((Symbol*)pop(S),OBJ_CMPLX));
    goto*dequeue(C);
_ITOS:
    push(S,(void*)objtype2symbol(OBJ_INT,pop(S)));
    goto* dequeue(C);
_LTOS:
    push(S,(void*)objtype2symbol(OBJ_LINT,pop(S)));
    goto* dequeue(C);
_RTOS:
    push(S,(void*)objtype2symbol(OBJ_RAT,pop(S)));
    goto* dequeue(C);
_FTOS:
    push(S,(void*)objtype2symbol(OBJ_FLT,pop(S)));
    goto* dequeue(C);
_OTOS:
    push(S,(void*)objtype2symbol(OBJ_GEN,pop(S)));
    goto* dequeue(C);
_VTOS:
    push(S,(void*)objtype2symbol(OBJ_VECT,pop(S)));
    goto* dequeue(C);
/*
_VMUL:
    i=(long)pop(S);
    ll=(Vector*)pop(S);l=vector_init(10);
    for (j=0;j<i;j++) l=vector_append(l,ll);
    push(S,(void*)l);
    goto*dequeue(C);
*/
_VMUL:
    i=(long)pop(S);
    ll=(Vector*)pop(S);l=vector_init(i*ll->_sp);
    for(j=0;j<i;j++) memcpy(l->_table+j*ll->_sp,ll->_table,ll->_sp*sizeof(void*));
    l->_sp=i*ll->_sp;
    push(S,(void*)l);
    goto*dequeue(C);
/*
_SMUL:
    i=(long)pop(S);
    sym=(Symbol*)pop(S);sym1=new_symbol("",0);
    for (j=0;j<i;j++) sym1=symbol_append(sym1,sym);
    push(S,(void*)sym1);
    goto*dequeue(C);
*/
_SMUL:
    i=(long)pop(S);sym=(Symbol*)pop(S);
    ch=(char*)malloc((i*sym->_size+1)*sizeof(char));
    //strcpy(ch,sym->_table);
    for(j=0;j<i;j++) //strcat(ch,sym->_table);
        memcpy(ch+j*sym->_size,sym->_table,sym->_size);
    ch[i*sym->_size]='\0';
    push(S,(void*)new_symbol(ch,i*sym->_size));
    goto *dequeue(C);
_VEQ:
    ll=(Vector*)pop(S);
    if ((l=(Vector*)pop(S))->_sp != ll->_sp) push(S,(void*)FALSE);
    else if ((long)(void*)l == (long)(void*)ll) push(S,(void*)TRUE) ;
    else {
        for(i=0;i<l->_sp;i++) {
            if (objcmp((object*)vector_ref(l,i), (object*)vector_ref(ll,i)) != 0) {push(S,(void*)FALSE); goto *dequeue(C);}
        }
        push(S,(void*)TRUE);
    }
    goto*dequeue(C);
_SEQ:
    sym=(Symbol*)pop(S);
    push(S,(void*)(long)symbol_eq((Symbol*)pop(S),sym));
    goto *dequeue(C);
_ISR:
    i=(long)pop(S);
    push(S,(void*)((long)pop(S)>>i));
    goto * dequeue(C);
_ISL:
    i=(long)pop(S);
    push(S,(void*)((long)pop(S)<<i));
    goto * dequeue(C);
_LSR:
    i=(long)pop(S);
    z=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(z);
    mpz_cdiv_q_2exp(z,(mpz_ptr)pop(S),i);
    push(S,(void*)z);
    goto *dequeue(C);
_LSL:
    i=(long)pop(S);
    z=(mpz_ptr)malloc(sizeof(MP_INT));mpz_init(z);
    mpz_mul_2exp(z,(mpz_ptr)pop(S),i);
    push(S,(void*)z);
    goto *dequeue(C);
_OSR:
    i=(long)pop(S);
    push(S,(void*)objsr((object*)pop(S),(object*)i));
    goto *dequeue(C);
_OSL:
    i=(long)pop(S);
    push(S,(void*)objsl((object*)pop(S),(object*)i));
    goto *dequeue(C);
_VSLS:
    j=(long)pop(S);i=(long)pop(S);
    l=(Vector*)malloc(sizeof(Vector));
    ll=(Vector*)pop(S);
    if (i<0) i=ll->_sp+i;
    if (j<=0) j=ll->_sp+j;
    if (j>=ll->_sp) j=ll->_sp;
    if (j<i) i=j;
    l->_table=&ll->_table[i];
    l->_cp=0;
    l->_sp=(j-i);
    l->_size=ll->_size-i;
    push(S,(void*)l);
    goto *dequeue(C);
_SSLS:
    j=(long)pop(S);i=(long)pop(S);
    sym=symbol_cpy_n((Symbol*)pop(S),i,j-i);
    push(S,(void*)sym);
    goto *dequeue(C);
_OSLS:
    j=(long)pop(S);i=(long)pop(S);
    push(S,(void*)objslice((object*)pop(S),i,j));
    goto *dequeue(C);
_LFADD:
    lfy=(mpfr_ptr)pop(S);lfx=(mpfr_ptr)pop(S);
    lfz = (mpfr_ptr )malloc(sizeof(__mpfr_struct));
    mpfr_init(lfz);
    mpfr_add(lfz,lfx,lfy,MPFR_RNDA);
    push(S,(void*)lfz);
    goto * dequeue(C);
_LFSUB:
    lfy=(mpfr_ptr)pop(S);lfx=(mpfr_ptr)pop(S);
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(lfz);
    mpfr_sub(lfz,lfx,lfy,MPFR_RNDA);
    push(S,(void*)lfz);
    goto * dequeue(C);
_LFMUL:
    lfy=(mpfr_ptr)pop(S);lfx=(mpfr_ptr)pop(S);
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(lfz);
    mpfr_mul(lfz,lfx,lfy,MPFR_RNDA);
    push(S,(void*)lfz);
    goto * dequeue(C);
_LFDIV:
    lfy=(mpfr_ptr)pop(S);
    if (mpfr_sgn(lfy)==0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);};
    lfx=(mpfr_ptr)pop(S);
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(lfz);
    mpfr_div(lfz,lfx,lfy,MPFR_RNDA);
    push(S,(void*)lfz);
    goto * dequeue(C);
_LFMOD:
    lfy=(mpfr_ptr)pop(S);lfx=(mpfr_ptr)pop(S);
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(lfz);
    mpfr_modf(lfz,lfx,lfy,MPFR_RNDA);
    push(S,(void*)lfz);
    goto * dequeue(C);
_LFPOW:
    lfy=(mpfr_ptr)pop(S);lfx=(mpfr_ptr)pop(S);
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(lfz);
    mpfr_pow(lfz,lfx,lfy,MPFR_RNDA);
    push(S,(void*)lfz);
    goto * dequeue(C);
_LFGT:
    lfx=(mpfr_ptr)pop(S);lfy=(mpfr_ptr)pop(S);
    push(S,(void*)(long)(mpfr_cmp(lfx, lfy)<0));
    goto*dequeue(C);
_LFLT:
    lfx=(mpfr_ptr)pop(S);lfy=(mpfr_ptr)pop(S);
    push(S,(void*)(long)(mpfr_cmp(lfx, lfy)>0));
    goto*dequeue(C);
_LFEQ:
    lfx=(mpfr_ptr)pop(S);lfy=(mpfr_ptr)pop(S);
    push(S,(void*)(long)(mpfr_cmp(lfx, lfy)==0));
    goto*dequeue(C);
_LFNEQ:
    lfx=(mpfr_ptr)pop(S);lfy=(mpfr_ptr)pop(S);
    push(S,(void*)(long)(mpfr_cmp(lfx, lfy) != 0));
    goto*dequeue(C);
_LFGEQ:
    lfx=(mpfr_ptr)pop(S);lfy=(mpfr_ptr)pop(S);
    push(S,(void*)(long)(mpfr_cmp(lfx, lfy)<=0));
    goto*dequeue(C);
_LFLEQ:   // OBJ_LFLT
    lfx=(mpfr_ptr)pop(S);lfy=(mpfr_ptr)pop(S);
    push(S,(void*)(long)(mpfr_cmp(lfx, lfy)>=0));
    goto*dequeue(C);
_LFNEG:
    lfx=(mpfr_ptr)pop(S);
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init(lfz);
    mpfr_neg(lfz,lfx,MPFR_RNDA);
    push(S,(void*)lfz);
    goto * dequeue(C);
_ITOLF:
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init_set_si(lfz, (long)pop(S),MPFR_RNDA);
    push(S, (void*)lfz);
    goto *dequeue(C);
_LTOLF:
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init_set_z(lfz,(mpz_ptr)pop(S),MPFR_RNDA);
    push(S, (void*)lfz);
    goto *dequeue(C);
_RTOLF:
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    mpfr_init_set_q(lfz,(mpq_ptr)pop(S),MPFR_RNDA);
    push(S, (void*)lfz);
    goto *dequeue(C);
_FTOLF:
    lfz = (mpfr_ptr)malloc(sizeof(__mpfr_struct));
    //fz=(double*)pop(S);
    i = (long)pop(S);
    mpfr_init_set_d(lfz,*(double*)&i, MPFR_RNDA);
    push(S, (void*)lfz);
    goto *dequeue(C);
_OTOLF:
    push(S, (void*)obj2lflt((object*)pop(S)));
    goto *dequeue(C);
_LFTOI:
    push(S,(void*)mpfr_get_si((mpfr_ptr)pop(S),MPFR_RNDA));
    goto *dequeue(C);
_LFTOL:
    z=(mpz_ptr)malloc(sizeof(MP_INT));
    mpz_init(z);
    mpfr_get_z(z,(mpfr_ptr)pop(S),MPFR_RNDA);
    push(S,(void*)z);
    goto *dequeue(C);
_LFTOR:
    qz=(mpq_ptr)malloc(sizeof(MP_RAT));
    mpq_init(qz);
    mpfr_get_q(qz,(mpfr_ptr)pop(S));
    push(S,(void*)qz);
    goto *dequeue(C);
_LFTOF:
    //fx=(double*)malloc(sizeof(double));
    //*fx=mpfr_get_d((mpfr_ptr)pop(S),MPFR_RNDA);
    zz = mpfr_get_d((mpfr_ptr)pop(S),MPFR_RNDA);
    push(S,(void*)*(long*)&zz);
    goto *dequeue(C);
_LFTOO:
    push(S,(void*)newLFLT((mpfr_ptr)pop(S)));
    goto*dequeue(C);
_LFTOS:
    push(S,(void*)objtype2str(OBJ_LFLT,pop(S)));
    goto *dequeue(C);
_CADD:
    cy=(complex*)pop(S);cx=(complex*)pop(S);
    cz = (complex * )malloc(sizeof(complex)); *cz=*cx+(*cy);
    push(S,(void*)cz);
    goto * dequeue(C);
_CSUB:
    cy=(complex*)pop(S);cx=(complex*)pop(S);
    cz = (complex * )malloc(sizeof(complex)); *cz=*cx-(*cy);
    push(S,(void*)cz);
    goto * dequeue(C);
_CMUL:
    cy=(complex*)pop(S);cx=(complex*)pop(S);
    cz = (complex * )malloc(sizeof(complex)); *cz=(*cx)*(*cy);
    push(S,(void*)cz);
    goto * dequeue(C);
_CDIV:
    if (*(cy=(complex*)pop(S)) == 0) {printf("RuntimeError:ZeroDivision!\n");Throw(3);};
    cx=(complex*)pop(S);
    cz = (complex * )malloc(sizeof(complex)); *cz=(*cx)/(*cy);
    push(S,(void*)cz);
    goto * dequeue(C);
_CPOW:
    cy=(complex*)pop(S);cx=(complex*)pop(S);
    cz = (complex * )malloc(sizeof(complex)); *cz=cpow((*cx),(*cy));
    push(S,(void*)cz);
    goto * dequeue(C);
_CNEG:
    cx=(complex*)pop(S);cz=(complex*)malloc(sizeof(complex));*cz=-(*cx);
    push(S, (void*)cz);
    goto *dequeue(C);
_CEQ:
    cy=(complex*)pop(S);cx=(complex*)pop(S);
    push(S,(void*)(long)((*cx)==(*cy)));
    goto * dequeue(C);
_CNEQ:
    cy=(complex*)pop(S);cx=(complex*)pop(S);
    push(S,(void*)(long)((*cx)!=(*cy)));
    goto * dequeue(C);
_ITOC:
    cz=(complex*)malloc(sizeof(complex));
    *cz=(double)(long)pop(S)+0.0*I;
    push(S, (void*)cz);
    goto *dequeue(C);
_LTOC:
    cz=(complex*)malloc(sizeof(complex));
    *cz=mpz_get_d((mpz_ptr)pop(S))+0.0*I;
    push(S, (void*)cz);
    goto *dequeue(C);
_RTOC:
    cz=(complex*)malloc(sizeof(complex));
    *cz=mpq_get_d((mpq_ptr)pop(S))+0.0*I;
    push(S, (void*)cz);
    goto *dequeue(C);
_FTOC:
    cz=(complex*)malloc(sizeof(complex));
    *cz=*(double*)pop(S)+0.0*I;
    push(S, (void*)cz);
    goto *dequeue(C);
_LFTOC:
    cz=(complex*)malloc(sizeof(complex));
    *cz=mpfr_get_d((mpfr_ptr)pop(S),MPFR_RNDA)+0.0*I;
    push(S,(void*)cz);
    goto *dequeue(C);
_OTOC:
    push(S, (void*)obj2c((object*)pop(S)));
    goto *dequeue(C);
_CTOO:
    //fy=pop(S);fx=pop(S);cz=(complex*)malloc(sizeof(complex));
    //*cz=(*fx)+(*fy)*I;
    //push(S,(void*)cz);
    push(S, (void*)newCMPLX((complex*)pop(S)));
    goto *dequeue(C);
_CTOS:
    push(S,(void*)objtype2symbol(OBJ_CMPLX,pop(S)));
    goto *dequeue(C);
_WHILE:
    n = (long)dequeue(C); 
    v = dequeue(C);
    p = (long)pop(S);
    if (p) {
        C->_cp = C->_cp-n-3;// !!!
        push(R,(void*)C);//vector_print(C);
        C=(Vector*)v; C->_cp=0;//vector_print(C);
    } else {
        push(S, (void*)0);
    }
    goto *dequeue(C);
_LOOP_SET:
    LOOP_COUNTER = (long)dequeue(S);
    goto *dequeue(C);
_LOOP:
    v = dequeue(C);
    if (LOOP_COUNTER) {
        push(S,(void*)0);
        goto *dequeue(C);
    }
    LOOP_COUNTER--;
    C->_cp = C->_cp-1;
    push(R, (void*)C);
    C = (Vector*)v;C->_cp=0;
    goto *dequeue(C);
_VMAP:  // S:[...,v0,v1,...,vn,fn] C:[vmap n ...]
    n=(long)pop(C);
    fn=(Vector*)pop(S);
    ll=vector_init(3);
    push(R, (void * )C);
    push(EE, (void * )E);
    i=0;
    while (TRUE) {
        l=vector_init(3);
        for (j=S->_sp-1;j>S->_sp-n;j--) {
            if (i >= ((Vector*)vector_ref(S,j))->_sp)  goto loopend; 
            push(l,vector_ref((Vector*)vector_ref(S,j),i));
        }
        E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l);
        //E = (Vector * )vector_ref(fn, 2); push(E,l);
        C = vector_copy1((Vector * )vector_ref(fn, 1));
        push(ll,eval(S,E,C,R,EE,G));
        i++;
    }
    loopend:
    E=(Vector*)pop(EE);C=(Vector*)pop(R);
    push(S,(void*)ll);
    goto *dequeue(C);

_SET_LADD00:
    l =vector_ref(E, E->_sp-1);
    mpz_add((mpz_ptr)(l->_table[0]),(mpz_ptr)(l->_table[0]),(mpz_ptr)pop(S));
    push(S,(void*)(l->_table[0]));
    goto *dequeue(C);
_SET_ISUB00:
    l=vector_ref(E, E->_sp-1);
    v = (void*)((long)(l->_table[0]) -(long)pop(S));
    l->_table[0] = v;push(S,(void*)v);
    goto *dequeue(C);
//_SET_IMUL:
//_SET_IDIV:
//_SET_IMOD:
//_SET_IBOR:
//_SET_IBAND:

/*
   Vector * vector_make(void * L[], int N) {
   int i ;
   Vector * V = vector_init(N);
   for(i = 0;  i<N; i ++ ) {
   push(V, (void * )L[i]);
   };
   return V;
   }
   */
}