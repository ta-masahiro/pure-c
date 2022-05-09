/*#include "vm.h"
#include "lexparse.h"

void disassy(Vector*v,int i,FILE*fp);
*/
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "generate.h"

int op2_1[] = {'+', '-', '*', '/', '%', '*'*256+'*','<'*256+'-', '|', '&', '^', '>'*256+'>', '<'*256+'<', '>', '<', '='*256+'=', '!'*256+'=', '>'*256+'=', '<'*256+'=',0};
//char*op2_alt_name[]={"add","sub","mul","div","mod","pow","push",NULL,NULL,"sr","sl","gt","lt","eq","neq","ge","le"};
enum CODE op2_2[18][19] = {
                {IADD, ISUB, IMUL, IDIV, IMOD, IPOW, 0,    IBOR, IBAND,IBXOR,ISR, ISL, IGT, ILT, IEQ, INEQ, IGEQ, ILEQ, 0},   // OBJ_NONEはINTと同じに   
              //{ADD,  SUB,  MUL,  DIV,  MOD,  POW,  PUSH, BOR,  BAND, BXOR, SR,  SL,  GT,  LT,  EQ,  NEQ,  GEQ,  LEQ , 0},
                {IADD, ISUB, IMUL, IDIV, IMOD, IPOW, 0,    IBOR, IBAND,IBXOR,ISR, ISL, IGT, ILT, IEQ, INEQ, IGEQ, ILEQ, 0},   // OBJ_INT
                {LADD, LSUB, LMUL, LDIV, LMOD, LPOW, 0,    LBOR, LBAND,LBXOR,LSR, LSL, LGT, LLT, L_EQ,LNEQ, LGEQ, LLEQ, 0},   // OBJ_LINT
                {RADD, RSUB, RMUL, RDIV, RMOD, RPOW, 0,    0,    0,    0,    0,   0,   RGT, RLT, REQ, RNEQ, RGEQ, RLEQ, 0},   // OBJ_RAT
                {FADD, FSUB, FMUL, FDIV, FMOD, FPOW, 0,    0,    0,    0,    0,   0,   FGT, FLT, FEQ, FNEQ, FGEQ, FLEQ, 0},   // OBJ_FLT
                {LFADD,LFSUB,LFMUL,LFDIV,LFMOD,LFPOW,0,    0,    0,    0,    0,   0,   LFGT,LFLT,LFEQ,LFNEQ,LFGEQ,LFLEQ,0},   // OBJ_LFLT
                {CADD, CSUB, CMUL, CDIV, 0,    CPOW, 0,    0,    0,    0,    0,   0,   0,   0,   CEQ, CNEQ, 0    ,0    ,0},   // OBJ_CMPLX
                {OADD, OSUB, OMUL, ODIV, OMOD, OPOW, OPUSH,OBOR, OBAND,OBXOR,OSR, OSL, OGT, OLT, OEQ, ONEQ, OGEQ, OLEQ, 0},   // OBJ_GEN
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_SYSFUNC
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_PFUNC
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_UFUNC
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_CNT
                {VAPP, 0,    VMUL, 0,    0,    0,    VPUSH,0,    0,    0,    0,   0,   0,   0,   VEQ, 0,    0,    0,    0},   // OBJ_VECT
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_DICT
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_PAIR
                {SAPP, 0,    SMUL, 0,    0,    0,    SPUSH,0,    0,    0,    0,   0,   0,   0,   SEQ, 0,    0,    0,    0},   // OBJ_SYM
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_ARRAY
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_IO
                };
obj_type op2_3[]={0,0,0,0,0,0,0,0,0,0,0,0,OBJ_INT,OBJ_INT,OBJ_INT,OBJ_INT,OBJ_INT,OBJ_INT,0};

int op1_1[] = {'-', '~', '@','+'*256+'+', '-'*256+'-', '-'*256+'>', 0};
enum CODE op1_2[18][6] = 
               {{INEG, IBNOT,0,    IINC, DEC , 0   },   // NONE
              //{NEG, BNOT,  0,　　INC,  DEC , VPOP},   
                {INEG, BNOT, 0,    INC,  DEC , 0   },   // INT
                {LNEG, LBNOT,0,    LINC, LDEC, 0   },   // LONG
                {RNEG, 0,    0,    0,    0   , 0   },   // RAT
                {FNEG, 0,    0,    0,    0   , 0   },   // FLOAT
                {LFNEG,0,    0,    0,    0   , 0   },   // LFLOAT
                {CNEG, 0,    0,    0,    0   , 0   },    // CMPLX 
                {ONEG, OBNOT,OLEN, OINC, ODEC, OPOP},     // GEN
                {0,    0,    0,    0,    0   , 0   },     // SYSFUNC
                {0,    0,    0,    0,    0   , 0   },     // PFUNC
                {0,    0,    0,    0,    0   , 0   },     // UFUNC
                {0,    0,    0,    0,    0   , 0   },     // CNT
                {0,    0,    VLEN, 0,    0   , VPOP},     // VECT
                {0,    0,    DLEN, 0,    0   , 0   },     // DICT
                {0,    0,    0,    0,    0   , 0   },     // PAIR
                {0,    0,    SLEN, 0,    0   , SPOP},     // SYM
                {0,    0,    ALEN,  0,    0   , 0   },     // ARRAY
                {0,    0,    0,    0,    0   , 0   },     // IO
               };
obj_type op1_3[]={0,0,OBJ_INT,0,0,OBJ_GEN};
enum CODE conv_op[18][19] = 
                        {{0,  0,    ITOL, ITOR, ITOF,  0,    ITOO,  ITOO },    //OBJ_NONEには0が入っているのでINTとみなす
                    //  NONE  INT   LONG  RAT   FLOAT LFLOAT CMPLX  GEN    SYSFUNC PFUNC UFUNC CNT   VECT DICT PAIR SYM   ARRAY IO  KEY
                        {0,   0,    ITOL, ITOR, ITOF, ITOLF, ITOC,  ITOO,  0,      0,    0,    0,    0,   0,   0,   ITOS, 0,    0,  ITOK  },
                        {0,   LTOI, 0   , LTOR, LTOF, LTOLF, LTOC,  LTOO,  0,      0,    0,    0,    0,   0,   0,   LTOS, 0,    0,  0  },
                        {0,   RTOI, RTOL, 0   , RTOF, RTOLF, RTOC,  RTOO,  0,      0,    0,    0,    0,   0,   0,   RTOS, 0,    0,  0  },
                        {0,   FTOI, FTOL, FTOR, 0   , FTOLF, FTOC,  FTOO,  0,      0,    0,    0,    0,   0,   0,   FTOS, 0,    0,  FTOK  },
                        {0,   LFTOI,LFTOL,LFTOR,LFTOF,0,     LFTOC, LFTOO, 0,      0,    0,    0,    0,   0,   0,   LFTOS,0,    0,  0  },
                        {0,   0,    0,    0,    0,    0,     0,     CTOO,  0,      0,    0,    0,    0,   0,   0,   CTOS, 0,    0,  0  },
                        {0,   OTOI, OTOL, OTOR, OTOF, OTOLF, OTOC,  0,     0,      0,    0,    0,    OTOV,OTOD,0,   OTOS, OTOA, 0,  0  },
                        {0,   0,    0,    0,    0,    0,     0,     0,     0,      0,    0,    0,    0,   0,   0,   0,    0,    0,  0  },//SYSFUNC
                        {0,   0,    0,    0,    0,    0,     0,     0,     0,      0,   -1,    0,    0,   0,   0,   0,    0,    0,  0  },//PFUNC
                        {0,   0,    0,    0,    0,    0,     0,     0,     0,     -1,    0,    0,    0,   0,   0,   0,    0,    0,  0  },//UFUNC
                        {0,   0,    0,    0,    0,    0,     0,     0,     0,      0,    0,    0,    0,   0,   0,   0,    0,    0,  0  },//CNT
                        {0,   0,    0,    0,    0,    0,     0,     VTOO,  0,      0,    0,    0,    0,   0,   0,   VTOS, 0,    0,  VTOK  },//VECTOR
                        {0,   0,    0,    0,    0,    0,     0,     DTOO,  0,      0,    0,    0,    0,   0,   0,   DTOS, 0,    0,  0  },//DICT
                        {0,   0,    0,    0,    0,    0,     0,     0,     0,      0,    0,    0,    0,   0,   0,   0,    0,    0,  0  },//PAIR
                        {0,   STOI, STOL, STOR, STOF, 0,     0,     STOO,  0,      0,    0,    0,    0,   0,   0,   0,    0,    0,  0  },//SYM
                        {0,   0,    0,    0,    0,    0,     0,     ATOO,  0,      0,    0,    0,    0,   0,   0,   ATOS, 0,    0,  0  },//ARRAY
                        {0,   0,    0,    0,    0,    0,     0,     0,     0,      0,    0,    0,    0,   0,   0,   0,    0,    0,  0  } //IO 
                        };
/* 後で完成させること
enum CODE add_op[][5] = {
    {   IADD,   LiADD,  RiADD,  FADD,   LFiADD  },
    {   LADDi,  LADD,   RlADD,  FADD,   LFlADD  },
    {   RADDi,  RADDl,  RADD,   FADD,   LFrADD  },
    {   FADD,   FADD,   FADD,   FADD,   LFfADD  },
    {   LFADDi, LFADDl, LFADDr, LFADDf, LFADD   }
};
enum CODE sub_op[][5] = {
    {   ISUB,   LiSUB,  RiSUB,  FSUB,   LFiSUB  },
    {   LSUBi,  LSUB,   RlSUB,  FSUB,   LFlSUB  },
    {   RADDi,  RADDl,  RADD,   FADD,   LFrADD  },
    {   FADD,   FADD,   FADD,   FADD,   LFfADD  },
    {   LFADDi, LFADDl, LFADDr, LFADDf, LFADD   }
};
enum CODE mul_op[][5] = {
    {   IMUL,   LiMUL,  RiMUL,  FMUL,   LFiMUL  },
    {   LMULi,  LMUL,   RlSUB,  FMUL,   LFlMUL  },
    {   RMULi,  RMUL,   RMUL,   FMUL,   LFrMUL  },
    {   FMUL,   FMUL,   FMUL,   FMUL,   LFfMUL  },
    {   LFMULi, LFMULl, LFMULr, LFMULf, LFMUL   }
};
*/
enum CODE get_convert_op(obj_type from, obj_type to) {
    if (from<=to) return 0;
    return conv_op[from][to];
}

Hash* G;                // global 環境
Hash* GLOBAL_VAR;       // global nameの型を保持
Hash* PRIMITIVE_FUNC;   // primitive関数を保持
Hash* CLASS_NAME;       // class名を保持
Hash * IMPORT_NAME;
/*
typedef struct {
    obj_type type;
    obj_type functon_ret_type;
    Vector  *arg_type;
    int dotted;
} code_type;
*/
code_type * new_ct(obj_type type,obj_type frt, Vector*at,int dot ) {
    code_type* ct=(code_type*)malloc(sizeof(code_type));
    ct->type=type;ct->functon_ret_type=frt;ct->arg_type=at,ct->dotted=dot;
    return ct;
}
/*
code_type * new_ct(obj_type type, code_type frt, Vector*at,int dot ) {
    code_type* ct=(code_type*)malloc(sizeof(code_type));
    ct->type = type; ct->functon_ret_type = frt; ct->arg_type = at, ct->dotted = dot;
    return ct;
}
*/
void* code_type_print(code_type*ct) {
    printf("Type: %s",dcl_string[ct->type]);
    if (ct->type == OBJ_UFUNC) {
        printf("FunctionRetType:%s FunctionDotted %d FunctionArgType:",dcl_string[ct->functon_ret_type],ct->dotted);
        vector_print(ct->arg_type);
    } 
    printf("\n");
}

void put_gv(Symbol*var_name, code_type* ct) {// printf("%s\n",var_name->_table);
    //global_var_type *gvt=(global_var_type*)malloc(sizeof(global_var_type));
    //gvt->type=var_type;
    Hash_put(GLOBAL_VAR,var_name,(void*)ct);
}

code_type* get_gv(Symbol*var_name) {// printf("%s\n",var_name->_table);
    void ** t ;
    if ((t= Hash_get(GLOBAL_VAR,var_name))==NULL) {
        //未定義
        return NULL; 
    } else {
        return (code_type*)(*t);
    };
}

