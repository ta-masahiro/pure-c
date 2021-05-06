#include "vm.h"
#include "lexparse.h"

void disassy(Vector*v,int i,FILE*fp);

int op2_1[] = {'+', '-', '*', '/', '%', '*'*256+'*','<'*256+'-','|','&', '>'*256+'>', '<'*256+'<', '>', '<', '='*256+'=', '!'*256+'=', '>'*256+'=', '<'*256+'=',0};
enum CODE op2_2[15][18] = {
                {IADD, ISUB, IMUL, IDIV, IMOD, IPOW, 0,    IBOR, IBAND,ISR, ISL, IGT, ILT, IEQ, INEQ, IGEQ, ILEQ, 0},   // OBJ_NONEはINTと同じに   
              //{ADD,  SUB,  MUL,  DIV,  MOD,  POW,  PUSH, BOR,  BAND, SR,  SL,  GT,  LT,  EQ,  NEQ,  GEQ,  LEQ , 0},
                {IADD, ISUB, IMUL, IDIV, IMOD, IPOW, 0,    IBOR, IBAND,ISR, ISL, IGT, ILT, IEQ, INEQ, IGEQ, ILEQ, 0},   // OBJ_INT
                {LADD, LSUB, LMUL, LDIV, LMOD, LPOW, 0,    LBOR, LBAND,LSR, LSL, LGT, LLT, LEQ, LNEQ, LGEQ, LLEQ, 0},   // OBJ_LINT
                {RADD, RSUB, RMUL, RDIV, RMOD, RPOW, 0,    0,    0,    0,   0,   RGT, RLT, REQ, RNEQ, RGEQ, RLEQ, 0},   // OBJ_RAT
                {FADD, FSUB, FMUL, FDIV, FMOD, FPOW, 0,    0,    0,    0,   0,   FGT, FLT, FEQ, FNEQ, FGEQ, FLEQ, 0},   // OBJ_FLT
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_LFLT
                {OADD, OSUB, OMUL, ODIV, OMOD, OPOW, OPUSH,OBOR, OBAND,OSR, OSL, OGT, OLT, OEQ, ONEQ, OGEQ, OLEQ, 0},   // OBJ_GEN
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_PFUNC
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_UFUNC
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_CNT
                {VAPP, 0,    VMUL, 0,    0,    0,    VPUSH,0,    0,    0,   0,   0,   0,   VEQ, 0,    0,    0,    0},   // OBJ_VECT
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_DICT
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_PAIR
                {SAPP, 0,    SMUL, 0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   SEQ, 0,    0,    0,    0},   // OBJ_SYM
                {0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,    0,    0,    0},   // OBJ_IO
                };
obj_type op2_3[]={0,0,0,0,0,0,0,0,0,0,0,OBJ_INT,OBJ_INT,OBJ_INT,OBJ_INT,OBJ_INT,OBJ_INT,0};

int op1_1[] = {'-', '~', '@','+'*256+'+', '-'*256+'-', '-'*256+'>', 0};
enum CODE op1_2[15][6] = 
               {{INEG, IBNOT,0,    IINC, DEC , 0   },
              //{NEG, BNOT,  0,　　INC,  DEC , VPOP},
                {INEG, BNOT, 0,    INC,  DEC , 0},
                {LNEG, LBNOT,0,    LINC, LDEC, 0},
                {RNEG, 0,    0,    0,    0   , 0},
                {FNEG, 0,    0,    0,    0   , 0},
                {0,    0,    0,    0,    0   , 0},
                {ONEG, OBNOT,OLEN, OINC, ODEC, OPOP},     // GEN
                {0,    0,    0,    0,    0   , 0},     // PFUNC
                {0,    0,    0,    0,    0   , 0},     // UFUNC
                {0,    0,    0,    0,    0   , 0},     // CNT
                {0,    0,    VLEN, 0,    0   , VPOP},     // VECT
                {0,    0,    0,    0,    0   , 0},     // DICT
                {0,    0,    0,    0,    0   , 0},     // PAIR
                {0,    0,    SLEN, 0,    0   , SPOP},     // SYM
                {0,    0,    0,    0,    0   ,0},     // IO
               };
obj_type op1_3[]={0,0,OBJ_INT,0,0,OBJ_GEN};
enum CODE conv_op[15][15] = {{0, 0,    ITOL,    ITOR,    ITOF,   0,     ITOO   },    //OBJ_NONEには0が入っているのでINTとみなす
                    //  NONE  INT   LONG  RAT   FLOAT LFLOAT GEN    PFUNC UFUNC CNT   VECT DICT PAIR SYM   IO
                        {0,   0,    ITOL, ITOR, ITOF, 0,     ITOO,  0,    0,    0,    0,   0,   0,   ITOS, 0  },
                        {0,   LTOI, 0   , LTOR, LTOF, 0,     LTOO,  0,    0,    0,    0,   0,   0,   LTOS, 0  },
                        {0,   RTOI, RTOL, 0   , RTOF, 0,     RTOO,  0,    0,    0,    0,   0,   0,   RTOS, 0  },
                        {0,   FTOI, FTOL, FTOR, 0   , 0,     FTOO,  0,    0,    0,    0,   0,   0,   FTOS, 0  },
                        {0,   0,    0,    0,    0,    0,     0,     0,    0,    0,    0,   0,   0,   0,    0  },
                        {0,   OTOI, OTOL, OTOR, OTOF, 0,     0,     0,    0,    0,    OTOV,0,   0,   OTOS, 0  },
                        {0,   0,    0,    0,    0,    0,     0,     0,    0,    0,    0,   0,   0,   0,    0  },//PFTOO},
                        {0,   0,    0,    0,    0,    0,     0,     0,    0,    0,    0,   0,   0,   0,    0  },//UFTOO},
                        {0,   0,    0,    0,    0,    0,     0,     0,    0,    0,    0,   0,   0,   0,    0  },//CNTOO},
                        {0,   0,    0,    0,    0,    0,     VTOO,  0,    0,    0,    0,   0,   0,   VTOS, 0  },
                        {0,   0,    0,    0,    0,    0,     0,     0,    0,    0,    0,   0,   0,   0,    0  },//DTOO} ,
                        {0,   0,    0,    0,    0,    0,     0,     0,    0,    0,    0,   0,   0,   0,    0  },//PATOO},
                        {0,   STOI, STOL, STOR, STOF, 0,     STOO,  0,    0,    0,    0,   0,   0,   0,    0  },
                        {0,   0,    0,    0,    0,    0,     0,     0,    0,    0,    0,   0,   0,   0,    0  }};//IOTOO}};

