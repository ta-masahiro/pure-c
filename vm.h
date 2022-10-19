#include "hash.h"
#include "object.h"
//Symbol*new_symbol(unsigned char*s, unsigned long i);
mpz_ptr new_long();
mpz_ptr new_long_valued(long val);
mpz_ptr new_long_str(char* s);

typedef void*(*Funcpointer)(Vector*);
typedef void (*Pfuncpointer)(void**,int);
enum CODE { STOP,  LDC,  LD,   ADD,  CALL, RTN,  SEL,  JOIN, LDF,  SET,  LEQ,  LDG,  GSET, SUB,  \
            DEC,   TCALL,TSEL, DROP, EQ,   INC,  MUL,  DIV,  VEC,  REF,  VSET, HASH, LDH,  HSET, \
            VPUSH, VPOP, LADD, LSUB, LMUL, ITOL, LPR,  PCALL,LDM,  DUP,  SWAP, ROT, _2ROT, CALLS,\
            TCALLS,RTNS, LDP,  LDL,  FADD, FSUB, FMUL, FDIV, FPR,  ITOF, LCPY, OADD, OSUB, OMUL, \
            ODIV,  OEQ,  OLEQ, ITOO, OPR,  ODEC, OINC, IADD, ISUB, IMUL, IDIV, IEQ,  ILEQ, IDEC, \
            IINC,  LTOO, FTOO, IJTOO,SPR,  LDIV, OLT,  LT,   ILT,  GT  , IGT,  OGT,  GEQ,  IGEQ, \
            OGEQ,  NEG,  INEG, ONEG, BNOT, APL,  TAPL, FEQ,  FLEQ, FGEQ, FLT,  FGT,  L_EQ, LLEQ, \
            LGEQ,  LLT,  LGT , RADD, RSUB, RMUL, RDIV, REQ,  RLEQ, RGEQ, RLT,  RGT,  ITOR, OTOF, \
            LTOR,  LTOF, RTOF, RTOO ,LTOI, RTOI, RTOL, FTOI, FTOL, FTOR, LNEG, RNEG, FNEG, LINC, \
            LDEC,  NEQ,  INEQ, LNEQ, RNEQ, FNEQ, ONEQ, OTOI, OTOL, OTOR, VTOO, STOO, IPOW, LPOW, \
            RPOW,  FPOW, OPOW, IMOD, LMOD, RMOD, FMOD, OMOD, IBOR, LBOR, OBOR, IBNOT,LBNOT,OBNOT,\
            IBAND, LBAND,OBAND,VLEN, SLEN, OLEN, VAPP, SAPP, VREF, SREF, OREF, SSET, OSET, STOI, \
            STOL,  STOR, STOF, ITOS, LTOS, RTOS, FTOS, OTOS, VTOS, SPOP, OPOP, OPUSH,SMUL, VMUL, \
            VEQ,   SEQ,  ISR,  ISL,  LSR,  LSL,  OSR,  OSL,  OTOV, VSLS, SSLS, OSLS, LD00, LD01, \
            LD02,  LD03, LD10, LD11, LD12, LD13, SET00,SET01,SET02,SET03,SET10,SET11,SET12,SET13,\
            LFADD, LFSUB,LFMUL,LFDIV,LFMOD,LFPOW,LFGT, LFLT, LFEQ, LFNEQ,LFGEQ,LFLEQ,LFNEG,ITOLF,\
            LTOLF, RTOLF,FTOLF,OTOLF,LFTOI,LFTOL,LFTOR,LFTOF,LFTOO,CADD, CSUB, CMUL, CDIV, CPOW, \
            CNEG,  LFTOS,ITOC, LTOC, RTOC, FTOC, LFTOC,CTOO, OTOC, CTOS, CEQ,  CNEQ, WHILE,LOOP_SET,\
            LOOP,  STOLF,STOC, DIC,  ITOK, FTOK, VTOK, IBXOR,LBXOR,OBXOR,VSLS_,V_SLS,SSLS_,S_SLS,\
            OSLS_, O_SLS,DLEN, LDL0, LDL1, LDL2, LDL3, LDL4 ,DTOO, DTOS ,OTOD, SPUSH,OTOA, ATOO, \
            ATOS,  ALEN ,LTOK, RTOK, LFTOK,CTOK, OTOK, DTOK, ATOK, STL,  STL0, STL1, STL2, STL3, \
            STL4,  LDMF ,PAPL, APLS, TAPLS};

enum FUNCTION_ID { FUNC_SYS,FUNC_PRIM,FUNC_USER,FUNC_SMALL,MACRO_C,MACRO_F,MACRO_S};

extern int op_size[];
extern char*code_name[];
void * eval(Vector * S, Vector * E, Vector * Code, Vector * R, Vector * EE, Hash * G);