void* env_print(Vector*env) {
    Vector*l_env;
    Data *d;
    int i,j;
    for(i=0;i<env->_sp;i++) {
        l_env=vector_ref(env,i);
        for (j=0;j<l_env->_sp;j++) {
            d=vector_ref(l_env,j);
            printf("%s :",d->key->_table);
            code_type_print((code_type*)d->val);
        }printf("\n");
    }
}

Vector * var_location(Symbol * varname, Vector * env) { // env: [[(sym00:type00),  (sym01:type01),...], [(sym10:type10),(sym11),...]]
    int i,j,dotted,max_j,ttt;
    Vector * v, * nv, * rv;//printf("varname:%s\n",varname->_table);
    //env_print(env);//PR(999);//obj_type type; 
    if (is_stac_empty(env)) return NULL; 
    for(i = 0; i< env->_sp; i ++ ) {
        v = vector_ref(env,env->_sp-i-1);//vector_print(v);
        if (v->_sp==0) continue;            // 引数なしの場合は飛ばす
        if ((ttt=symbol_eq((Symbol*)((Data*)vector_ref(v,v->_sp-1))->key,new_symbol("..",2)))) {max_j=v->_sp-1;dotted=TRUE;} else {max_j=v->_sp;dotted=FALSE;}
        for (j = 0; j<max_j; j ++ ) {//printf("env%d:%s\n",j,((Symbol*)vector_ref(v,j))->_table);printf("%d\n",symbol_eq(varname,vector_ref(v,j)));
            if (symbol_eq(varname, ((Data*)vector_ref(v, j))->key)) {
                nv = vector_init(2); 
                //if ((j==v->_sp-1) && (strcmp(((Symbol*)vector_ref(v,j+1))->_table,"..")==0))  j=-(j+1);
                push(nv,(void*)(long)i);
                if (dotted && j==max_j-1) {
                    push(nv,(void*)(long)(-j-1));
                    rv=vector_init(2);
                    push(rv,(void*)nv);push(rv,(void*)new_ct(OBJ_VECT,OBJ_NONE,(void*)0,FALSE));//PR(8888888); 
                    return rv; // return [[pos_i,pos_j],<code_type>]
                } else {push(nv, (void * )(long)j);
                    rv=vector_init(2);
                    push(rv,(void*)nv);push(rv,((Data*)vector_ref(v,j))->val);//PR(8888888); 
                    return rv; // return [[pos_i,pos_j],<code_type>]
                }
            }//PR(77777777);
        }
    }
    return NULL;
}

code_ret*new_code(Vector*code, code_type * ct) {
    code_ret*r=(code_ret*)malloc(sizeof(code_ret));
    r->code=code;r->ct=ct;
    return r;
}

void* create_zero(obj_type type) {
    void* r;
    mpz_ptr z;
    mpq_ptr q;
    mpfr_ptr lf;
    Vector*cl;
    complex *c;
    double* d;
    switch(type) {
        case OBJ_NONE:
        case OBJ_INT: return (void*)0;
        case OBJ_LINT:z = (mpz_ptr)malloc(sizeof(MP_INT));mpz_init(z); return (void*)z;
        case OBJ_RAT: q = (mpq_ptr)malloc(sizeof(MP_RAT));mpq_init(q); return (void*)q;
        case OBJ_FLT: d = (double*)malloc(sizeof(double));*d=0; return (void*)d;
        case OBJ_LFLT:lf = (mpfr_ptr)malloc(sizeof(__mpfr_struct));mpfr_init_set_si(lf,0,MPFR_RNDA); return (void*)lf;
        case OBJ_CMPLX: c=(complex*)malloc(sizeof(complex));*c=0;return (void*)c;
        case OBJ_GEN: return newINT(0);
        case OBJ_SYM: return (void*)new_symbol("",0);
        case OBJ_VECT:return (void*)vector_init(0);
        case OBJ_UFUNC:
            cl = vector_init(4);
            push(cl, (void * )FUNC_USER); push(cl, (void * )0); push(cl, (void * )0);
            return cl;
    }
}

//code_ret* codegen(ast*a,Vector*v,int tail);

Vector *make_arg_list_type(ast *arg_list_ast) {//途中！　
    // arg_list_astを受け取って、それぞれのargのCTを作り、vectorに入れて返す
    if ((arg_list_ast->type != AST_ARG_LIST) && (arg_list_ast->type != AST_ARG_LIST_DOTS)) {
        printf("SyntaxError:not argment list!\n");Throw(0);
    }

    int i, dotted;
    ast * arg_ast_i;
    Data *d;
    Vector *args = vector_init(3), *v = vector_init(3);   // v必要？ 
    for(i = 0; i < arg_list_ast->table->_sp; i ++ ) {
        arg_ast_i=(ast*)vector_ref(arg_list_ast->table, i);// ast_print(a2,0);
        if (arg_ast_i->type == AST_VAR) {
            d=(Data*)malloc(sizeof(Data));
            d->key=(Symbol*)vector_ref(arg_ast_i->table,0);
            //
            //if (arg_ast_i->o_type==OBJ_UFUNC ) {
            //    d->val=new_ct(arg_ast_i->o_type,OBJ_GEN,(void*)0,TRUE);
            //} else {
                d->val=new_ct(arg_ast_i->o_type,OBJ_NONE,(void*)0,FALSE);
            //}
            push(args,(void*)d);push(v,(void*)arg_ast_i->o_type);
        } else if (arg_ast_i->type == AST_FCALL) {
            // 関数プロトタイプの場合を記載すること
            // AST_FCALL [AST_NAME,[AST_EXP_LIST [AST,AST,...]]]
            //            <0>       <1>           <1,1>...
            ast *f_arg=(ast*)vector_ref(arg_ast_i->table,1);
            Vector *d_args=make_arg_list_type(f_arg);
            Vector *_args=(Vector*)vector_ref(d_args,0);
            Vector *_v=(Vector*)vector_ref(d_args,1);
            if (strcmp(((Data*)vector_ref(_args,_args->_sp-1))->key->_table,"..")==0) dotted=TRUE; else dotted=FALSE;

            d=(Data*)malloc(sizeof(Data));
            ast *fname=(ast*)vector_ref(arg_ast_i->table,0);
            if (fname->type != AST_VAR) {printf("SyntaxError:MustBeFunctionName!\n");Throw(0);}
            d->key=(Symbol*)vector_ref(fname->table,0);
            d->val=new_ct(OBJ_UFUNC, arg_ast_i->o_type,_v,dotted);
            push(args,(void*)d);push(v,(void*)OBJ_UFUNC);
        } else {printf("illegal argment!\n");}
    } //for(i=0;i<args->_sp;i++) printf("%s\t",((Symbol*)vector_ref(args,i))->_table);printf("\n");
    if (arg_list_ast->type==AST_ARG_LIST_DOTS) {
        d=(Data*)malloc(sizeof(Data));
        d->key = new_symbol("..",2);//printf("%s\n",d->key->_table);
        d->val = new_ct(OBJ_NONE,OBJ_NONE,(void*)0,FALSE);
        push(args,(void*)d);
    } //vector_print(v);
    //push(env,(void*)args);//PR(1);
    Vector *retvect=vector_init(3);
    push(retvect,(void*)args);push(retvect,(void*)v);   //argsはsymbolとctのペア、vはctのみ
    //return args;
    return retvect;
}

code_ret * codegen_vect(ast*vect_ast,Vector*env,int tail) {
    // AST_VECT [AST_expr_list [AST_v1,AST_v2,...]]
    //                          <0,0>  <0,1> ...
    Vector*code=vector_init(3), *code1;
    code_ret*code_s;
    code_type *ct1;
    obj_type type1;
    int i;

    if (((ast*)vector_ref(vect_ast->table,0))->type == AST_LIT) {   // nullvectorの場合
        push(code,(void*)VEC);push(code,(void*)0);                  // ->[ VECT 0 ]
        return new_code(code,new_ct(OBJ_VECT,OBJ_NONE,(void*)0,FALSE));
    } 
    int n=((ast*)vector_ref(vect_ast->table,0))->table->_sp;
    for(i = 0; i < n; i ++ ) {
        code_s = codegen((ast*)vector_ref(((ast*)vector_ref(vect_ast->table, 0))->table, i),env, FALSE);
        code1 = code_s->code;
        ct1 = code_s->ct;
        type1 = ct1->type;
        if (type1 != OBJ_GEN) push(code1,(void*)conv_op[type1][OBJ_GEN]);
        code=vector_append(code,code1);
    }
    push(code,(void*)VEC);push(code,(void*)(long)n);
    return new_code(code,new_ct(OBJ_VECT,OBJ_GEN,(void*)0,FALSE));

}

code_ret * codegen_dict(ast*pair_list_ast,Vector*env,int tail) {
    // ペアリストタイプのastから辞書リテラルを作る 
    // AST_pair_list [AST_pair1 [AST_pair_left,AST_pair_right],AST_pair2,...]]
    //                0          <0,0>         <0,1> ...       1
    Vector*code=vector_init(3), *code1;
    code_ret*code_s;
    code_type *ct1;
    obj_type type1;
    ast * pair_ast;
    int i;

    if (pair_list_ast->table == NULL) {                                     // nullvectorの場合
        push(code,(void*)DIC);push(code,(void*)0);                          // ->[ DIC 0 ]
        return new_code(code,new_ct(OBJ_DICT,OBJ_NONE,(void*)0,FALSE));
    } 
    int n = pair_list_ast->table->_sp;
    for(i = 0; i < n; i ++ ) {
        pair_ast = (ast*)vector_ref(pair_list_ast->table, i);
        code_s = codegen((ast*)vector_ref(pair_ast->table, 0),env, FALSE);  // code of left_pair
        code1 = code_s->code;
        ct1 = code_s->ct;
        type1 = ct1->type;
        if (type1 != OBJ_SYM) push(code1,(void*)conv_op[type1][OBJ_KEY]);
        code=vector_append(code,code1);

        code_s = codegen((ast*)vector_ref(pair_ast->table, 1),env, FALSE);  // code of right_pair
        code1 = code_s->code;
        ct1 = code_s->ct;
        type1 = ct1->type;
        if (type1 != OBJ_GEN) push(code1,(void*)conv_op[type1][OBJ_GEN]);
        code=vector_append(code,code1);
    }
    push(code,(void*)DIC);push(code,(void*)(long)n);
    return new_code(code,new_ct(OBJ_DICT,OBJ_GEN,(void*)0,FALSE));
}