enum CODE get_convert_op(obj_type from, obj_type to) {
    if (from<=to) return 0;
    return conv_op[from][to];
}

Hash* G;
Hash* GLOBAL_VAR;
typedef struct {
    obj_type type;
    obj_type functon_ret_type;
    Vector  *arg_type;
    int dotted;
} code_type;

code_type * new_ct(obj_type type,obj_type frt, Vector*at,int dot ) {
    code_type* ct=(code_type*)malloc(sizeof(code_type));
    ct->type=type;ct->functon_ret_type=frt;ct->arg_type=at,ct->dotted=dot;
    return ct;
}

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

typedef struct {
    Vector      *code;              // intermediate code
    //obj_type    type;               // object type of code
    //Vector*     arg_type;           // if type is function, parameter type
    //obj_type    function_r_type;    // if type is function, function return type
    //int         dotted;             // 0: normal function 1: dotted fctunction
    code_type * ct;
} code_ret;


code_ret*new_code(Vector*code, code_type * ct) {
    code_ret*r=(code_ret*)malloc(sizeof(code_ret));
    r->code=code;r->ct=ct;
    return r;
}

void* create_zero(obj_type type) {
    void* r;
    mpz_ptr z=(mpz_ptr)malloc(sizeof(MP_INT));
    mpq_ptr q=(mpq_ptr)malloc(sizeof(MP_RAT));
    Vector*cl;
    double* d;
    switch(type) {
        case OBJ_NONE:
        case OBJ_INT:return (void*)0;
        case OBJ_LINT:mpz_init(z); return (void*)z;
        case OBJ_RAT:mpq_init(q);return (void*)q;
        case OBJ_FLT:d=(double*)malloc(sizeof(double));*d=0;return (void*)d;
        case OBJ_GEN:return newINT(0);
        case OBJ_SYM:return (void*)new_symbol("",0);
        case OBJ_VECT:return (void*)vector_init(0);
        case OBJ_UFUNC:
            cl = vector_init(4);
            push(cl, (void * )"CL"); push(cl, (void * )0); push(cl, (void * )0);
            return cl;
    }
}