code_ret *codegen_lit(ast*lit_ast,Vector*env,int tail) {
    // リテラルタイプのastを処理し、コードを作る 
    tokentype lit_type=(tokentype)vector_ref(lit_ast->table,0);
    Symbol*str_symbol=(Symbol*)vector_ref(lit_ast->table,1);
    Vector*code = vector_init(3);
    long int_num;double*fl_num_p;
    mpz_ptr z;mpq_ptr q;mpfr_ptr lf;
    //char**test_buf=(char**)malloc(1024*sizeof(char));
    push(code,(void*)LDC);
    switch(lit_type) {
        case TOKEN_NONE:
            push(code,(void*)0);
            return new_code(code,new_ct(OBJ_NONE,OBJ_NONE,(void*)0,FALSE));
        case TOKEN_INT:
            sscanf(str_symbol->_table,"%ld",&int_num);
            push(code,(void*)int_num);
            return new_code(code,new_ct(OBJ_INT,OBJ_NONE,(void*)0,FALSE));
        case TOKEN_LINT:
            z = (mpz_ptr)malloc(sizeof(MP_INT));
            mpz_set_str(z,str_symbol->_table,10);
            push(code,(void*)z);
            return new_code(code,new_ct(OBJ_LINT,OBJ_NONE,(void*)0,FALSE));
        case TOKEN_RAT:
            q = (mpq_ptr)malloc(sizeof(MP_RAT));
            mpq_init(q);
            if (mpq_set_str(q,str_symbol->_table,10) !=0 || mpz_sgn(mpq_denref(q))==0) {printf("SyntaxError:IllegalRationalNumber!\n");Throw(0);}
            mpq_canonicalize(q);
            push(code,(void*)q);
            return new_code(code,new_ct(OBJ_RAT,OBJ_NONE,(void*)0,FALSE));
        case TOKEN_STR:
            push(code,(void*)vector_ref(lit_ast->table,1));
            return new_code(code,new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));
        case TOKEN_FLT: 
            fl_num_p=(double*)malloc(sizeof(double));
            sscanf(str_symbol->_table,"%lf",fl_num_p);
            push(code,(void*)*((long*)fl_num_p));
            return new_code(code,new_ct(OBJ_FLT,OBJ_NONE,(void*)0,FALSE));
        //case TOKEN_FLT:
        //    fl_num_p=(double*)malloc(sizeof(double));
        //    sscanf(str_symbol->_table,"%lf",fl_num_p);
        //    push(code,(void*)fl_num_p);
        //    return new_code(code,new_ct(OBJ_FLT,OBJ_NONE,(void*)0,FALSE));
        case TOKEN_EFLT:
            fl_num_p=(double*)malloc(sizeof(double));
            sscanf(str_symbol->_table,"%le",fl_num_p);
            //push(code,(void*)fl_num_p);
            push(code,(void*)*((long*)fl_num_p));
            return new_code(code,new_ct(OBJ_FLT,OBJ_NONE,(void*)0,FALSE));
        case TOKEN_LEFLT:
            //char**testbuf=(char**)malloc(1024*sizeof(char));
            lf=(mpfr_ptr)malloc(sizeof(__mpfr_struct));
            mpfr_init_set_str(lf, str_symbol->_table, 10,MPFR_RNDA);
            //mpfr_strtofr(lf,str_symbol->_table,test_buf,10,MPFR_RNDA);
            push(code,(void*)lf);
            return new_code(code,new_ct(OBJ_LFLT,OBJ_NONE,(void*)0,FALSE));
        case TOKEN_HEX:
            if (str_symbol->_size>16) { // long int
                z = (mpz_ptr)malloc(sizeof(MP_INT));
                mpz_set_str(z,str_symbol->_table,16);
                push(code,(void*)z);//printf("%s\n",objtype2str(OBJ_LINT,(void*)z));
                return new_code(code,new_ct(OBJ_LINT,OBJ_NONE,(void*)0,FALSE));
            } else {
                sscanf(str_symbol->_table,"%lx",&int_num);
                push(code,(void*)int_num);
                return new_code(code,new_ct(OBJ_INT,OBJ_NONE,(void*)0,FALSE));
            }
        case TOKEN_OCT:
            if (str_symbol->_size>21) { // long int
                z = (mpz_ptr)malloc(sizeof(MP_INT));
                mpz_set_str(z,str_symbol->_table,8);
                push(code,(void*)z);//printf("%s\n",objtype2str(OBJ_LINT,(void*)z));
                return new_code(code,new_ct(OBJ_LINT,OBJ_NONE,(void*)0,FALSE));
            } else {
                sscanf(str_symbol->_table,"%lo",&int_num);
                push(code,(void*)int_num);
                return new_code(code,new_ct(OBJ_INT,OBJ_NONE,(void*)0,FALSE));
            }
        case TOKEN_BIN:
            z = (mpz_ptr)malloc(sizeof(MP_INT));
            mpz_set_str(z,str_symbol->_table,2);
            if (str_symbol->_size>64) {
                push(code,(void*)z);//printf("%s\n",objtype2str(OBJ_LINT,(void*)z));
                return new_code(code,new_ct(OBJ_LINT,OBJ_NONE,(void*)0,FALSE));
            } else {
                push(code,(void*)mpz_get_ui(z));
                return new_code(code,new_ct(OBJ_INT,OBJ_NONE,(void*)0,FALSE));
            }
        case TOKEN_CHR:
        default: printf("Unknown Token!\n");Throw(2);
    }

}

code_ret* codegen_var(ast* var_ast,Vector*env,int tail) {
    Vector*code=vector_init(3);
    Symbol*s=(Symbol*)vector_ref(var_ast->table,0);  // s:var name
    code_type*ct;
    s =(Symbol*)vector_ref(var_ast->table, 0);
    // macroにあるか…後で
    // システム定義変数関数か…後で
    // primitive functionか？
    if (Hash_get(PRIMITIVE_FUNC,s) != NULL) {
        ct=(code_type*)*Hash_get(PRIMITIVE_FUNC,s);  
        push(code,(void*)LDG);push(code,(void*)vector_ref(var_ast->table,0));
        return new_code(code,ct);
    }
    // ローカル変数かチェック
    Vector*_pos=var_location(s,env);//PR(3333);// printf("var_location OK!!\n");
    if (_pos) { // ローカル変数である
        Vector*pos=(Vector*)vector_ref(_pos,0);ct=(code_type*)vector_ref(_pos,1);
        if ((long)vector_ref(pos, 0) == 0) {
            switch((long)vector_ref(pos, 1)) {
                case 0: push(code, (void * )LD00); break; 
                case 1: push(code, (void * )LD01); break; 
                case 2: push(code, (void * )LD02); break; 
                case 3: push(code, (void * )LD03); break; 
                default:push(code,(void*)LD);push(code,(void*)pos);
            }
        } else if ((long)vector_ref(pos, 0) == 1) {
            switch((long)vector_ref(pos, 1)) {
                case 0: push(code, (void * )LD10); break; 
                case 1: push(code, (void * )LD11); break; 
                case 2: push(code, (void * )LD12); break; 
                case 3: push(code, (void * )LD13); break; 
                default:push(code,(void*)LD);push(code,(void*)pos);
            }
        } else{
            push(code,(void*)LD);push(code,(void*)pos);//disassy(code,0,stdout);
        }
    //} else if 
    } else {    // 大域変数の場合
        s=(Symbol*)vector_ref(var_ast->table,0);
        if (get_gv(s) == NULL) {printf("SyntaxError :Global value <%s> not defined!\n",s->_table);Throw(0);}
        ct=get_gv(s);
        push(code,(void*)LDG);push(code,(void*)vector_ref(var_ast->table,0));
    } //disassy(code,0,stdout);//PR(6);
    // ... constant macro ...
    return new_code(code,ct);

}

void * p_array_ref(Vector * v) {
    array * a = (array*)vector_ref(v,0);
    int dim = v->_sp - 1;
    int index = (int)(long)vector_ref(v, 1); 
    if (index > a->sizes[0]) {printf("IndexError!\n");Throw(3);}
    int j;complex *c;
    for (int i = 1; i < dim; i++) {
        if ((j = (int)(long)vector_ref(v, i + 1)) > a->sizes[i]) {printf("IndexError!\n");Throw(3);}
        index = (index * a -> sizes[i]) + j; 
    }
    if (a->type == OBJ_CMPLX) return  &a->table._cmplx;
    return (void *)a->table._ptr[index];
}

code_ret *codegen_vref(ast *vref_ast, Vector*env, int tail) {  // AST_VREF [AST_vect, AST_expr_list[i1,i2,...]]
                //           <0>                     <1,0>,<1,1>,
    code_ret *code_vect = codegen(((ast*)vector_ref(vref_ast->table,0)), env,FALSE);    // vector指示部をコンパイル
    Vector *code = code_vect->code; 
    code_type *ct = code_vect->ct;
    obj_type type1=ct->type;
    // arrayの処理
    Vector * index_vect;
    //Funcpointer fp;
    if (type1 == OBJ_ARRAY) {                                                               // arrayのrefはgeneraruにしてから返すように変更すべし(現状float決め打ちなので)
        int array_dim = (index_vect=((ast*)vector_ref(vref_ast->table, 1))->table)->_sp;
        for(int i = 0; i < array_dim ; i++) {
            code_ret *code_ref = codegen((ast*)vector_ref(index_vect, i), env, FALSE);
            code = vector_append(code, code_ref->code);
            if (code_ref->ct->type != OBJ_INT) push(code, (void*)conv_op[code_ref->ct->type][OBJ_INT]);
        }
        Vector *func_vect = vector_init(3);push(func_vect, (void*)FUNC_PRIM);push(func_vect,(void*)p_array_ref);
        push(code, (void*)LDC); push(code, (void *)func_vect); push(code, (void *)PCALL); push(code, (void *)(long)(array_dim + 1));
        return new_code(code,new_ct(OBJ_FLT,OBJ_NONE,(void*)0,FALSE));  // !!!!!!!arrayの要素はfloatと決めつけているが…
    }

    // array以外
    if (type1 != OBJ_VECT && type1 != OBJ_SYM && type1 != OBJ_GEN && type1 != OBJ_DICT) {printf("Syntax Error:must be vector/String/Dictionary/Array!\n");Throw(0);}
    code_ret *code_ref = codegen((ast*)vector_ref(((ast*)vector_ref(vref_ast->table,1))->table,0), env,FALSE);
    code = vector_append(code, code_ref->code);
    if ((type1 == OBJ_DICT) && (code_ref->ct->type != OBJ_SYM)) push(code, (void*)conv_op[code_ref->ct->type][OBJ_KEY]);  
    if ((type1 != OBJ_DICT) && (code_ref->ct->type != OBJ_INT)) push(code, (void*)conv_op[code_ref->ct->type][OBJ_INT]);
    //
    if (type1==OBJ_VECT) {
        push(code,(void*)REF);
        return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
    } else if (type1==OBJ_SYM) {
        push(code,(void*)SREF);
        return new_code(code,new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));
    } else if (type1==OBJ_DICT) {
        push(code,(void*)LDH);
        return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
    } else {
        push(code,(void*)OREF);
        return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
    }
}
/*
code_ret*codegen_sls(ast*sls_ast, Vector*env, int tail) {   // AST_SLS [AST_vect,AST_PAIR_LIST [AST_PAIR[car_ast,cdr_ast],AST_PAIR[...],...]]
                                                            //          <0>      <1>            <1,0>    <1,0,0> <1,0,1>          <1,1>
    code_ret *code_vect = codegen(((ast*)vector_ref(sls_ast->table,0)), env,FALSE);    // vector指示部をコンパイル
    Vector *code = code_vect->code; 
    code_type *ct=code_vect->ct;
    obj_type type1 = ct->type;
    if (type1 != OBJ_VECT && type1 != OBJ_SYM && type1 != OBJ_GEN) {printf("Syntax Error:must be vector!\n");Throw(0);}
    code_ret *code_sls_s = codegen((ast*)vector_ref(((ast*)vector_ref(((ast*)vector_ref(sls_ast->table,1))->table,0))->table,0), env,FALSE);
    code = vector_append(code,code_sls_s->code);
    if (code_sls_s->ct->type != OBJ_INT) push(code,(void*)conv_op[code_sls_s->ct->type][OBJ_INT]);
    code_ret *code_sls_e = codegen((ast*)vector_ref(((ast*)vector_ref(((ast*)vector_ref(sls_ast->table,1))->table,0))->table,1), env,FALSE);
    code=vector_append(code,code_sls_e->code);
    if (code_sls_e->ct->type != OBJ_INT) push(code,(void*)conv_op[code_sls_e->ct->type][OBJ_INT]);
    //disassy(code,0,stdout);//ok
    if (type1==OBJ_VECT) {
        push(code,(void*)VSLS);
        return new_code(code,new_ct(OBJ_VECT,OBJ_NONE,(void*)0,FALSE));
    } else if (type1==OBJ_SYM) {
        push(code,(void*)SSLS);
        return new_code(code,new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));
    } else {
        push(code,(void*)OSLS);
        return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
    }
}
*/
code_ret*codegen_sls(ast*sls_ast, Vector*env, int tail) {   // AST_SLS [AST_vect,AST_PAIR_LIST [AST_PAIR[car_ast,cdr_ast],AST_PAIR[...],...]]
                                                            //          <0>      <1>            <1,0>    <1,0,0> <1,0,1>          <1,1>
    ast * vect_ast=(ast*)vector_ref(sls_ast->table,0);
    ast * pair_list_ast = (ast*)vector_ref(sls_ast->table,1);
    ast * pair_ast = (ast*)vector_ref(pair_list_ast->table,0);
    ast * sls_start_ast = (ast*)vector_ref(pair_ast->table,0);
    ast * sls_end_ast = (ast*)vector_ref(pair_ast->table,1);

    code_ret *code_vect = codegen(vect_ast, env,FALSE);    // vector指示部をコンパイル
    code_type *vect_ct = code_vect->ct;
    obj_type vect_type = vect_ct->type;
    if (vect_type != OBJ_VECT && vect_type != OBJ_SYM && vect_type != OBJ_GEN) {printf("Syntax Error:must be vector!\n");Throw(0);}

    Vector *code = code_vect->code;

    code_ret * sls_s_code, * sls_e_code;
    // make slice start code
    if (sls_start_ast != NULL) {
        sls_s_code = codegen(sls_start_ast, env,FALSE);
        code = vector_append(code, sls_s_code->code);
        if (sls_s_code->ct->type != OBJ_INT) push(code,(void*)conv_op[sls_s_code->ct->type][OBJ_INT]);
    }
    // make slice end code
    if (sls_end_ast != NULL) {
        sls_e_code = codegen(sls_end_ast, env,FALSE);
        code = vector_append(code, sls_e_code->code);
        if (sls_e_code->ct->type != OBJ_INT) push(code,(void*)conv_op[sls_e_code->ct->type][OBJ_INT]);
    }
    //disassy(code,0,stdout);//ok
    if (sls_start_ast) {
        if (sls_end_ast) {
            if (vect_type == OBJ_VECT) {push(code, (void*)VSLS);return new_code(code,new_ct(OBJ_VECT,OBJ_NONE,(void*)0,FALSE));}
            else if (vect_type == OBJ_SYM) {push(code, (void*)SSLS);return new_code(code,new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));}
            else {push(code, (void*)OSLS); return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));}
        } else {
            if (vect_type == OBJ_VECT) {push(code, (void*)VSLS_);return new_code(code,new_ct(OBJ_VECT,OBJ_NONE,(void*)0,FALSE));}
            else if (vect_type == OBJ_SYM) {push(code, (void*)SSLS_);return new_code(code,new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));}
            else {push(code, (void*)OSLS_);return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));}
        }
    } else {
        if (sls_end_ast) {
            if (vect_type == OBJ_VECT) {push(code, (void*)V_SLS);return new_code(code,new_ct(OBJ_VECT,OBJ_NONE,(void*)0,FALSE));}
            else if (vect_type == OBJ_SYM) {push(code, (void*)S_SLS);return new_code(code,new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));}
            else {push(code, (void*)O_SLS);return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));} 
        }
    }
}