code_ret * codegen(ast * a, Vector * env, int tail) {
    Vector * code = vector_init(10), *code1, *code2,*v,*v1,*v2,*v3,*a_arg_v,*d_arg_v,*pos,*_pos,*args,*v_expr_body;
    code_ret* code_s,*code_s1;
    //code_s->code=code;code_s->type=OBJ_NONE;
    ast *a0,* a1,*a2,*a3,*ast_list;
    int i,j,n,*tp,dcl_flg,m,dot,dot1,dot2; 
    long int_num;
    double fl_num,*fl_num_p;
    mpz_ptr z; mpq_ptr q;
    tokentype lit_type;
    obj_type type1,type2,r_type,r_type1,r_type2;
    Symbol*str_symbol,*s;
    Data*d;
    code_type *ct,*ct1,*ct2;
    enum CODE opcode;
    //ast_print(a,0);
    switch(a->type) {
        case AST_ML:    //AST_ML [AST_expr_list [AST_1,AST_2,...]]
                        //                       <0,0> <0,1>
            dcl_flg = FALSE;
            ast_list=(ast*)vector_ref(a->table,0);//ast_print(ast_list,0);
            a_arg_v=vector_init(3);d_arg_v=vector_init(3);v_expr_body=vector_init(3);
            for(i=0;i<ast_list->table->_sp;i++) {
                a1=(ast*)vector_ref(ast_list->table,i);//ast_print(a1,0);
                if (a1->type==AST_DCL) {
                    dcl_flg=TRUE;
                    for(j=0;j<((ast*)vector_ref(a1->table,0))->table->_sp;j++) {
                        a2 = (ast*)vector_ref(((ast*)vector_ref(a1->table,0))->table,j);//ast_print(a2,0);
                        if (a2->type==AST_VAR) {                                        // a2:AST_VAR [symbol_var_name_]
                            a2->o_type=a1->o_type;
                            v=vector_init(2);
                            push(v,(void*)TOKEN_NONE);push(v,(void*)NULL);
                                push(a_arg_v,new_ast(AST_LIT,a1->o_type,v));printf("######\n<<<a_arg_v:%d>>>\n",j);ast_print(new_ast(AST_LIT,OBJ_NONE,v),0);                           // a_arg_v:actual arg list
                            push(d_arg_v,(void*)a2);                                    // d_arg_v:dummy arg list
                        } else if (a2->type==AST_SET &&                                 // a2: AST_SET [set_type, AST_VAR [var_name], expr_ast]
                                    ((ast*)vector_ref(a2->table,1))->type==AST_VAR) {   //                        <1>                 <2>
                            push(a_arg_v,(void*)vector_ref(a2->table,2));               // a_arg_v:actual arg list
                            //push(d_arg_v,(void*)vector_ref(a2->table,1));               // d_arg_v:dummy arg list
                            a0=(ast*)vector_ref(a2->table,1);
                            a0->o_type=a1->o_type;
                            push(d_arg_v,(void*)a0);               // d_arg_v:dummy arg list
                        } else if (a2->type==AST_SET &&                                 // a2: AST_SET [set_type, AST_FCALL [FUNC_NAME,ARG_LIST], expr_ast]
                                    ((ast*)vector_ref(a2->table,1))->type==AST_FCALL) { //                        <1>        <1,0>,    <1,1>,       <2>
                            // ...;function Fname;Fname(...)=...に変換する
                            v=vector_init(2);push(v,(void*)TOKEN_NONE);push(v,(void*)NULL);
                            push(a_arg_v,new_ast(AST_LIT,OBJ_UFUNC,v));
                            //
                            a3=(ast*)vector_ref(((ast*)vector_ref(a2->table,1))->table,0);  //関数名のASTを取り出して
                            a3->o_type=OBJ_UFUNC;                                           //typeを関数に変更して             
                            push(d_arg_v,a3);                                                //仮引数リストに登録
                            push(v_expr_body,(void*)a2);    //body部にa2:Fname(...)=...のASTを挿入
                            //コードを書く！
                        } else {
                            printf("Syntax error :decliation in ml_expr%d\n",a2->type);
                            return NULL;
                        }
                    }
                } else {//printf("!!!\n");
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
                    code=vector_append(code,code_s->code);
                }
                code_s=codegen((ast*)vector_ref(v_expr_body,v_expr_body->_sp-1),env,tail);
                code=vector_append(code,code_s->code);
                return new_code(code,code_s->ct);
            } else {
                v1=vector_init(2);v2=vector_init(1);
                push(v1,(void*)new_ast(AST_ARG_LIST,OBJ_NONE, d_arg_v));printf("##########\n<<<v1>>>\n");ast_print(new_ast(AST_ARG_LIST,OBJ_NONE,d_arg_v),0);//dummy arg list
                //push(v2,new_ast(AST_EXP_LIST,((ast*)vector_ref(v_expr_body,v_expr_body->_sp-1))->o_type,v_expr_body));
                push(v2,new_ast(AST_EXP_LIST,OBJ_NONE,v_expr_body));printf("##########\n<<<v2>>>\n");ast_print(new_ast(AST_EXP_LIST,OBJ_NONE,v_expr_body),0);
                //push(v1,(void*)new_ast(AST_ML,((ast*)vector_ref(v_expr_body,v_expr_body->_sp-1))->o_type,v2));//ast_print(new_ast(AST_ML,v5),0) ;      // v3:[AST_EXP_LIST,AST_ML]
                push(v1,(void*)new_ast(AST_ML,OBJ_NONE,v2));//ast_print(new_ast(AST_ML,OBJ_NONE,v2),0) ;      // v3:[AST_EXP_LIST,AST_ML]
                //a1=new_ast(AST_LAMBDA,((ast*)vector_ref(v_expr_body,v_expr_body->_sp-1))->o_type,v1);ast_print(a1,0);
                a1=new_ast(AST_LAMBDA,OBJ_NONE,v1);printf("########\n<<<a1>>>\n");ast_print(a1,0);//ここまでOK!
                //
                v3=vector_init(3);
                push(v3,(void*)a1);
                push(v3,(void*)new_ast(AST_EXP_LIST,OBJ_NONE,a_arg_v));//ast_print(new_ast(AST_EXP_LIST,OBJ_NONE,a_arg_v),0);   // actual arg list
                a2=new_ast(AST_FCALL,a1->o_type,v3);//ast_print(a2,0);                   // AST_FCALL [AST_LAMBDA [AST_EXP_LIST [..],AST_ML [...]],AST_EXP_LIST [...]]
                return codegen(a2,env,tail);
            }
        case AST_IF:    // AST_IF,[cond_expr,true_expr,false_expr]
                //printf("if in!\n");
                code_s1 = codegen(vector_ref(a -> table, 0),env,FALSE);
                code=code_s1->code;                                             // make cond_code 
                code_s1 = codegen(vector_ref(a->table,1),env,TRUE);             // make true_code
                code1=code_s1->code;ct1=code_s1->ct;
                code_s1 = codegen(vector_ref(a->table,2),env,TRUE);             // make false_code
                code2=code_s1->code;ct2=code_s1->ct;
                if (tail) push(code,(void*)TSEL); else push(code,(void*)SEL);   

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
        case AST_SET:// AST_SET [set_type, AST_left_expr, AST_right_expr]
                switch(((ast*)vector_ref(a->table,1))->type) {
                    case AST_VREF:  // AST_SET [set_type, AST_VREF [vect_expr,index] ], right_expr]
                                    //          <0,0>               <1,0>     <1,1>    <2>
                                    // -> right_code vect_name_code index_code VSET
                        //right exprをcode化
                        code_s=codegen(vector_ref(a->table,2),env,FALSE);
                        code=code_s->code;ct=code_s->ct;;
                        //left exprをcode化
                        code_s=codegen(vector_ref(a->table,1),env,FALSE);
                        code1=code_s->code;ct1=code_s->ct;
                        //vector_set(code1,code1->_sp-1,(void*)VSET);
                        i=(long)pop(code1);
                        if ((i==REF || i==OREF) && ct->type != OBJ_GEN ) {
                           push(code,(void*)conv_op[ct->type][OBJ_GEN]);
                        } else if (i==SREF && ct->type != OBJ_SYM ) {
                           //push(code,(void*)conv_op[ct->type][OBJ_SYM]);
                            printf("SyntaxError:Must be Symbol!\n");return NULL;
                        }
                        if (i==REF) {
                            push(code1,(void*)VSET);
                            return new_code(vector_append(code,code1),new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
                        } else if (i==SREF) {
                            push(code1,(void*)SSET);
                            return new_code(vector_append(code,code1),new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));
                        } else {
                            push(code1,(void*)OSET);
                            return new_code(vector_append(code,code1),new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
                        }
                    case AST_FCALL: // AST_SET [set_type,AST_FCALL [expr_name , expr_list],right_expr]
                                    //          <0,0>              <1,0>        <1,1>      <2>
                        a1=(ast*)vector_ref(((ast*)vector_ref(a->table,1))->table,0);                       //a1:expr name
                        if (a1->type != AST_VAR) {printf("SyntaxError:Must be Function Name!\n");return NULL;}                            
                        s=(Symbol * )vector_ref(a1->table,0);                                               //dcl宣言式を作成する
                        
                        v1=vector_init(2);
                        a0=(ast*)vector_ref(((ast*)vector_ref(a->table,1))->table,1);                        //a:expr_list
                        push(v1,(void*)a0);                      //push expr_list
                        push(v1,(void*)vector_ref(a->table,2)); //push righ expr
                        a2=new_ast(AST_LAMBDA,((ast*)vector_ref(a->table,2))->o_type,v1);
                        v2=vector_init(2);push(v2,(void*)(long)'=');
                        push(v2,(void*)a1);push(v2,(void*)a2);ast_print(new_ast(AST_SET,a2->o_type,v2),0);
                        return codegen(new_ast(AST_SET,a2->o_type,v2),env,FALSE);
                    case AST_VAR:   // AST_SET [set_type, AST_VAR [var_name], right_expr]
                                    //          <0,0>             <1,0>       <2>
                        code_s=codegen((ast*)vector_ref(a->table,2),env,FALSE);
                        code=code_s->code;ct2=code_s->ct;
                        //if (type2==OBJ_UFUNC) {v2=code_s->arg_type;r_type2=code_s->function_r_type;dot2=code_s->dotted;}
                        s=(Symbol*)vector_ref(((ast*)vector_ref(a->table,1))->table,0);
                        _pos=var_location(s,env);
                        if (_pos) {
                            pos=(Vector*)vector_ref(_pos,0);
                            ct1=(code_type*)(long)vector_ref(_pos,1);   //ct1は変数名のコードタイプ
                            if (ct1->type==OBJ_NONE || (ct1->type == OBJ_UFUNC && ct1->functon_ret_type ==0 )) {
                                ct1=ct2;
                                //envに保存してある戻り型情報がない関数型変数のコードタイプをct2で置き換える
                                i=(env->_sp)-(long)vector_ref(pos,0)-1;j=(long)vector_ref(pos,1);   //env内のポジションを計算して
                                d=(Data*)malloc(sizeof(Data));d->key=s;d->val=ct1;                  //入れるべきデータを作って
                                vector_set((Vector*)vector_ref(env,i),j,(void*)d);printf("||changed||\n");env_print(env);                  //セットする
                            } else if (ct1->type != ct2->type) {
                                if (conv_op[ct2->type][ct1->type]==0) {printf("SyntaxError:CanotConvertType!\n");return NULL;}
                                push(code,(void*)conv_op[ct2->type][ct1->type]);
                            }

                            push(code,(void*)SET);push(code,(void*)pos);
                        } else {
                            if ((ct1=get_gv(s))==NULL) {            //変数テーブルに名前がないなら
                                //printf("SyntaxError:variable not defined!");return NULL;
                                put_gv(s,ct2);ct1=ct2;              //新たにct2=右辺値のコードタイプで変数を作り左辺値のコードタイプは右辺値と同一
                            } else if (ct1->type==OBJ_NONE || (ct1->type == OBJ_UFUNC && ct1->functon_ret_type ==0 )) {
                                ct1=ct2;put_gv(s,ct1);
                                //if (ct2->type==OBJ_UFUNC) {gvt->type=OBJ_UFUNC;gvt->arg_type=v;gvt->functon_ret_type=r_type;put_gv(s,gvt);}
                            }else if (ct1->type != ct2->type) push(code,(void*)conv_op[ct2->type][ct1->type]);
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
        case AST_LAMBDA:    // AST_LAMBDA [AST_EXP_LIST [expr,   exp,   ...]], body_expr]
                            //                           <0,0,0>,<0,0,1>,...   <1>
            args=vector_init(3);
            v=vector_init(3);
            a1=(ast*)vector_ref(a->table,0);    //arg_list
            if ((a1->type != AST_ARG_LIST) && (a1->type != AST_ARG_LIST_DOTS)) {printf("SyntaxError:not argment list!\n");return NULL;}
            for(i=0;i<a1->table->_sp;i++) {
                a2=(ast*)vector_ref(a1->table,i);// ast_print(a2,0);
                if (a2->type==AST_VAR) {
                    d=(Data*)malloc(sizeof(Data));
                    d->key=(Symbol*)vector_ref(a2->table,0);
                    //d->val=(void*)a2->o_type;
                    d->val=new_ct(a2->o_type,OBJ_NONE,(void*)0,FALSE);
                    push(args,(void*)d);push(v,(void*)a2->o_type);
                } else {printf("illegal argment!\n");}
            } //for(i=0;i<args->_sp;i++) printf("%s\t",((Symbol*)vector_ref(args,i))->_table);printf("\n");
            if (a1->type==AST_ARG_LIST_DOTS) {
                d=(Data*)malloc(sizeof(Data));
                    d->key=new_symbol("..",2);printf("%s\n",d->key->_table);
                    d->val=OBJ_NONE;
                push(args,(void*)d);
            } vector_print(v);
            push(env,(void*)args);//PR(1);
            code_s=codegen((ast*)vector_ref(a->table,1),env,TRUE);//PR(2);
            code1=code_s->code;ct1=code_s->ct;
            push(code1,(void*)RTN);
            push(code,(void*)LDF);push(code,(void*)code1);
            pop(env);// !!don't forget!!!
            code_s= new_code(code,new_ct(OBJ_UFUNC,ct1->type,v,0));//code_s->arg_type=v;code_s->function_r_type=type1;
            if (a1->type==AST_ARG_LIST_DOTS) code_s->ct->dotted=1;
            return code_s;
        case AST_DCL:   // AST_DCL [AST_EXPR_LIST [exp1,exp2...]]
                        //          <0>            <0,0>,<0,1>
            a1=a;
            for(j=0;j<((ast*)vector_ref(a1->table,0))->table->_sp;j++) {
                a2 = (ast*)vector_ref(((ast*)vector_ref(a1->table,0))->table,j);//ast_print(a2,0);
                if (a2->type==AST_VAR) {
                    if (get_gv(s=(Symbol*)vector_ref(a2->table,0))!=NULL) {printf("Warning!:DupricateDifinition!\n");}  //警告を出して続行
                    // printf("!!!!\n");
                    ct=new_ct(a1->o_type,OBJ_NONE,(void*)0,FALSE);
                    put_gv(s,ct);
                    push(code,(void*)LDC);push(code,create_zero(a1->o_type));push(code,(void*)GSET);push(code,(void*)s);push(code,(void*)DROP);//PR(12);
                } else if (a2->type==AST_SET &&                                 // a2: AST_SET [set_type, AST_VAR [var_name], expr_ast]
                            ((ast*)vector_ref(a2->table,1))->type==AST_VAR) {   //                        <1>                 <2>
                    //変数名を取り出してSymbol*sに取っておき、定義済(GVにある)ならエラー
                    s = (Symbol*)vector_ref(((ast*)vector_ref(a2->table,1))->table,0);
                    if (get_gv(s) !=0) printf("Warning!:DupricateDifinition!\n");//警告を出して続行
                    // 右辺式のコードを生成
                    code_s = codegen((ast*)vector_ref(a2->table,2),env,FALSE);
                    code=vector_append(code, (void*)code_s->code);ct=code_s->ct;
                    // 変数名sに型を設定する
                    // ここにおいて右辺値の型はct、dcl宣言された型は1->o_typeである
                    if (a1->o_type==OBJ_UFUNC || a1->o_type==OBJ_PFUNC) {       //型宣言詞がfunctionの場合
                        if (ct->type==OBJ_UFUNC || ct->type==OBJ_PFUNC) {       //右辺値もfunctionなら正常
                            put_gv(s,ct);                                       //そのまま変数名に割り当てる
                        }
                        printf("SyntaxError:Must be Function!\n");              //そうえないならエラー
                        return NULL;
                    } else {                                                    //型宣言が関数タイプでないときは
                            put_gv(s,new_ct(a1->o_type,OBJ_NONE,(void*)0,FALSE));   //宣言された型を変数名sに割り当てるが
                            if (a1->o_type != ct->type) {                       //右辺値の型と宣言型が異なる場合には型変換命令を入れる
                                push(code, (void*)conv_op[ct->type][a1->o_type]);
                                ct->type=a1->o_type;                             //返す型は型返還後のもの、すなわち宣言型する
                            }
                    }
                    push(code,(void*)GSET);push(code,(void*)s);
                    push(code,(void*)DROP);
                } else {
                    printf("Syntax error :decliation in ml_expr\n");
                    return NULL;
                }
            }
            pop(code);
            return  new_code(code,ct);
        case AST_FCALL:     // AST_FCALL [AST_NAME,[AST_EXP_LIST [AST,AST,...]]]
                            //            <0>                     <1,0>,<1,1>...
            // ... macro function ...
            a1=(ast*)vector_ref(a->table,1);    // parameter ast
            n = a1->table->_sp;//PR(3);         // number of actual parameters
            a2=(ast*)vector_ref(a->table,0);    // function ast
            // 「..」だったら即APPLYに変換する
            if (a1->type==AST_EXP_LIST_DOTS) {                      // if expr_list_dots -> apply
                v1=vector_init(1+a1->table->_sp);
                push(v1,(void*)vector_ref(a->table,0));
                for(i=0;i<a1->table->_sp;i++) {
                    push(v1,(void*)vector_ref(a1->table,i));
                }
                v2=vector_init(1);
                push(v2,(void*)new_ast(AST_EXP_LIST_DOTS,((ast*)vector_ref(a1->table,a1->table->_sp-1))->o_type,v1));
                return codegen(new_ast(AST_APPLY,((ast*)vector_ref(a1->table,a1->table->_sp-1))->o_type,v2),env,tail);     // AST_APPLY [AST_EXP_LIST [expr0,expr1,...]]
            }
            // 一般の場合
            // function astのコード生成
            code_s=codegen(a2,env,FALSE);code2=code_s->code;//PR(0);            
            if (code_s->ct->type != OBJ_UFUNC) {printf("SyntaxError:Must be Function!\n");return NULL;}
            r_type = code_s->ct->functon_ret_type;//PR(1);
            v=code_s->ct->arg_type;//vector_print(v);PR(2);                 // list of dummy parameter type
            if (r_type != OBJ_NONE) {
                m = v->_sp;//PR(2);                       // number of dummy parameters
                //n = a1->table->_sp;//PR(3);                 // number of actual parameters
                if (!(dot=code_s->ct->dotted) && n != m) {printf("SyntaxError: Illegal parameter number!\n");return NULL;}
                for(i=0;i<n;i++) {PR(i);
                    code_s = codegen((ast*)vector_ref(a1->table,i),env,FALSE);
                    code1 = code_s->code;ct1=code_s->ct;type1=ct1->type;                            // ct1/type1:actual parameter type
                    if (dot && i >= m-1) type2=OBJ_GEN ; else type2 = (int)(long)vector_ref(v,i);  // ct2/type2:dummy parameter type
                    if ((type1 != type2 ) && (type1 != OBJ_NONE)) push(code1,(void*)conv_op[type1][type2]);
                    code=vector_append(code,code1);
                }
                code = vector_append(code, code2);    // append Function name % expr_list
                push(code, tail ? (void*)TCALL : (void*)CALL);
                push(code, (void*)(long)n);
                return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
            } else { // r_typeが設定されていない≒詳細未定義の関数→再起関数の場合は型情報は後で（関数セット時に）入れる
                     //                                           →高階関数の引数の場合は型はすべてgenとするしかない
                     //                                            ※再起関数か高階関数の引数かを判定しなければならない!!!
                //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                v=vector_init(3);
                for(i=0;i<n;i++){
                    code_s = codegen((ast*)vector_ref(a1->table,i),env,FALSE);
                    code1 = code_s->code;ct1=code_s->ct;type1=ct1->type;                            // ct1/type1:actual parameter type
                    //if (dot && i >= m-1) type2=OBJ_GEN ; else type2 = (int)(long)vector_ref(v,i);  // ct2/type2:dummy parameter type
                    //if (type1 != type2) push(code1,(void*)conv_op[type1][type2]);
                    push(v,(void*)type1);
                    code=vector_append(code,code1);
                }
                code = vector_append(code, code2);    // append Function name % expr_list
                push(code, tail ? (void*)TCALL : (void*)CALL);
                push(code, (void*)(long)n);
                return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));

            }
        case AST_APPLY: // AST_APPLY [AST_EXP_LIST [ast1, ast2, ...]]
                        //            <0>           <0,0>,<0,1e>,...    
            n=((ast*)vector_ref(a->table,0))->table->_sp;//printf("%d\n",n);
            for(i=0;i<n;i++) {
                code_s = codegen((ast*)vector_ref(((ast*)vector_ref(a->table,0))->table,i),env,FALSE);//disassy(code,0);
                code1 = code_s->code;ct1=code_s->ct;type1 = ct1->type;
                code = vector_append(code,code1);//disassy(code,0);
                if (i==0) {
                    if (type1 != OBJ_UFUNC) {printf("SyntaxError:Must be Function!\n");return NULL;}
                    r_type = ct1->functon_ret_type;
                    //code = vector_append(code,code1);//disassy(code,0);
                } else if (i==n-1) {    //最後の引数はtypeがvectorであることを確認しそのままcodeにする
                    if ( type1 != OBJ_VECT) {printf("SyntaxError:Must be Vector!\n");return NULL;}
                    //code = vector_append(code,code1);//disassy(code,0);
                } else if (type1 != OBJ_GEN) {  //最初でも最後でもなければtypeを汎用型に変換
                    //code = vector_append(code,code1);//disassy(code,0);
                    push(code,(void*)conv_op[type1][OBJ_GEN]);
                }
            }
            if (tail) {
                push(code, (void*)TAPL);push(code, (void*)(long)n);
            } else {
                push(code, (void*)APL);push(code, (void*)(long)n);
            }
            return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
        case AST_2OP://PR(1);   // AST_2OP [op_type,AST_L_EXPR,AST_R_EXPR]
            code_s = codegen((ast*)vector_ref(a->table,1),env,FALSE);   //左辺式をコンパイルする
            code1=code_s->code;ct1=code_s->ct;type1=ct1->type;//PR(2);  //code1:左辺式のコード　ct1:左辺式のct type1:左辺式のコードタイプ
            code_s = codegen((ast*)vector_ref(a->table,2),env,FALSE);   //右辺式をコンパイルする
            code2=code_s->code;ct2=code_s->ct;type2=ct2->type;          //code2:右辺式のコード　ct2:右辺式のct type2:右辺式のコードタイプ
            lit_type=(int)(long)vector_ref(a->table,0);                 //2項演算子
            //特別な場合を先に処理
            //vect*y、string*yでy
            if ((type1==OBJ_VECT || type1 == OBJ_SYM ) && lit_type=='*' ) {
                if (type2 != OBJ_INT)  push(code2,(void*)conv_op[type2][OBJ_INT]);//printf("%d %d %d %s\n",type2,type1,conv_op[type2][type1],code_name[conv_op[type2][type1]]);
                r_type=type1;
                code=vector_append(code1,code2);
                push(code,type1==OBJ_VECT ? (void*)VMUL : (void*)SMUL);
                return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
            } else if ((type1 == OBJ_VECT || type1 == OBJ_GEN ) && lit_type == '<'*256+'-' ) {
                if (type2 != OBJ_GEN) push(code2,(void*)conv_op[type2][OBJ_GEN]);//printf("%d %d %d %s\n",type2,type1,conv_op[type2][type1],code_name[conv_op[type2][type1]]);
                //r_type=OBJ_GEN;
                r_type=type1;
                code=vector_append(code1,code2);
                push(code,(void*)(long)(type1==OBJ_VECT ? VPUSH : OPUSH));
                return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
            } else if (lit_type=='>'*256+'>' || lit_type == '<'*256+'<') {
                r_type=type1;
                if (type2 != OBJ_INT) push(code2,(void*)conv_op[type2][OBJ_INT]);
            } else if (type1 < type2) {
                push(code1,(void*)conv_op[type1][type2]);//printf("%d %d %d %s\n",type1,type2,conv_op[type1][type2],code_name[conv_op[type1][type2]]);
                r_type=type2;
            } else if (type1>type2) {
                push(code2,(void*)conv_op[type2][type1]);//printf("%d %d %d %s\n",type2,type1,conv_op[type2][type1],code_name[conv_op[type2][type1]]);
                r_type=type1;
            } else r_type=type1;
            //printf("%d\n",ret_obj);
            code=vector_append(code1,code2);//disassy(code,0,stdin);
            for(i=0;i<=17;i++) {
                if (op2_1[i]==(int)(long)vector_ref(a->table,0)) break;
            }
            if (i>=18) {printf("illegal 2oprand\n");return NULL;}
            push(code,(void*)(long)op2_2[r_type][i]);
            if (op2_3[i] != 0) r_type=op2_3[i];
            // printf("ret_type:%d\n",ret_obj);
            return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
        case AST_1OP: // AST_1OP [op_type,AST_EXPR]
            code_s = codegen((ast*)vector_ref(a->table,1),env,FALSE);
            code=code_s->code;r_type=code_s->ct->type;
            for(i=0;i<6;i++) {
                if (op1_1[i]==(int)(long)vector_ref(a->table,0)) break;
            }
            if (i>=6){printf("illegal 2oprand\n");return NULL;}
            if ((opcode=op1_2[r_type][i])==0) {printf("syntax Error:operator is not supported\n");return NULL;}
            push(code,(void*)(long)opcode);
            if (op1_3[i] != 0) r_type=op1_3[i];
            if (op1_1[i]=='-'*256+'>' && r_type==OBJ_SYM) r_type=OBJ_SYM;
            return new_code(code,new_ct(r_type,OBJ_NONE,(void*)0,FALSE));
        case AST_VREF:  // AST_VREF [AST_vect, AST_expr_list[i1,i2,...]]
                        //           <0>                     <1,0>,<1,1>,
            code_s = codegen(((ast*)vector_ref(a->table,0)), env,FALSE);    // vector指示部をコンパイル
            code=code_s->code;ct=code_s->ct;type1=ct->type;
            if (type1 != OBJ_VECT && type1 != OBJ_SYM && type1 != OBJ_GEN) {printf("Syntax Error:must be vector!\n");return NULL;}
            code_s = codegen((ast*)vector_ref(((ast*)vector_ref(a->table,1))->table,0), env,FALSE);
            code = vector_append(code,code_s->code);
            if (code_s->ct->type != OBJ_INT) push(code,(void*)conv_op[code_s->ct->type][OBJ_INT]);
            //disassy(code,0,stdout);//ok
            //push(code,(void*)(long)(REF ? type1==OBJ_VECT : SREF));
            if (type1==OBJ_VECT) {
                push(code,(void*)REF);
                return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
            } else if (type1==OBJ_SYM) {
                push(code,(void*)SREF);
                return new_code(code,new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));
            } else {
                push(code,(void*)OREF);
                return new_code(code,new_ct(OBJ_GEN,OBJ_NONE,(void*)0,FALSE));
            }
        case AST_SLS:   // AST_SLS [AST_vect,AST_PAIR_LIST [AST_PAIR[car_ast,cdr_ast],AST_PAIR[...],...]]
                        //          <0>      <1>            <1,0>    <1,0,0> <1,0,1>          <1,1>
            code_s = codegen(((ast*)vector_ref(a->table,0)), env,FALSE);    // vector指示部をコンパイル
            code=code_s->code;ct=code_s->ct;type1=ct->type;
            if (type1 != OBJ_VECT && type1 != OBJ_SYM && type1 != OBJ_GEN) {printf("Syntax Error:must be vector!\n");return NULL;}
            code_s = codegen((ast*)vector_ref(((ast*)vector_ref(((ast*)vector_ref(a->table,1))->table,0))->table,0), env,FALSE);
            code = vector_append(code,code_s->code);
            if (code_s->ct->type != OBJ_INT) push(code,(void*)conv_op[code_s->ct->type][OBJ_INT]);
            code_s = codegen((ast*)vector_ref(((ast*)vector_ref(((ast*)vector_ref(a->table,1))->table,0))->table,1), env,FALSE);
            code=vector_append(code,code_s->code);
            if (code_s->ct->type != OBJ_INT) push(code,(void*)conv_op[code_s->ct->type][OBJ_INT]);
            disassy(code,0,stdout);//ok
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
        case AST_VAR:   // AST_VAR [var_symbol]
            _pos=var_location((Symbol*)vector_ref(a->table,0),env);//PR(3333);// printf("var_location OK!!\n");
            if (_pos) {//PR(4444);
                pos=(Vector*)vector_ref(_pos,0);ct=(code_type*)vector_ref(_pos,1);
                push(code,(void*)LD);push(code,(void*)pos);//disassy(code,0,stdout);
            } else {
                s=(Symbol*)vector_ref(a->table,0);
                if (get_gv(s) == NULL) {printf("SyntaxError :Global value not defined!\n");return NULL;}
                ct=get_gv(s);
                push(code,(void*)LDG);push(code,(void*)vector_ref(a->table,0));
            } //disassy(code,0,stdout);//PR(6);
            // ... constant macro ...
            return new_code(code,ct);
        case AST_LIT:   // AST_LIT [lit_type,lit_symbol]
            //printf("###\n");
            lit_type=(tokentype)vector_ref(a->table,0);
            str_symbol=(Symbol*)vector_ref(a->table,1);
            //printf("!!!\n");
            push(code,(void*)LDC);
            switch(lit_type) {
                case TOKEN_NONE:
                    push(code,(void*)0);
                    return new_code(code,new_ct(OBJ_NONE,OBJ_NONE,(void*)0,FALSE));
                case TOKEN_INT:
                    sscanf(str_symbol->_table,"%ld",&int_num);
                    push(code,(void*)int_num);
                    //push(code,(void*)newINT(int_num));
                    //return new_code(code,OBJ_INT);
                    return new_code(code,new_ct(OBJ_INT,OBJ_NONE,(void*)0,FALSE));
                case TOKEN_LINT:
                    z = (mpz_ptr)malloc(sizeof(MP_INT));
                    mpz_set_str(z,str_symbol->_table,10);
                    push(code,(void*)z);//printf("%s\n",objtype2str(OBJ_LINT,(void*)z));
                    //return new_code(code,OBJ_LINT);
                    return new_code(code,new_ct(OBJ_LINT,OBJ_NONE,(void*)0,FALSE));
                case TOKEN_RAT:
                    q = (mpq_ptr)malloc(sizeof(MP_RAT));
                    mpq_init(q);mpq_set_str(q,str_symbol->_table,10);mpq_canonicalize(q);
                    push(code,(void*)q);
                    //return new_code(code,OBJ_RAT);
                    return new_code(code,new_ct(OBJ_RAT,OBJ_NONE,(void*)0,FALSE));
                case TOKEN_STR:
                    push(code,(void*)vector_ref(a->table,1));
                    //return new_code(code,OBJ_SYM);
                    return new_code(code,new_ct(OBJ_SYM,OBJ_NONE,(void*)0,FALSE));
                case TOKEN_FLT: 
                    fl_num_p=(double*)malloc(sizeof(double));
                    sscanf(str_symbol->_table,"%lf",fl_num_p);
                    push(code,(void*)fl_num_p);
                    //push(code,(void*)newFLT(fl_num));
                    //return new_code(code,OBJ_FLT);
                    return new_code(code,new_ct(OBJ_FLT,OBJ_NONE,(void*)0,FALSE));
                case TOKEN_EFLT:
                    fl_num_p=(double*)malloc(sizeof(double));
                    sscanf(str_symbol->_table,"%le",fl_num_p);
                    push(code,(void*)fl_num_p);
                    //push(code,(void*)newFLT(fl_num));
                    //return new_code(code,OBJ_FLT);
                    return new_code(code,new_ct(OBJ_FLT,OBJ_NONE,(void*)0,FALSE));
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
            }
        case AST_VECT:  // AST_VECT [AST_expr_list [AST_v1,AST_v2,...]]
                        //                          <0,0>  <0,1> ...
            if (((ast*)vector_ref(a->table,0))->type == AST_LIT) {
                //push(code,(void*)LDC);push(code,create_zero(OBJ_VECT));
                push(code,(void*)VEC);push(code,(void*)0);
                return new_code(code,new_ct(OBJ_VECT,OBJ_NONE,(void*)0,FALSE));
            } 
            n=((ast*)vector_ref(a->table,0))->table->_sp;//printf("%d\n",n);
            for(i=0;i<n;i++) {
                //code=vector_append(code,codegen(vector_ref(((ast*)vector_ref(a->table,0))->table,i),env,FALSE));
                code_s=codegen(vector_ref(((ast*)vector_ref(a->table,0))->table,i),env,FALSE);
                code1=code_s->code;ct1=code_s->ct;type1=ct1->type;
                if (type1 != OBJ_GEN) push(code1,(void*)conv_op[type1][OBJ_GEN]);
                code=vector_append(code,code1);
            }
            push(code,(void*)VEC);push(code,(void*)(long)n);
            // 
            return new_code(code,new_ct(OBJ_VECT,OBJ_GEN,(void*)0,FALSE));
        default:
            printf("syntaxError:Unknown AST!\n");
            return NULL;
    } 
}

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
            case LDF:   case LDP:
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
            default:
                if (op_size[c] == 0) printf("%s\n", code_name[c]);
                else if (op_size[c] == 1) printf("%s\t%ld\n", code_name[c], (long)dequeue(code)); 
                else printf("Unknkown Command %s\n", (char * )c); 
        }
    }
    indent--;
    for(i= 0; i< indent; i ++ ) printf("\t"); 
    fprintf(fp,"]\n"); 
    code ->_cp = 0;  
} 

extern Vector*tokenbuff;

void * _realloc(void * ptr, size_t old_size, size_t new_size) {
    return GC_realloc(ptr, new_size); 
}

int main(int argc, char*argv[]) {
    void* value;
    ast *a;
    printf("PURE REPL Version 0.02\nCopyright 2021.04.02- M.Taniguro\n\n>>");
    Stream *S;
    int token_p;
    Vector*env;
    Vector*code;
    code_ret *code_s;
    obj_type type; 
    code_type* ct;
    mp_set_memory_functions((void *)GC_malloc, (void * )_realloc, (void * ) GC_free);

    Vector * t; 
    Vector * Stack = vector_init(500000); 
    //Vector * C, * CC ; 
    Vector * Ret = vector_init(500); 
    Vector * Env = vector_init(50); 
    Hash * G = Hash_init(128); // must be 2^n 
    GLOBAL_VAR=Hash_init(128);
    if (argc<=1) S=new_stream(stdin);
    else {
        FILE*fp = fopen(argv[1], "r");
        if (fp == NULL) {printf("file %s doesn't exist\n", argv[1]); return  - 1; }
        S = new_stream(fp); 
    }

        tokenbuff=vector_init(100);
    while (TRUE) {
        //tokenbuff=vector_init(100);
        env=vector_init(10);
        //token_p=tokenbuff->_cp;
        //token_print(tokenbuff);
        if ((a=is_expr(S)) && get_token(S)->type==';') {
            ast_print(a,0);
            code_s = codegen(a,env,FALSE);//PR(121);
            code=code_s->code;push(code,(void*)STOP);//PR(122);
            ct=code_s->ct;type=ct->type;printf("expr type:%d\n",type);
            if (type==OBJ_UFUNC ) {
                    if (ct->functon_ret_type !=0) {printf("arg type:");vector_print(ct->arg_type);printf("ret type: %d\n",ct->functon_ret_type);}
                else {printf("argment & return type not assigned!\n");}
            }
            disassy(code,0,stdout);
            value = eval(Stack,Env,code,Ret,Env,G);
            printf("%s ok\n>>", objtype2str(type,value));
        } else {
            printf("Not expression!\n");
           // tokenbuff->_cp=token_p;
           tokenbuff=vector_init(100);
        }
        //token_print(tokenbuff); 
        //tokenbuff->_cp=0;tokenbuff->_sp=0;
        if (get_token(S)==NULL) exit(0);
        unget_token(S);
    }

}