void * aa_mul(Vector * v) {return (void*)array_array_mul((array*)vector_ref(v,0), (array*)vector_ref(v,1));}
void * as_mul(Vector * v) {return (void*)array_scler_mull((array*)vector_ref(v,0), vector_ref(v,1));}
//void * as_div(Vector * v) {return (void*)array_scler_mull((array*)vector_ref(v,0), vector_ref(v,1));}
void * sa_mul(Vector * v) {return (void*)array_scler_mull((array*)vector_ref(v,1), vector_ref(v,0));}
void * aa_add(Vector * v) {return (void*)array_array_add((array*)vector_ref(v, 0), (array*)vector_ref(v, 1));}

code_ret *codegen_2op(ast * _2op_ast, Vector *env, int tail) {  // AST_2OP [op_type,AST_L_EXPR,AST_R_EXPR]
    obj_type r_type;
    int i,op_code;
    Funcpointer fp; Vector * func_vect ;                        // for array operator

    Vector *code = vector_init(3);
    code_ret *code_r_left = codegen((ast*)vector_ref(_2op_ast->table,1),env,FALSE);   //左辺式をコンパイルする
    Vector *code_left = code_r_left->code;                      // 左辺式のコード
    code_type *ct_left = code_r_left->ct;                       // 左辺式のコードタイプ
    obj_type type_left = ct_left->type;                         // 左辺式の型
    //
    code_ret *code_r_right = codegen((ast*)vector_ref(_2op_ast->table,2),env,FALSE);   //右辺式をコンパイルする
    Vector *code_right = code_r_right->code;                    // 右辺式のコード
    code_type *ct_right = code_r_right->ct;                     // 右辺式のコードタイプ
    obj_type type_right = ct_right->type;                       // 右辺式の型
    //
    tokentype lit_type=(int)(long)vector_ref(_2op_ast->table,0);       //2項演算子
    //特別な場合を先に処理
    // 論理or、論理and IF式に変換する
    // A || B -> if A : true : B      A && B -> if A : B : false
    if (lit_type == '|'*256+'|' || lit_type == '&'*256+'&') {
        ast *ast_left = (ast*)vector_ref(_2op_ast->table,1);
        ast *ast_right = (ast*)vector_ref(_2op_ast->table,2);
        Vector *vv=vector_init(3);
        push(vv,(void*)TOKEN_INT);push(vv,(void*)new_symbol("1",1));
        ast *ast_true=new_ast(AST_LIT,OBJ_INT,vv);
        vv=vector_init(3);
        push(vv,(void*)TOKEN_INT);push(vv,(void*)new_symbol("0",1));
        ast *ast_false=new_ast(AST_LIT,OBJ_INT,vv);

        Vector *v=vector_init(3);
        if (lit_type=='|'*256+'|') {
            push(v,(void*)ast_left);push(v,(void*)ast_true);push(v,(void*)ast_right);
        } else {
            push(v,(void*)ast_left);push(v,(void*)ast_right);push(v,(void*)ast_false);
        }
        return codegen(new_ast(AST_IF,OBJ_NONE,v),env,tail);
    }
    //vecter、stringの乗算
    if ((type_left == OBJ_VECT || type_left == OBJ_SYM ) && lit_type=='*' ) {
        if (type_right != OBJ_INT)  push(code_right,(void*)conv_op[type_right][OBJ_INT]);
        r_type=type_left;
        code = vector_append(code_left, code_right);
        push(code,type_left==OBJ_VECT ? (void*)VMUL : (void*)SMUL);
        return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
    }
    // array 演算
    if (type_left == OBJ_ARRAY || type_right == OBJ_ARRAY) {
        switch(lit_type) {
            case '*':
                if (type_left == OBJ_ARRAY && type_right == OBJ_ARRAY) fp = aa_mul;
                else if (type_left == OBJ_FLT) fp = sa_mul;
                else if (type_right == OBJ_FLT) fp = as_mul;
                else {printf("IllegalOprand!\n");Throw(3);}
                break;
            case '+':
                if (type_left == OBJ_ARRAY && type_right == OBJ_ARRAY) fp = aa_add;
                else {printf("IllegalOprand!\n");Throw(3);}
                break;
            default :printf("IllegalOprator!\n");Throw(3);
            //case '/'
        }
        func_vect = vector_init(3);push(func_vect, (void*)FUNC_PRIM);push(func_vect,(void*)fp);
        r_type = OBJ_ARRAY;
        code = vector_append(code_left, code_right);
        push(code, (void*)LDC); push(code ,(void*)func_vect); push(code, (void*)PCALL); push(code, (void*)2);
        return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
    
    // vector、generalへのpush
    } else if ((type_left == OBJ_VECT || type_left == OBJ_GEN ) && lit_type == '<'*256+'-' ) {
        if (type_right != OBJ_GEN) push(code_right,(void*)conv_op[type_right][OBJ_GEN]);
        //r_type=OBJ_GEN;
        r_type=type_left;
        code = vector_append(code_left, code_right);
        push(code, (void*)(long)(type_left == OBJ_VECT ? VPUSH : OPUSH));
        return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
    // シフト命令
    } else if (lit_type=='>'*256+'>' || lit_type == '<'*256+'<') {
        r_type=type_left;
        if (type_right != OBJ_INT) push(code_right,(void*)conv_op[type_right][OBJ_INT]);
    // 累乗命令
    //      2,3,4乗は特別に
    //      1/2,1/3,1/4乗も特別
    // 加減算命令
    //      ±2までは特別に
    // その他一般の場合
    } else if (type_left < type_right) {
        op_code = conv_op[type_left][type_right];
        if (op_code ==0 ) {printf("SyntaxError:IllegalOpecode!000\n");Throw(0);}
        push(code_left,(void*)(long)op_code);
        r_type=type_right;
    } else if (type_left > type_right) {
        op_code = conv_op[type_right][type_left];
        if (op_code ==0 ) {printf("SyntaxError:IllegalOpecode!001 %d %d\n",type_right,type_left);ast_print(_2op_ast,0);Throw(0);}
        push(code_right,(void*)(long)op_code);
        r_type=type_left;
    } else r_type=type_left;
    //
    code=vector_append(code_left,code_right);//disassy(code,0,stdin);
    for(i=0;i<=17;i++) {
        if (op2_1[i]==(int)(long)vector_ref(_2op_ast->table,0)) break;
    }
    if (i>=18) {printf("illegal 2oprand\n");return NULL;}
    if ((op_code =op2_2[r_type][i])==0) {printf("SyntaxError:IllegalOpecode!002\n");Throw(0);};
    push(code,(void*)(long)op_code);
    //
    if (op2_3[i] != 0) r_type=op2_3[i];
    // printf("ret_type:%d\n",ret_obj);
    return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
}

code_ret *codegen_1op(ast *_1op_ast, Vector * env, int tail) { // AST_1OP [op_type,AST_EXPR]
    enum CODE opcode;
    int i;
    code_ret *code_s = codegen((ast*)vector_ref(_1op_ast->table,1),env,FALSE);
    Vector *code = code_s->code; obj_type r_type=code_s->ct->type;
    tokentype litpyte = (int)(long)vector_ref(_1op_ast->table,0);
    for(i=0;i<6;i++) {
        if (op1_1[i]==(int)(long)vector_ref(_1op_ast->table,0)) break;
    }
    if (i>=6){printf("illegal 1oprand\n");return NULL;}
    if ((opcode=op1_2[r_type][i])==0) {printf("syntax Error:operator is not supported\n");Throw(0);}
    push(code,(void*)(long)opcode);
    if ((op1_1[i]=='-'*256+'>') && (r_type==OBJ_SYM)) r_type=OBJ_SYM;
    else if ((op1_1[i] == '@') && (r_type == OBJ_ARRAY)) r_type = OBJ_VECT;
    else if (op1_3[i] != 0) r_type=op1_3[i];
    return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
}

code_ret *codegen_apply(ast *apply_ast, Vector *env, int tail) {    // AST_APPLY [AST_EXP_LIST [ast1, ast2, ...]]
                                                                    //            <0>           <0,0>,<0,1e>,...
    int i; obj_type r_type,dotted;
    int n=((ast*)vector_ref(apply_ast->table,0))->table->_sp;//printf("%d\n",n);
    Vector *code = vector_init(3), *arg_type;

    for(i = 0; i < n; i ++ ) {
        code_ret *code_s = codegen((ast*)vector_ref(((ast*)vector_ref(apply_ast->table, 0))->table, i),env,FALSE);//disassy(code,0);
        Vector *code_i = code_s->code;
        code_type *ct_i=code_s->ct;
        obj_type type_i = ct_i->type;
        //
        if (i==0) {                         // 最初の引数は関数でなければならない
            if (type_i != OBJ_UFUNC && type_i != OBJ_PFUNC) {printf("SyntaxError:Must be Function!\n");Throw(0);}
                r_type = ct_i->functon_ret_type;    // 関数を返す関数の場合どうする？
        } else if (i == n-1) {              //最後の引数はtypeがvectorであることを確認しそのままcodeにする
            if ( type_i != OBJ_VECT) {printf("SyntaxError:Must be Vector!\n");Throw(0);}
        } else if (type_i != OBJ_GEN) {     //最初でも最後でもなければtypeを汎用型に変換 ※applyの因数は関数と汎用型とvector
            push(code_i,(void*)conv_op[type_i][OBJ_GEN]);
        }
        code = vector_append(code, code_i);
    }
    if (tail)  {push(code, (void*)TAPL);push(code, (void*)(long)n);}
    else       {push(code, (void*)APL); push(code, (void*)(long)n);}
    
    return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
}

char *sys_func_name[] =     {"toint","tolong","torat","tofloat","tolfloat","tocmplex","tostr","togeneral",  // 変換関数
                             "abs","sqrt","cbrt","num","den","imag","real","conj",                          // 数値処理
                             "copy",NULL};
int sys_func_code[][10] = {//int    long    rat     float   lfloat  complex 
                            {0,     LTOI,   RTOI,   FTOI,   LFTOI,  0,      STOI,   OTOI},      // toint
                            {ITOL,  0,      RTOL,   FTOL,   LFTOL,  0,      STOL,   OTOL},      // tolong
                            {ITOR,  LTOR,   0,      FTOR,   LFTOR,  0,      STOR,   OTOR},      // torational
                            {ITOF,  LTOF,   RTOF,   0,      LFTOF,  0,      STOF,   OTOLF},     // tofloat
                            {ITOLF, LTOLF,  RTOLF,  FTOLF,  0,      0,      STOLF,  OTOLF},     // tolfloat
                            {0,     0,      0,      0,      0,      0,      STOC,   OTOC},      // tocmplex
                            {ITOS,  LTOS,   RTOS,   FTOS,   LFTOS,  CTOS,   0,      OTOS},      // tostr
                            {ITOO,  LTOO,   RTOO,   FTOO,   LFTOO,  CTOO,   STOO,   0   },      // togeneral
                          //{IABS,  LABS,   RABS,   FABS,   LFABS,  CABS,   0,      OABS},      // abs
                          //{ISQRT, LSQRT,  RSQRT,  FSQRT,  LFSQRT, CSQRT,  0,      OSQRT},     // sqrt
                          //{ICBRT, LCBRT,  RCBRT,  FCBRT,  LFCBRT, CCBRT,  0,      OCBRT},     // cbrt
                          //{0,     0,      num,    0,      0,      0,      0,      ONUM},      // num
                          //{0,     0,      den,    0,      0,      0,      0,      ODEN},      // den
                          //{0,     0,      0,      0,      0,      IMAG,   0,      OIMAG}.     // imag
                          //{0,     0,      0,      0,      0,      REAL    0,      OREAL},     // real
                          //{0,     0,      0,      0,      0,      CONJ,   0,      OCONJ},     // conj
                          } ;

code_ret *codegen_fcall(ast *fcall_ast, Vector * env, int tail) {  // AST_FCALL [AST_NAME,[AST_EXP_LIST [AST,AST,...]]]
                                                                    //            <0>                     <1,0>,<1,1>...
    // ... macro function ...
    Vector *v1,*v2;
    ast *param_ast = (ast*)vector_ref(fcall_ast->table,1);          // parameter ast
    int i, n = param_ast->table->_sp, m, t_op;                                  // number of actual parameters
    ast *function_ast = (ast*)vector_ref(fcall_ast->table,0);       // function ast
    // 「..」だったら即APPLYに変換する
    if (param_ast->type==AST_EXP_LIST_DOTS || param_ast->type==AST_ARG_LIST_DOTS) {                      // if expr_list_dots -> apply
        v1=vector_init(1 + param_ast->table->_sp);
        //push(v1,(void*)vector_ref(a->table,0));
        push(v1,(void*)function_ast);
        for(i = 0; i< n; i ++) {
            push(v1,(void*)vector_ref(param_ast->table,i));
        }
        v2=vector_init(1);
        push(v2,(void*)new_ast(AST_EXP_LIST_DOTS,((ast*)vector_ref(param_ast->table,n-1))->o_type,v1));
        return codegen(new_ast(AST_APPLY,((ast*)vector_ref(param_ast->table,n-1))->o_type,v2),env,tail);     // AST_APPLY [AST_EXP_LIST [expr0,expr1,...]]
    }
    // 一般の場合
    // function astのコード生成
    Vector *code=vector_init(3);
    code_ret *code_s_function = codegen(function_ast,env,FALSE);                        // 関数名部分をコンパイル
    Vector *code_function = code_s_function->code;//PR(0);            
    if (code_s_function->ct->type != OBJ_UFUNC && code_s_function->ct->type != OBJ_PFUNC) { printf("SyntaxError:Must be Function!\n");Throw(0);}
    obj_type r_type = code_s_function->ct->functon_ret_type;//PR(1);                    // 関数の戻り型
    Vector *v = code_s_function->ct->arg_type;//vector_print(v);PR(2);                  // 引数の型のリスト

    int doted;
    code_ret *code_s_param; 
    Vector *code_param;                 // 引数要素のコード
    code_type *ct_param;                // 引数要素の型構造体
    obj_type type_param,type_dummy;     // 実引数要素の型、仮引数要素の型
    // 実引数を展開する
    //if (r_type != OBJ_NONE) {
    if (code_s_function->ct->arg_type != NULL) {           // arg_typeがNULLでないこと！
        m = v->_sp;//PR(2);                             // 仮引数の個数　number of dummy parameters
        //n = a1->table->_sp;//PR(3);                   // 実引数の個数　number of actual parameters
        if (!(doted=code_s_function->ct->dotted) && n != m) { printf("SyntaxError: Illegal parameter number! d=%d a= %d\n",m,n);Throw(0);}
        for(i = 0; i < n; i ++ ) {//PR(i);
            code_s_param = codegen((ast*)vector_ref(param_ast->table, i),env,FALSE);
            code_param = code_s_param->code; ct_param = code_s_param->ct; type_param = ct_param->type; // ct1/type1:actual parameter type
            if (doted && i >= m-1) type_dummy=OBJ_GEN ; else type_dummy = (int)(long)vector_ref(v,i);  // ct2/type2:dummy parameter type
            if ((type_param != type_dummy ) && (type_param != OBJ_NONE)) {
                if ((t_op=conv_op[type_param][type_dummy])==0) {printf("SyntaxError:IllegalArgmentType!\n");Throw(0);}
                if (t_op != -1) {push(code_param,(void*)conv_op[type_param][type_dummy]);}
            }
            // type_param,type_dummyがfunctionの場合はそのret_typeが同じかどうか確かめたいが、現状引数である関数のパラメータ詳細を保持していない
            // (CT->arg_typeは型名しかもたずfunction型であることはわかっても戻り値等不明なため)
            // 現状仮引数に書いた関数を実引数に書いた関数をきっちり一致させるのはユーザ責任！<=修正が必要
            //
            //
            code=vector_append(code,code_param);
        }

        code = vector_append(code, code_function);      // 展開した実印数のコードに関数名コードを追加append Function name % expr_list
        if (code_s_function->ct->type == OBJ_SYSFUNC) { // sys funcの場合はコードは完成している
           ;// push(code,(void*))
        } else if (code_s_function->ct->type == OBJ_PFUNC) {
            push(code, (void*)PCALL);
            push(code, (void*)(long)n);
        } else {
            push(code, tail ? (void*)TCALL : (void*)CALL);
            push(code, (void*)(long)n);
        }
        return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
    } else { // arg_typeが設定されていない≒詳細未定義の関数→再起関数の場合は型情報は後で（関数セット時に）入れる
             //                                           →高階関数の引数の場合は型はすべてgenとするしかない
             //                                            ※再起関数か高階関数の引数かを判定しなければならない!!!
        //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        v=vector_init(3);
        for(i=0;i<n;i++){
            code_s_param = codegen((ast*)vector_ref(param_ast->table,i),env,FALSE);
            code_param = code_s_param->code;ct_param=code_s_param->ct;type_param=ct_param->type;                            // ct1/type1:actual parameter type
            //if (dot && i >= m-1) type2=OBJ_GEN ; else type2 = (int)(long)vector_ref(v,i);  // ct2/type2:dummy parameter type
            //if (type1 != type2) push(code1,(void*)conv_op[type1][type2]);
            push(v,(void*)type_param);
            code=vector_append(code,code_param);
        }
        code = vector_append(code, code_function);    // append Function name % expr_list
        push(code, tail ? (void*)TCALL : (void*)CALL);
        push(code, (void*)(long)n);
        return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));

    }
}
code_ret *codegen_cl_var(ast * ast_cl_var, Vector * env, int tail) {
    // AST_CL_VAR [left_ast, right_astn]
}

code_ret *codegen_dcl(ast *dcl_ast, Vector *env, int tail) {                        // AST_DCL [AST_EXPR_LIST [exp1,exp2...]]
                                                                                    //          <0>            <0,0>,<0,1>
    int j,i,dotted;
    Vector *code = vector_init(3);                                                                   
    ast *ast_j, *ast_i,*fcall_ast, *arglist_ast, *f_name_ast;
    code_ret *code_s_right, *code_s;
    code_type *ct;
    Symbol *s;

    for(j = 0; j < ((ast*)vector_ref(dcl_ast->table, 0))->table->_sp; j++) {        // declear内のexpr_listを順番に見ていき
        ast_j = (ast*)vector_ref(((ast*)vector_ref(dcl_ast->table,0))->table,j);    // ast_j:宣言式のj番目の式
        if (ast_j->type==AST_VAR) {                                                 // int I,J,K;等初期代入がない場合
            if (get_gv(s=(Symbol*)vector_ref(ast_j->table,0))!=NULL) {printf("Warning!:DupricateDifinition!\n");}  //警告を出して続行
            // printf("!!!!\n");
            ct=new_ct(dcl_ast->o_type,OBJ_NONE,(void*)0, dcl_ast->o_type == OBJ_UFUNC ? TRUE : FALSE);                     // 宣言された型でctを作り
            put_gv(s,ct);                                                               // 型をgvに登録する
            push(code,(void*)LDC);push(code,create_zero(dcl_ast->o_type));              // 「0」で初期化しておく
            push(code,(void*)GSET);push(code,(void*)s);push(code,(void*)DROP);          // declear式は値を返さない;
        } else if (ast_j->type==AST_SET) {                                              // a2: AST_SET [set_type, AST_VAR [var_name], expr_ast]
            if (((ast*)vector_ref(ast_j->table,1))->type==AST_VAR) {                    //                        <1>                 <2>
                //変数名を取り出してSymbol*sに取っておき、定義済(GVにある)なら警告
                s = (Symbol*)vector_ref(((ast*)vector_ref(ast_j->table,1))->table,0);
                if (get_gv(s) !=0) printf("Warning!:DupricateDifinition!\n");           //警告を出して続行
                // 右辺式のコードを生成
                code_s_right = codegen((ast*)vector_ref(ast_j->table,2),env,FALSE);
                code = vector_append(code, (void*)code_s_right->code);ct=code_s_right->ct;
                // 変数名sに型を設定する
                // ここにおいて右辺値の型はct、dcl宣言された型はdcl_ast->o_typeである
                if (dcl_ast->o_type==OBJ_UFUNC || dcl_ast->o_type==OBJ_PFUNC) {         //型宣言詞がfunctionの場合  =>=> この構文は削除予定!!
                    if (ct->type==OBJ_UFUNC || ct->type==OBJ_PFUNC) {                   //右辺値もfunctionなら正常
                        put_gv(s,ct);                                                   //そのまま変数名に割り当てる
                    } else {
                        printf("SyntaxError:Must be Function!\n");                      // 左辺値がfunctionなのに右辺値がそうでないならエラー
                        Throw(0) ;
                    }
                } else {                                                                //型宣言が関数タイプでないときは
                    put_gv(s,new_ct(dcl_ast->o_type,OBJ_NONE,(void*)0,FALSE));          //宣言された型を変数名sに割り当てるが
                    if (dcl_ast->o_type != ct->type) {                                  //右辺値の型と宣言型が異なる場合には型変換命令を入れる
                        push(code, (void*)conv_op[ct->type][dcl_ast->o_type]);
                        ct->type=dcl_ast->o_type;                                       //返す型は型返還後のもの、すなわち宣言型する
                    }
                }
                push(code,(void*)GSET);push(code,(void*)s);
                push(code,(void*)DROP);
            } else if ((fcall_ast=(ast*)vector_ref(ast_j->table,1))->type==AST_FCALL) { //  左辺式がAST_CALL即ち関数宣言
                arglist_ast = (ast*)vector_ref(fcall_ast->table,1);
                f_name_ast = (ast*)vector_ref(fcall_ast->table,0);
                if (f_name_ast->type != AST_VAR) {printf("SyntaxError:IllegalFunctionName!\n");Throw(1);}
                Vector *d_args = make_arg_list_type(arglist_ast);
                if (arglist_ast->type ==  AST_ARG_LIST_DOTS ||arglist_ast->type ==  AST_EXP_LIST_DOTS) dotted=TRUE; else dotted=FALSE;
                //
                s=(Symbol*)vector_ref(f_name_ast->table,0);                                 // sは関数名
                ct=new_ct(OBJ_UFUNC, dcl_ast->o_type,(Vector*)vector_ref(d_args,1),dotted); // CTを作り
                put_gv(s,ct);                                                               // sにセット
                code_s = codegen_set(ast_j,env,tail);                                       // ast_jは型宣言詞抜きのset astなので型情報がセットされた上でcode化
                code = code_s->code;//disassy(code,0,stdout);
                push(code,(void*)DROP);                                                     // declear式は値を返さない;
            }else {
                printf("SyntaxError:IllegalDecleartype!\n");
                Throw(1);
            }
        } else if (ast_j->type==AST_FCALL) {   //  typeが関数呼出し≒関数宣言(所謂プロトタイプ宣言)の場合である
            // ast_j:ASF_FCALL [AST_VAR[],AST_ARG_LIST[ast0,ast1,.....]]
            //                  <0>       <1>          <1,0>,<1,1>,...g
            arglist_ast = (ast*)vector_ref(ast_j->table,1);
            f_name_ast = (ast*)vector_ref(ast_j->table,0);
            if (f_name_ast->type != AST_VAR) {printf("SyntaxError:IllegalFunctionName!\n");Throw(1);}
            Vector *d_args = make_arg_list_type(arglist_ast);
            if (arglist_ast->type ==  AST_ARG_LIST_DOTS ||arglist_ast->type ==  AST_EXP_LIST_DOTS) dotted=TRUE; else dotted=FALSE;
            //
            s=(Symbol*)vector_ref(f_name_ast->table,0);
            ct=new_ct(OBJ_UFUNC, dcl_ast->o_type,(Vector*)vector_ref(d_args,1),dotted);
            put_gv(s,ct);
            //
            push(code,(void*)LDC);push(code,create_zero(OBJ_UFUNC));                // 「0」で初期化しておく
            push(code,(void*)GSET);push(code,(void*)s);push(code,(void*)DROP);      // declear式は値を返さない;
        } else {                                                                    // 宣言詞の後に続くのは変数(var),関数呼び出し(fcall)≒関数宣言 以外にない
            printf("SyntaxError:IllegalDecleaition!\n");
            Throw(0);
        }
    }
    pop(code);                  // 最後のDROPコードは不要なので削除
    return  new_code(code,ct);  // 最後に作ったcodeとCTを返す
}

code_ret *codegen_lambda(ast * lambda_ast,Vector *env, int tail) {  // AST_LAMBDA [AST_EXP_LIST [expr,   exp,   ...]], body_expr]
                                                                    //                           <0,0,0>,<0,0,1>,...   <1>
    //Vector *args=vector_init(3);
    //Vector *v=vector_init(3);
    ast *arg_list_ast = (ast*)vector_ref(lambda_ast->table,0);    //arg_list
    if ((arg_list_ast->type != AST_ARG_LIST) && (arg_list_ast->type != AST_ARG_LIST_DOTS)) {printf("SyntaxError:not argment list!\n");Throw(0);}
    int i;
    //ast * arg_ast_i;
    //Data *d;
    Vector *d_args=make_arg_list_type(arg_list_ast);
    Vector *args=(Vector*)vector_ref(d_args,0);
    Vector *v=(Vector*)vector_ref(d_args,1);
    push(env,(void*)args);//PR(1);
    //env_print(env);
    //
    Vector *code = vector_init(3);push(code,(void*)LDF);
    code_ret *code_s=codegen((ast*)vector_ref(lambda_ast->table,1),env,TRUE);//PR(2);
    Vector * code1 = code_s -> code; push(code1,(void*)RTN);
    // code=vector_append(code,code_s->code);
    code_type *ct=code_s->ct;                                                   // CT:body_exprが返すcode_type
    push(code,(void*)code1);
    //
    pop(env);// !!don't forget!!!
    code_s= new_code(code,new_ct(OBJ_UFUNC,ct->type,v,0));                      // codeの型はUFUNC、返す値の型はbody_exprが返す型
    if (arg_list_ast->type==AST_ARG_LIST_DOTS) code_s->ct->dotted=1;
    return code_s;
}

code_ret * codegen_while(ast *while_ast, Vector *env, int tail){
    code_ret *code_s = codegen(vector_ref(while_ast->table, 0), env, FALSE);
    if (code_s->ct->type != OBJ_INT ) {printf("SyntaxError:Must be Cond code!\n");Throw(0);}
    Vector * code = code_s->code; long n = vector_length(code);
    //
    code_s = codegen(vector_ref(while_ast->table, 1), env, FALSE);
    Vector *loop_code =code_s->code;
    push(loop_code, (void*)DROP); push(loop_code, (void*)JOIN);
    //
    push(code, (void*)WHILE); push(code, (void*)n); push(code, (void*)loop_code);
    //
    return new_code(code, new_ct(OBJ_NONE, OBJ_NONE, (void*)0, FALSE));
}

code_ret * codegen_for(ast *for_ast, Vector *env, int tail){
    // init astの処理
    code_ret *code_s = codegen(vector_ref(for_ast->table, 0), env, tail);  // init ast
    Vector *code = code_s->code;push(code, (void*)DROP);
    // cond astの処理
    code_s = codegen(vector_ref(for_ast->table, 1), env, FALSE);  // cond ast
    if (code_s->ct->type != OBJ_INT ) {printf("SyntaxError:Must be Cond code!\n");Throw(0);}
    //Vector * code = code_s->code; long n = vector_length(code);
    long n = vector_length(code_s->code);//push(code, code_s->code);
    code = vector_append(code, code_s->code);
    // loop astの処理
    code_s = codegen(vector_ref(for_ast->table, 2), env, FALSE);    // loop ast
    Vector *loop_code =code_s->code;
    push(loop_code, (void*)DROP); push(loop_code, (void*)JOIN);
    //
    push(code, (void*)WHILE); push(code, (void*)n); push(code, (void*)loop_code);
    //
    return new_code(code, new_ct(OBJ_NONE, OBJ_NONE, (void*)0, FALSE));
}
/*
code_ret * codegen_loop(ast *loop_ast, Vector *env, int tail){
    code_ret *code_s = codegen(vector_ref(loop_ast->table, 0), env, FALSE);
    if (code_s->ct->type != OBJ_INT ) {printf("SyntaxError:Must be Cond code!\n");Throw(0);}
    Vector * code = code_s->code; //long n = vector_length(code);
    //
    code_s = codegen(vector_ref(loop_ast->table, 1), env, FALSE);
    Vector *loop_code =code_s->code;
    push(loop_code, (void*)DROP); push(loop_code, (void*)JOIN);
    //
    push(code, (void*)WHILE); push(code, (void*)n); push(code, (void*)loop_code);
    //
    return new_code(code, new_ct(OBJ_NONE, OBJ_NONE, (void*)0, FALSE));
}
*/
code_ret *codegen_if(ast *a, Vector *env, int tail) {               // AST_IF,[cond_expr,true_expr,false_expr]

    code_ret *code_s1 = codegen(vector_ref(a -> table, 0),env,FALSE);
    Vector *code = code_s1->code;                                             // make cond_code 
    //code_s1 = codegen(vector_ref(a->table,1),env,tail);             // make true_code
    //Vector *code1=code_s1->code;code_type *ct1=code_s1->ct;
    //code_s1 = codegen(vector_ref(a->table,2),env,tail);             // make false_code
    //Vector *code2=code_s1->code;code_type *ct2=code_s1->ct;
    //if (tail) push(code,(void*)TSEL); else push(code,(void*)SEL);
    Vector *code1, *code2;
    code_type *ct1, *ct2;

    if (tail) {
        code_s1 = codegen(vector_ref(a->table,1),env,TRUE);             // make true_code
        code1=code_s1->code; ct1=code_s1->ct;
        code_s1 = codegen(vector_ref(a->table,2),env,TRUE);             // make false_code
        code2=code_s1->code; ct2=code_s1->ct;
        push(code,(void*)TSEL);

    } else { 
        code_s1 = codegen(vector_ref(a->table,1),env,FALSE);             // make true_code
        code1=code_s1->code; ct1=code_s1->ct;
        code_s1 = codegen(vector_ref(a->table,2),env,FALSE);             // make false_code
        code2=code_s1->code; ct2=code_s1->ct;
        push(code,(void*)SEL);
    }
    if (ct1->type == ct2->type || ct1->type==OBJ_NONE || ct2->type==OBJ_NONE) { // 暫定処置
        if (tail) {push(code1,(void*)RTN);push(code2,(void*)RTN);}
        else {push(code1,(void*)JOIN);push(code2,(void*)JOIN);}
        push(code ,(void*)code1); push(code,(void*)code2); 
        if (ct1->type >= ct2->type) return new_code(code,ct1);      
        return new_code(code,ct2);
    } else {
        if (ct1->type != OBJ_GEN)  {push(code1,(void*)conv_op[ct1->type][OBJ_GEN]);}
        if (ct2->type != OBJ_GEN)  {push(code2,(void*)conv_op[ct2->type][OBJ_GEN]);}

        if (tail) {push(code1,(void*)RTN);push(code2,(void*)RTN);}
        else {push(code1,(void*)JOIN);push(code2,(void*)JOIN);}
        push(code,(void*)code1);push(code,(void*)code2);
        return new_code(code, new_ct(OBJ_GEN,OBJ_GEN,(void*)0,FALSE));
    }
}

code_ret *codegen_set(ast * set_ast, Vector *env, int tail) {   // AST_SET [set_type, AST_left_expr, AST_right_expr]
                                                                //          <0>       <1>            <2>
    // case of '+=' '-=' '*=' '/=' '%=' '|=' '&=' '^='
    code_ret *code_s;
    Vector *code, *code1, *v, *vv;
    code_type *ct, *ct1;
    ast *a1 ;
    Symbol *s;
    int i,j,_2op;
    // 演算代入の場合は2項演算命令と代入命令のastを作る
    if ((long)vector_ref(set_ast->table,0) != '=') {        // set_typeが'='以外の場合は演算代入である
        _2op=(((long)vector_ref(set_ast->table,0)) >> 8);   // 演算子は上位8bit
        vv=vector_init(3);push(vv,(void*)(long)'=');
        v=vector_init(3);push(v,(void*)(long)_2op);push(v,(void*)vector_ref(set_ast->table,1));push(v,(void*)vector_ref(set_ast->table,2));
        push(vv,(void*)vector_ref(set_ast->table,1));push(vv,(void*)new_ast(AST_2OP,OBJ_NONE,v));
        return codegen_set(new_ast(AST_SET,OBJ_NONE,vv),env,tail);
    }
    // 通常の代入の場合
    switch(((ast*)vector_ref(set_ast->table,1))->type) {
        case AST_VREF:  // AST_SET [set_type, AST_VREF [vect_expr,index] ], right_expr]
                        //          <0,0>               <1,0>     <1,1>    <2>
                        // -> right_code vect_name_code index_code VSET
            //right exprをcode化
            code_s=codegen(vector_ref(set_ast->table,2),env,FALSE);
            code=code_s->code;ct=code_s->ct;;
            //left exprをcode化
            code_s=codegen(vector_ref(set_ast->table,1),env,FALSE);
            code1=code_s->code;ct1=code_s->ct;
            //vector_set(code1,code1->_sp-1,(void*)VSET);
            i=(long)pop(code1);
            if ((i==REF || i==OREF || i==LDH) && ct->type != OBJ_GEN ) {
               push(code,(void*)conv_op[ct->type][OBJ_GEN]);
            } else if (i==SREF && ct->type != OBJ_SYM ) {
               //push(code,(void*)conv_op[ct->type][OBJ_SYM]);
                printf("SyntaxError:Must be Symbol!\n");Throw(0);
            } 
            if (i==REF) {
                push(code1,(void*)VSET);
                return new_code(vector_append(code,code1),new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
            } else if (i==SREF) {
                push(code1,(void*)SSET);
                return new_code(vector_append(code,code1),new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));
            } else if (i==LDH) {
                push(code1,(void*)HSET);
                return new_code(vector_append(code,code1),new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
            } else {
                push(code1,(void*)OSET);
                return new_code(vector_append(code,code1),new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
            }
        case AST_FCALL: // AST_SET [set_type,AST_FCALL [expr_name , expr_list],right_expr]
                        //          <0,0>              <1,0>        <1,1>      <2>
            a1=(ast*)vector_ref(((ast*)vector_ref(set_ast->table,1))->table,0);                       //a1:expr name
            if (a1->type != AST_VAR) {printf("SyntaxError:Must be Function Name!\n");Throw(0);}                            
            s=(Symbol * )vector_ref(a1->table,0);                                            
               //dcl宣言式を作成する
            
            Vector *v1=vector_init(2);
            ast *a0=(ast*)vector_ref(((ast*)vector_ref(set_ast->table,1))->table,1);                        //a:expr_list
            push(v1,(void*)a0);                      //push expr_list
            push(v1,(void*)vector_ref(set_ast->table,2)); //push righ expr
            ast *a2=new_ast(AST_LAMBDA,((ast*)vector_ref(set_ast->table,2))->o_type,v1);
            Vector *v2=vector_init(2);push(v2,(void*)(long)'=');
            push(v2,(void*)a1);push(v2,(void*)a2);//ast_print(new_ast(AST_SET,a2->o_type,v2),0);
            return codegen(new_ast(AST_SET,a2->o_type,v2),env,FALSE);
        case AST_VAR:   // AST_SET [set_type, AST_VAR [var_name], right_expr]
                        //          <0,0>             <1,0>       <2>
            code_s=codegen((ast*)vector_ref(set_ast->table,2),env,FALSE);
            code=code_s->code;code_type *ct2=code_s->ct;
            //if (type2==OBJ_UFUNC) {v2=code_s->arg_type;r_type2=code_s->function_r_type;dot2=code_s->dotted;}
            s=(Symbol*)vector_ref(((ast*)vector_ref(set_ast->table,1))->table,0);
            Vector *_pos=var_location(s,env);
            if (_pos) {
                Vector *pos=(Vector*)vector_ref(_pos,0);
                ct1=(code_type*)(long)vector_ref(_pos,1);   //ct1は変数名のコードタイプ
                if (ct1->type==OBJ_NONE || (ct1->type == OBJ_UFUNC && ct1->functon_ret_type ==0 )) {
                    ct1=ct2;
                    //envに保存してある戻り型情報がない関数型変数のコードタイプをct2で置き換える
                    i=(env->_sp)-(long)vector_ref(pos,0)-1;j=(long)vector_ref(pos,1);   //env内のポジションを計算して
                    Data *d=(Data*)malloc(sizeof(Data));d->key=s;d->val=ct1;                  //入れるべきデータを作って
                    vector_set((Vector*)vector_ref(env,i),j,(void*)d);//printf("||changed||\n");env_print(env);                  //セットする
                } else if (ct1->type != ct2->type) {
                    if (conv_op[ct2->type][ct1->type]==0) {printf("SyntaxError:CanotConvertType!%d %d\n",ct1->type,ct2->type);Throw(0);}
                    push(code,(void*)conv_op[ct2->type][ct1->type]);
                }
                //
                if ((long)vector_ref(pos,0)== 0) {
                    switch((long)vector_ref(pos,1)) {
                        case 0: push(code, (void*)SET00);break;
                        case 1: push(code, (void*)SET01);break;
                        case 2: push(code, (void*)SET02);break;
                        case 3: push(code, (void*)SET03);break;
                        default:push(code,(void*)SET);push(code,(void*)pos);
                    }
                } else if ((long)vector_ref(pos,0)== 1) {
                    switch((long)vector_ref(pos,1)) {
                        case 0: push(code, (void*)SET10);break;
                        case 1: push(code, (void*)SET11);break;
                        case 2: push(code, (void*)SET12);break;
                        case 3: push(code, (void*)SET13);break;
                        default:push(code,(void*)SET);push(code,(void*)pos);
                    }
                } else {
                    push(code,(void*)SET);push(code,(void*)pos);
                }
            } else {
                if ((ct1=get_gv(s))==NULL) {            //変数テーブルに名前がないなら
                    //printf("SyntaxError:variable not defined!");return NULL;
                    put_gv(s,ct2);ct1=ct2;              //新たにct2=右辺値のコードタイプで変数を作り左辺値のコードタイプは右辺値と同一
                } else if (ct1->type==OBJ_NONE || (ct1->type == OBJ_UFUNC && ct1->functon_ret_type ==0 )) {
                    ct1=ct2;put_gv(s,ct1);
                    //if (ct2->type==OBJ_UFUNC) {gvt->type=OBJ_UFUNC;gvt->arg_type=v;gvt->functon_ret_type=r_type;put_gv(s,gvt);}
                }else if (ct1->type != ct2->type) { 
                    if (conv_op[ct2->type][ct1->type]==0) {printf("!!!!SyntaxError:CanotConvertType!\n");Throw(0);}
                    push(code,(void*)conv_op[ct2->type][ct1->type]);
                }
                // ct1->type,ct2->typeがfunctionの場合はそのret_typeが同じかどうか確かめる
                if ((ct1->type==OBJ_UFUNC || ct1->type == OBJ_PFUNC) && (ct1->functon_ret_type != ct2->functon_ret_type)) {
                    if (conv_op[ct2->functon_ret_type][ct1->functon_ret_type]==0) {printf("!!!!SyntaxError:CanotConvertType!\n");Throw(0);}
                    //
                    // codeは [... LDF [... RTN]]の形をしており通常はRETの前に型変換命令を入れる
                    // tail call時のif命令だった場合は[... LDF [... TSEL [... RTN] [... RTN] RTN]]
                    v=(Vector*)vector_ref(code,code->_sp-1);//disassy(v,0,stdout);// 関数のbodyのコード
                    if (vector_ref(v,v->_sp-4) == (void*)TSEL) {    // tail call のIF式である
                        v1=(Vector*)vector_ref(v,v->_sp-2);v2=(Vector*)vector_ref(v,v->_sp-3);
                        vector_set(v1,v1->_sp-1,(void*)conv_op[ct2->functon_ret_type][ct1->functon_ret_type]);push(v1,(void*)RTN);
                        vector_set(v2,v2->_sp-1,(void*)conv_op[ct2->functon_ret_type][ct1->functon_ret_type]);push(v2,(void*)RTN);
                    } else {                                        // 通常の式である
                        vector_set(v,v->_sp-1,(void*)conv_op[ct2->functon_ret_type][ct1->functon_ret_type]);//返還命令を入れて
                        push(v,(void*)RTN);
                    }
                }
                push(code,(void*)GSET);push(code,(void*)s);
            }
            return new_code(code,ct1);
            //if (type1==OBJ_UFUNC) {
            //    if 
            //    code_s->function_r_type=r_type;code_s->arg_type=v;code_s->dotted=dot;
            //}
            //return code_s;
        default:printf("jhdjadjaw4yy87wfhwj\n");
    }
}

code_ret * codegen_ml(ast *a, Vector *env, int tail) {  //AST_ML [AST_expr_list [AST_1,AST_2,...]]
                //                       <0,0> <0,1>
    int dcl_flg = FALSE;
    ast *ast_list=(ast*)vector_ref(a->table,0);//ast_print(ast_list,0);
    Vector *a_arg_v=vector_init(3);Vector *d_arg_v=vector_init(3);Vector *v_expr_body=vector_init(3);
    int i,j;
    Vector *v,*v1,*v2,*v3,*code=vector_init(3);
    code_ret *code_s;
    ast *a1,*a2,*a3,*a0;

    for(i=0;i<ast_list->table->_sp;i++) {
        a1=(ast*)vector_ref(ast_list->table,i);//ast_print(a1,0);
        if (a1->type==AST_DCL) {
        //
        //
        //    
            dcl_flg=TRUE;
            for(j=0;j<((ast*)vector_ref(a1->table,0))->table->_sp;j++) {
                a2 = (ast*)vector_ref(((ast*)vector_ref(a1->table,0))->table,j);//ast_print(a2,0);
                if (a2->type==AST_VAR) {                                        // a2:AST_VAR [symbol_var_name_]
                    a2->o_type=a1->o_type;
                    v=vector_init(2);
                    push(v,(void*)TOKEN_NONE);push(v,(void*)NULL);
                        push(a_arg_v,new_ast(AST_LIT,a1->o_type,v));//printf("######\n<<<a_arg_v:%d>>>\n",j);ast_print(new_ast(AST_LIT,OBJ_NONE,v),0);                           // a_arg_v:actual arg list
                    push(d_arg_v,(void*)a2);                                    // d_arg_v:dummy arg list
                } else if (a2->type==AST_SET &&                                 // a2: AST_SET [set_type, AST_VAR [var_name], expr_ast]
                            ((ast*)vector_ref(a2->table,1))->type==AST_VAR) {   //                        <1>                 <2>
                    push(a_arg_v,(void*)vector_ref(a2->table,2));               // a_arg_v:actual arg list
                    //push(d_arg_v,(void*)vector_ref(a2->table,1));               // d_arg_v:dummy arg list
                    ast *a0=(ast*)vector_ref(a2->table,1);
                    a0->o_type=a1->o_type;
                    push(d_arg_v,(void*)a0);               // d_arg_v:dummy arg list
                } else if (a2->type==AST_SET &&                                 // a2: AST_SET [set_type, AST_FCALL [FUNC_NAME,ARG_LIST], expr_ast]
                            ((ast*)vector_ref(a2->table,1))->type==AST_FCALL) { //                        <1>        <1,0>,    <1,1>,       <2>
                    // ...;function Fname;Fname(...)=...に変換する
                    v=vector_init(2);push(v,(void*)TOKEN_NONE);push(v,(void*)NULL);
                    push(a_arg_v,new_ast(AST_LIT,OBJ_UFUNC,v));
                    //
                    ast *a3=(ast*)vector_ref(((ast*)vector_ref(a2->table,1))->table,0);  //関数名のASTを取り出して
                    a3->o_type=OBJ_UFUNC;                                           //typeを関数に変更して             
                    push(d_arg_v,a3);                                                //仮引数リストに登録
                    push(v_expr_body,(void*)a2);    //body部にa2:Fname(...)=...のASTを挿入
                    //コードを書く！
                } else {
                    printf("Syntax error :decliation in ml_expr%d\n",a2->type);
                    Throw(0);
                }
            }
        } else {//printf("!!!\n");
        //
        //
        //

            if (i==ast_list->table->_sp-1) {
                //code_s1= codegen(a1,env,tail);      
                //ct=code_s1->ct;
                //code=vector_append(code,code_s1->code);
                push(v_expr_body,(void*)a1);//ast_print(a1,0);
            } else {
                //code_s1 = codegen(a1,env,FALSE);
                //code = vector_append(code,code_s1->code);
                //push(code,(void*)DROP);//disassy(code,0);
                push(v_expr_body,(void*)a1);//ast_print(a1,0);
            }
        }
    }
    if (dcl_flg==FALSE) {
        //code_s=new_code(code,ct);
        //return code_s;
        for(i=0;i<v_expr_body->_sp-1;i++) {
            code_s=codegen((ast*)vector_ref(v_expr_body,i),env,FALSE);
            code=vector_append(code,code_s->code);push(code,(void*)DROP);
        }
        code_s=codegen((ast*)vector_ref(v_expr_body,v_expr_body->_sp-1),env,tail);
        code=vector_append(code,code_s->code);
        return new_code(code,code_s->ct);
    } else {
        v1=vector_init(2);v2=vector_init(1);
        push(v1,(void*)new_ast(AST_ARG_LIST,OBJ_NONE, d_arg_v));//printf("##########\n<<<v1>>>\n");ast_print(new_ast(AST_ARG_LIST,OBJ_NONE,d_arg_v),0);//dummy arg list
        //push(v2,new_ast(AST_EXP_LIST,((ast*)vector_ref(v_expr_body,v_expr_body->_sp-1))->o_type,v_expr_body));
        push(v2,new_ast(AST_EXP_LIST,OBJ_NONE,v_expr_body));//printf("##########\n<<<v2>>>\n");ast_print(new_ast(AST_EXP_LIST,OBJ_NONE,v_expr_body),0);
        //push(v1,(void*)new_ast(AST_ML,((ast*)vector_ref(v_expr_body,v_expr_body->_sp-1))->o_type,v2));//ast_print(new_ast(AST_ML,v5),0) ;      // v3:[AST_EXP_LIST,AST_ML]
        push(v1,(void*)new_ast(AST_ML,OBJ_NONE,v2));//ast_print(new_ast(AST_ML,OBJ_NONE,v2),0) ;      // v3:[AST_EXP_LIST,AST_ML]
        //a1=new_ast(AST_LAMBDA,((ast*)vector_ref(v_expr_body,v_expr_body->_sp-1))->o_type,v1);ast_print(a1,0);
        a1=new_ast(AST_LAMBDA,OBJ_NONE,v1);//printf("########\n<<<a1>>>\n");ast_print(a1,0);//ここまでOK!
        //
        v3=vector_init(3);
        push(v3,(void*)a1);
        push(v3,(void*)new_ast(AST_EXP_LIST,OBJ_NONE,a_arg_v));//ast_print(new_ast(AST_EXP_LIST,OBJ_NONE,a_arg_v),0);   // actual arg list
        a2=new_ast(AST_FCALL,a1->o_type,v3);//ast_print(a2,0);                   // AST_FCALL [AST_LAMBDA [AST_EXP_LIST [..],AST_ML [...]],AST_EXP_LIST [...]]
        return codegen(a2,env,tail);
    }
}

code_ret * codegen_class_var(ast *class_var_ast, Vector *env, int tail) {
    ast * left_ast  = class_var_ast->table->_table[0];
    ast * right_ast = class_var_ast->table->_table[0];
    void * d;
    if (left_ast->type == AST_VAR) {
        if ((d=*Hash_get(CLASS_NAME,(Symbol*)left_ast->table->_table[0])) != NULL) {        // class変数

        }else if ((d=*Hash_get(IMPORT_NAME,(Symbol*)left_ast->table->_table[0])) != NULL) {// import変数

        }
    } 


}

code_ret * codegen_class(ast *a, Vector * env, int tail) {

}

code_ret * codegen(ast * a, Vector * env, int tail) {
    switch(a->type) {
        case AST_ML:        return  codegen_ml      (a, env, tail);
        case AST_IF:        return  codegen_if      (a, env, tail);
        case AST_SET:       return  codegen_set     (a, env, tail);
        case AST_LAMBDA:    return  codegen_lambda  (a, env, tail);
        case AST_DCL:       return  codegen_dcl     (a, env, tail);
        case AST_FCALL:     return  codegen_fcall   (a, env, tail);
        case AST_APPLY:     return  codegen_apply   (a, env, tail);
        case AST_2OP:       return  codegen_2op     (a, env, tail);
        case AST_1OP:       return  codegen_1op     (a, env, tail); 
        case AST_VREF:      return  codegen_vref    (a, env, tail);
        case AST_SLS:       return  codegen_sls     (a, env, tail);
        case AST_VAR:       return  codegen_var     (a, env, tail);
        case AST_LIT:       return  codegen_lit     (a, env, tail);
        case AST_VECT:      return  codegen_vect    (a, env, tail);
        case AST_PAIR_LIST: return  codegen_dict    (a, env, tail);
        case AST_WHILE:     return  codegen_while   (a, env, tail);
        case AST_FOR:       return  codegen_for     (a, env, tail);
//        case AST_CLASS_VAR: return  codegen_cl_var  (a, env, tail);
//        case AST_CLASS:     return  codegen_class   (a, env, tail);
        default: printf("syntaxError:Unknown AST!\n");Throw(0);
    } 
}

void ctrc_handler() {Throw(4);};
//Hash *G;
void disassy(Vector * code, int indent, FILE*fp) {
    int i; long c; 
    Vector * v; char * s; 
    for(i = 0; i<indent; i ++ ) fprintf(fp,"\t"); 
    fprintf(fp,"[\n"); indent ++ ; 

    while (TRUE) {
        if (is_queu_empty(code)) break;
        c = (long)dequeue(code);//printf("%ld\n",c); 
        for(i= 0; i< indent; i ++ ) fprintf(fp,"\t"); 
        switch(c) {  
            case SEL:   case TSEL:
                fprintf(fp,"%s\n", code_name[c]); 
                disassy((Vector * ) dequeue(code), indent ,fp); 
                disassy((Vector * ) dequeue(code), indent, fp); 
                break; 
            case LDF:   case LDP:   case LOOP:
                fprintf(fp,"%s\n", code_name[c]); 
                disassy((Vector *)dequeue(code), indent,fp ); 
                break;
            case LD:    case SET:
                v = (Vector * )dequeue(code); 
                fprintf(fp,"%s\t(%ld %ld)\n", code_name[c], (long)vector_ref(v, 0), (long)vector_ref(v, 1));  
                break; 
            case LDG:   case GSET: 
                s = ((Symbol * )dequeue(code)) -> _table;
                fprintf(fp,"%s\t%s\n", code_name[c], s);
                break;
            case WHILE: 
                fprintf(fp,"%s\t%ld\n", code_name[c], (long)dequeue(code));
                disassy((Vector*)dequeue(code), indent, fp);
                break;
            default:
                if (op_size[c] == 0) fprintf(fp, "%s\n", code_name[c]);
                else if (op_size[c] == 1) fprintf(fp, "%s\t%ld\n", code_name[c], (long)dequeue(code)); 
                else fprintf(fp, "Unknkown Command %s\n", (char * )c); 
        }
    }
    indent--;
    for(i= 0; i< indent; i ++ ) printf("\t"); 
    fprintf(fp,"]\n"); 
    code ->_cp = 0;  
} 

extern Vector*tokenbuff;
extern void make_primitive();
void * _realloc(void * ptr, size_t old_size, size_t new_size) {
    return GC_realloc(ptr, new_size); 
}
// 式1つをコンパイルしてコードと型を返す
// ※2つ以上の式があった場合どうするか？今は無視
code_ret * str_compile(Symbol * s) {
    TokenBuff * SS = new_str_tokenbuff(s);
    ast * a ;
    Vector * env = vector_init(10);
    if ((a = is_expr(SS)) && get_token(SS)->type == ';') return codegen(a,env,FALSE);
    else return NULL;
}

// 式一つを評価して結果をobjectにして返す
object * code_eval(code_ret * code_s) {
    Vector * code   = code_s->code;push(code,(void*)STOP);
    Vector * Ret    = vector_init(500); 
    Vector * Env    = vector_init(5); 
    Vector * EEnv   = vector_init(50); 
    Vector * Stack  = vector_init(500);
    
    void * value = eval(Stack,Env,code,Ret,EEnv,G);
    return newOBJ(code_s->ct->type, value); 
}

void code_load(FILE *f) {
    ast *a;Vector * env = vector_init(10);
    TokenBuff * SS = new_tokenbuff(f);
    while (TRUE) {
        if ((a = is_expr(SS)) && get_token(SS)->type == ';' ){
            //Vector * env = vector_init(10);
            code_eval(codegen(a,env,FALSE));
        } else break;
    }
}
#include <unistd.h>

int main(int argc, char*argv[]) {
    void* value;
    ast *a;
    TokenBuff *S;
    int tokencp,tokensp,DEBUG=FALSE;
    Vector*env;
    Vector*code;
    code_ret *code_s;
    obj_type type; 
    code_type* ct;
    if (mpfr_mp_memory_cleanup () != 0) {printf("MemoryAssignment Error!\n");exit(0);}
    mp_set_memory_functions((void *)GC_malloc, (void * )_realloc, (void * ) GC_free);
    mpfr_set_default_prec(256);
    Vector * t; 
    Vector * Stack = vector_init(500000); // 500だと５倍くらい遅い！なぜ？
    //Vector * C, * CC ; 
    Vector * Ret = vector_init(500); 
    Vector * Env = vector_init(5); 
    Vector * EEnv = vector_init(50); 
    G = Hash_init(256); // must be 2^n 
    GLOBAL_VAR = Hash_init(256);
    //
    clock_t s1_time,s2_time,e_time;
    struct timespec S1_T,S2_T,E_T;
    //if (argc<=1) S=new_stream(stdin);
    FILE*fp=stdin;
    char*PROMPT=">";
    make_primitive();    
    int i=1;
    token * tk;
    CEXCEPTION_T e;
    Symbol *underbar_sym=new_symbol("_",1);
    while (i<argc) {
        if (strcmp(argv[i],"-d")==0) {DEBUG=TRUE;i++;continue;}
        if (strcmp(argv[i],"-f")==0) {
            fp = fopen(argv[++i], "r");
            if (fp == NULL) {printf("file %s doesn't exist\n", argv[i]); return  - 1; }
            i++;continue;
        }
        //if (strcmp(argv[i],'-i')==0) {
        //    while (i<argv) {
        //        fp=fopen(argv[++i],"r");
        //        if (fp == NULL) {printf("file %s doesn't exist\n", argv[i]); continue; }
        //    }
        //}
    }
    if (fp==stdin) printf("PURE REPL Version 0.3.1 Copyright 2022.04.29 M.Taniguro\n");fflush(stdout);
    //DEBUG=TRUE;
    //
    code_load(fopen("lib.pur","r"));
    //
    S = new_tokenbuff(fp);
    //tokenbuff=vector_init(100);
    signal(SIGINT,ctrc_handler);
    while (TRUE) {
        //printf(">>");
        //tokenbuff=vector_init(100);
        env=vector_init(10);
        tokencp = S->buff->_cp;tokensp=S->buff->_sp;
        //token_print(tokenbuff);
        Try {
            if (fp==stdin) putchar('>');fflush(stdout);
            //if (fp==stdin) write(1,PROMPT,1);
            if ((a=is_expr(S)) && (tk=get_token(S))->type==';') {
                if (DEBUG) ast_print(a,0);
                s1_time = clock();clock_gettime(CLOCK_REALTIME,&S1_T);
                code_s = codegen(a,env,FALSE);//PR(121);
                code=code_s->code;push(code,(void*)STOP);//PR(122);
                ct=code_s->ct;type=ct->type;
                if (DEBUG) {
                    printf("expr type:%d\n",type);
                    if (type==OBJ_UFUNC || type==OBJ_PFUNC ) {
                            if (ct->functon_ret_type !=0) {printf("arg type:");vector_print(ct->arg_type);printf("ret type: %d\n",ct->functon_ret_type);}
                            else {printf("argment & return type not assigned!\n");}
                    }
                    disassy(code,0,stdout);
                }
                s2_time = clock();clock_gettime(CLOCK_REALTIME,&S2_T);
                value = eval(Stack,Env,code,Ret,EEnv,G);
                e_time = clock();clock_gettime(CLOCK_REALTIME,&E_T);
                if (fp==stdin) {
                    //printf("compile time[ms]:%f\tevalueate time[ms]:%f\n",(double)1000*(s2_time-s1_time)/CLOCKS_PER_SEC,(double)1000*(e_time-s2_time)/CLOCKS_PER_SEC);
                    printf("compile time[sec]:%f\tevalueate time[sec]:%f\n",(double)(S2_T.tv_sec-S1_T.tv_sec)+(double)(S2_T.tv_nsec-S1_T.tv_nsec)/(1000*1000*1000),(double)(E_T.tv_sec-S2_T.tv_sec)+(double)(E_T.tv_nsec-S2_T.tv_nsec)/(1000*1000*1000));
                    printf("%s ok\n", objtype2str(type,value));
                }
                Hash_put(G, underbar_sym,value);put_gv(underbar_sym,ct);
            }  else {
                //if (a==NULL) {//printf("file end!!\n");
                if (a == NULL || tk-> type == TOKEN_EOF ) {
                    //exit(0);
                    fclose(fp);
                    fp=stdin;S=new_tokenbuff(fp);
                    if (fp==stdin) printf("PURE REPL Version 0.3.0 Copyright 2021.08.11 M.Taniguro\n");
                } else {
                    printf("SyntaxErroor:Not a exprssion!\n");
                    Throw(1);
                // tokenbuff->_cp=token_p;
                //tokenbuff=vector_init(100);
                }
            } 
        } Catch(e) {
            switch(e) {
                case 0: printf("Error in generator\n");break;
                case 1: printf("Error in parser\n");break;
                case 3: printf("Error in VM\n");break;
                case 4: printf("Ctr-C pressed!\n");break;
            }
            //fclose(fp);
            S=new_tokenbuff(stdin);
            //S->buff=vector_init(100);
            //S->buff->_cp = tokencp;S->buff->_sp=tokensp;
            continue;
        }
        if (DEBUG) token_print(S); 
        //S->buff->_cp=0;S->buff->_sp=0;
    }

}
