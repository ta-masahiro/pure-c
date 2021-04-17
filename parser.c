#include "lexparse.h"

Vector * tokenbuff;

token * get_token(Stream * S) {
    token*t;
    if (is_queu_empty(tokenbuff)) {
        if ((t= _get_token(S))==NULL) return NULL;
        else push(tokenbuff,(void*)t);
    }
    return dequeue(tokenbuff);
}
void unget_token(Stream * S) {
    if ((tokenbuff -> _cp) < 0) printf("Unget_token is too much!\n");
    else (tokenbuff -> _cp) --;
}

void drop_token(Stream*S) {
    pop(tokenbuff);
}
void token_print(Vector * tokenbuff) {
    int i;
    printf("%d:", tokenbuff -> _cp);
    for(i = 0; i<tokenbuff -> _sp; i ++ ) printf("%s  ",((token*)vector_ref(tokenbuff,i))->source->_table);
    printf("\n");
}

ast * new_ast(ast_type type, obj_type o_type,Vector * table) {
    ast * a = (ast * )malloc(sizeof(ast));
    a->type     = type;
    a->o_type   = o_type;
    a ->table   = table;
    return a;
}

char* ast_type_str[] = {"None","MultiFunction","If","Set","Lambda","While","Class","Operator2",
                        "Opraor1","VectorRef","VectorSlice","Literal","Variable","Vector",
                        "Dictionary","ApplyFunction","FunctionCall","Exprlist","CallC/C","Propaeity",
                        "Declear","ExprListDotted","ArgmentList","argmentListDotted","\0"};

void ast_print(ast*a, int tablevel) {
    int i;
    for (i = 0; i<tablevel; i ++ ) printf("\t");
    ast_type t = a->type;
    switch(t) {
        // 1 ast type
        case AST_ML:
            printf("type:ML\t");
            printf("objecttype: %d\n",a->o_type);
            ast_print((ast*)(a ->table->_table[0]), tablevel + 1);
            break;
        // 
        case AST_SET: case AST_2OP:
            printf("type: %s %ld\t",ast_type_str[t], (long)a->table->_table[0]);
            printf("objecttype: %d\n",a->o_type);
            ast_print((ast*)a->table->_table[1], tablevel + 1); ast_print((ast*)a->table->_table[2], tablevel + 1);
            break;
        // ast list type
        case AST_WHILE: case AST_IF: case AST_CLASS: case AST_VREF: case AST_SLS: case AST_VECT: case AST_DICT:
        case AST_DCL:case AST_APPLY: case AST_LAMBDA:case AST_EXP_LIST: case AST_EXP_LIST_DOTS: case AST_ARG_LIST: 
        case AST_ARG_LIST_DOTS:
            printf("type:%s\t", ast_type_str[t]);
            printf("objecttype: %d\n",a->o_type);
            for(i = 0; i<a->table->_sp; i ++) ast_print((ast*)(a ->table->_table[i]), tablevel + 1);
            break;
        case AST_LIT:
            printf("type:LIT\t");
            printf("objecttype: %d\t",a->o_type);if (a->o_type==OBJ_NONE) {printf(" [ NONE ]\n"); break;}
            printf("source type:%ld\tvalue:%s\n", (long)vector_ref(a->table,0),(char * )((Symbol * )vector_ref(a->table, 1))->_table);
            break;
        case AST_VAR:
            printf("type:VAR\t");
            printf("objecttype: %d\t",a->o_type);
            printf("value:%s\n", (char * )((Symbol * )vector_ref(a->table,0))->_table) ;
            break;
        //case AST_2OP:
        //    printf("type:2OP %ld\t", (long)(a->table->_table[0]));
        //    printf("objecttype: %d\n",a->o_type);
        //    ast_print((ast*)a->table->_table[1], tablevel + 1); ast_print((ast*)a->table->_table[2], tablevel + 1);
        //    break;
        case AST_1OP:
            printf("type:1OP %ld\t", (long)(a->table->_table[0]));
            printf("objecttype: %d\n",a->o_type);
            ast_print((ast*)a->table->_table[1], tablevel + 1);
            break;
        case AST_FCALL:
            printf("type:fcall\t");
            printf("objecttype: %d\n",a->o_type);
            ast_print((ast*)a ->table->_table[0], tablevel + 1);
            ast_print((ast*)a ->table->_table[1],tablevel+1);
            break;
        //case AST_EXP_LIST: case AST_EXP_LIST_DOTS: case AST_ARG_LIST: case AST_ARG_LIST_DOTS:
        //    printf("type: %s\t",ast_type_str[t]);
        //    printf("objecttype: %d\n",a->o_type);
        //    for(i=0;i<a->table->_sp;i++) ast_print((ast*)a->table->_table[i],tablevel+1);
        //    break;
        //case AST_EXP_LIST_DOTS:
        //    printf("type:expr_list_dots\t");
        //    printf("objecttype: %d\n",a->o_type);
        //    for(i=0;i<a->table->_sp;i++) ast_print((ast*)a->table->_table[i],tablevel+1);
        //    break;
    }
}
ast *  is_lit(Stream*S) {
    Vector * v;
    ast *a;
    token * t = get_token(S);
    switch(t->type) {
        case TOKEN_INT:case TOKEN_HEX: case TOKEN_OCT: case TOKEN_BIN:
            v = vector_init(3);
            push(v, (void*)(long)t->type); push(v, (void*)t->source);
            a=new_ast(AST_LIT,OBJ_INT,v);
            return a;
        case TOKEN_LINT:
            v = vector_init(3);
            push(v, (void*)(long)t->type); push(v, (void*)t->source);
            a=new_ast(AST_LIT,OBJ_LINT,v);
            return a;
        case TOKEN_RAT:
            v = vector_init(3);
            push(v, (void*)(long)t->type); push(v, (void*)t->source);
            a=new_ast(AST_LIT,OBJ_RAT,v);
            return a;
        case TOKEN_FLT: case TOKEN_EFLT:
            v = vector_init(3);
            push(v, (void*)(long)t->type); push(v, (void*)t->source);
            a=new_ast(AST_LIT,OBJ_FLT,v);
            return a;
        case TOKEN_STR:
            v = vector_init(3);
            push(v, (void*)(long)t->type); push(v, (void*)t->source);
            a=new_ast(AST_LIT,OBJ_SYM,v);
            return a;
        case TOKEN_CHR:
            v = vector_init(3);
            push(v, (void*)(long)t->type); push(v, (void*)t->source);
            a=new_ast(AST_LIT,OBJ_SYM,v);
            return a;
        default:
            unget_token(S);
            return NULL;
    }
}

ast * is_var(Stream *S) {
    Vector * v;
    token * t = get_token(S);
    if (t -> type == TOKEN_SYM ) {
        v = vector_init(1);
        push(v, (void*)t->source);//printf("!!!\n");
        return new_ast(AST_VAR, OBJ_NONE,v);
    }else {
        unget_token(S);
        return NULL;
    }
}

ast * is_nomad(Stream *S) {
    ast*a;
    //printf("!!!\n");
    if (a=is_lit(S)) return a;
    if (a=is_var(S)) return a;
    return NULL;
}

ast * is_factor(Stream*S) {
    // factor       : nomad
    //              | '(' expr ')'
    //              | '{' ml_expr_list '}'
    //              | '{' pair_list '}'
    //              | '[' expr_list ']'
    ast*a;
    tokentype t;
    Vector *v;
    int token_p=tokenbuff->_cp;

    if (a=is_nomad(S)) return a;
    if ((t=get_token(S)->type)=='(') {
        if ((a=is_expr(S)) || (a=is_expr_list(S))) {
            if ((get_token(S)->type)==')') {
                return a;
            }
            //error()
        }
    } else if (t=='[') {
        if (a=is_expr_list(S)) {
            if (get_token(S)->type==']') {
                v=vector_init(1);
                a->o_type=OBJ_GEN;
                push(v,(void*)a);
                return new_ast(AST_VECT,OBJ_VECT,v);
            }
        }
    } else {
        unget_token(S);
        if (a=is_ml_expr(S)) {
            return a;
        }
    }
    tokenbuff->_cp=token_p;
    return NULL;
}
ast * is_expr_list(Stream * S) {
    // expr_list    : expr ',' expr_list
    //              | expr
    ast * a1, * a2;
    token * t1, * t2;
    Vector * v;
    int token_p = tokenbuff ->_cp;

    if (a1 = is_expr(S)) {
        v=vector_init(3);
        while (TRUE) {
            if ((t1 = get_token(S)) ->type != ',') {
                unget_token(S);
                push(v,(void*)a1);
                return new_ast(AST_EXP_LIST,OBJ_NONE,v);
            }
            push(v,(void*)a1);
            if (a1 = is_expr(S)) {
                ;
            } else {
                printf("Syntax error in expr_list\n");
                return NULL;
            }
        }
    }
    tokenbuff -> _cp = token_p;
    return NULL;
}

ast * is_expr_0(Stream *S) {
    // exp_0        : APPLY '(' expr_list ')'
    //              | factor
    //              | factor '(' expr_list '..' ')'
    //              | factor '(' expr_list ')'
    //              | factor '(' ')'
    //              | factor '[' expr_list ']'
    ast * a1, * a2;
    token*t;
    tokentype t1,t2;
    int token_p = tokenbuff -> _cp,i;
    Vector*v,*v1;
    if ((t=get_token(S))->type==TOKEN_SYM && strcmp(t->source->_table,"apply")==0) {
        if ((get_token(S)->type =='(') && (a1=is_expr_list(S)) && get_token(S)->type==')') {
            v=vector_init(1);push(v,(void*)a1);
            return new_ast(AST_APPLY,OBJ_UFUNC,v);
        }
        printf("Syntax error in apply\n");
        return NULL;
    }
    unget_token(S);
    if (a1 = is_factor(S)) {
        t1 = get_token(S)->type;
        if (t1 !='(' && t1 != '[') {
            unget_token(S);
            return a1;
        }
        if (a2 = is_expr_list(S)) {
            t2 = get_token(S) ->type;
            if ((t1=='(' && t2 == ')') || (t1 == '[' && t2==']')) {
                v = vector_init(2);
                push(v, (void * )a1); push(v, (void * )a2);
                if (t1=='(') {
                    return new_ast(AST_FCALL,OBJ_UFUNC, v);
                }
                return new_ast(AST_VREF,OBJ_UFUNC, v);
            } else if (t1=='(' && t2=='.'*256+'.' && get_token(S)->type==')') {
                v = vector_init(2);
                //push(v, (void * )a1);//expr_list a2の先頭にa1を入れたものをvとすること！！
                //for(i=0;i<a2->table->_sp;i++) {
                //    push(v, (void*)vector_ref(a2->table,i));
                //}
                //v1=vector_init(1);push(v1,new_ast(AST_EXP_LIST,v));
                //return new_ast(AST_APPLY, v1);
                a2->type=AST_EXP_LIST_DOTS;
                push(v, (void * )a1); push(v, (void * )a2);
                return new_ast(AST_FCALL,OBJ_UFUNC, v);
            }
        } else {
            t2 = get_token(S) -> type;
            if (t1=='(' && t2 == ')'){
                v = vector_init(2);
                push(v, (void * )a1); push(v, NULL);
                return new_ast(AST_FCALL,OBJ_UFUNC, v);
            }
        }
        printf("syntax error in function call/vector\n");
        return NULL;
    }
    tokenbuff -> _cp = token_p;
    return NULL;
}
#define max(a,b) (a ? a >= b: b)

ast * is_expr_1(Stream *S) {
    // expr_1       : expr_0
    //              : expr_0 '**' expr_1
    ast*a1,*a2;
    tokentype t;
    int token_p=tokenbuff->_cp;
    Vector*v;
    int op='*'*256+'*';
    if (a1=is_expr_0(S)) {
        if (t=(get_token(S)->type) != op) {
            unget_token(S);
            return a1;
        }
        if (a2=is_expr_1(S)) {
            v=vector_init(3);
            push(v,(void*)(long)op);push(v,(void*)a1);push(v,(void*)a2);
            return new_ast(AST_2OP,max(a1->o_type,a2->o_type),v);
        }
    }
    tokenbuff->_cp=token_p;
    return NULL;
}

ast * is_expr_2(Stream * S) {
    // expr_2       : expr_1
    //              | [-|~|@] expr_1
    ast * a;
    tokentype t;
    int token_p = tokenbuff ->_cp;
    Vector*v;

    if (((t = (get_token(S) -> type)) == '-') || (t == '~') || (t=='@')){
        if (a = is_expr_1(S)) {
            v = vector_init(1);
            push(v,(void*)t);push(v, (void * )a);
            return new_ast(AST_1OP, a->o_type,v);
        }
    } else {
        unget_token(S);
        if (a = is_expr_1(S)) {
            return a;
        }
    }
    tokenbuff -> _cp = token_p;
    return NULL;
}

ast * is_expr_3(Stream * S) {
    // expr_incdec  : expr_2  --
    //              | expr_2  ++
    //              | expr_2
    ast * a;
    tokentype t;
    int token_p = tokenbuff ->_cp;
    Vector*v;

    if (a=is_expr_2(S)) {
        if ((t=get_token(S)->type)=='-'*256+'-' || t== '+'*256+'+') {
            v=vector_init(2);
            push(v,(void*)t);push(v, (void * )a);
            return new_ast(AST_1OP,a->o_type, v);
        } else {
            unget_token(S);
            return a;
        }
    }
    tokenbuff->_cp=token_p;
    return NULL;
}

ast * is_expr_4(Stream * S) {
    // expr_pow : expr_incdec '**' expr_pow
    //          | expr_incdec
    return is_expr_3(S);
}
int* is_in(int*s,void* v) {
    int i=0;//printf("value:%ld\n",(long)v);
    while (TRUE) { //printf("check:%d\n",s[i]);
        if (s[i]==0) return NULL;
        if (v==(void*)(long)s[i]) return &s[i];
        i++;
    }
}
int op_type[][7]={  {0}, {0}, {0}, {0}, {0},
    {'*','/','/'*256+'/','%',0},      {'+','-',0}, {'<'*256+'<','>'*256+'>',0},
    {'&',0}, {'^',0}, {'|',0},        {'<','<'*256+'=','>','>'*256+'=','!'*256+'=','='*256+'=',0},
    {'!'*256+'!',0},  {'&'*256+'&',0},{'|'*256+'|',0}
};

ast * is_expr_2n(Stream * S,int n) {
    // expr_n       : expr_n-1
    //              | expr_n-1 op_type expr_n-1 op_type ...
    //
    ast * a1, * a2;
    tokentype t;
    int *tp;
    Vector * v;

    if (n==4) return is_expr_4(S);

    int token_p = tokenbuff -> _cp;
    if (a1 = is_expr_2n(S,n-1)) {
        while (TRUE) {
            tp=is_in(op_type[n],(void*)(long)(get_token(S) -> type) );
            if (! tp) {
                //if ((t=get_token(S)->type) !='<'*256+'<') {
                unget_token(S);
                return a1;
            }
            if (a2 = is_expr_2n(S,n-1)) {
                v = vector_init(3);
                push(v, (void *)(long)(*tp)); push(v, (void * )a1 ); push(v, (void *)a2 );
                a1 = new_ast(AST_2OP, max(a1->o_type,a2->o_type),v) ;
            } else {
                printf("Syntax error! not expression\n");
                return NULL;
            }
        }
        tokenbuff->_cp = token_p;
        return NULL;
    }
}

char*dcl_string[]={"none",   "int",  "long",  "rational","float", "lfloat","var",\
    //                  OBJ_NONE,OBJ_INT,OBJ_LINT,OBJ_RAT,    OBJ_FLT,OBJ_LFLT,OBJ_GEN,
                        "_function","function","cont","vector", "dict",   "pair"   ,"string",    \
    //                  OBJ_PFUNC,  OBJ_UFUNC,OBJ_CNT,OBJ_VECT, OBJ_DICT,  OBJ_PAIR,OBJ_SYM,
                        "file",(void*)0};
    //                  OBJ_IO

int string_isin(char* s,char* table[]) {
    // if is s in table,return table position else -1
    int i=0;
    while (TRUE) {
        if (table[i]==NULL) return -1;
        if (strcmp(s,table[i])==0) return i;
        i++;
    }
    return -1;
}

ast * is_arg_list(Stream * S) {
    // arg_list     : dcl_expr ',' arg_list
    //              | dcl_expr
    ast * a1, * a2;
    token *t,* t1, * t2;
    Vector * v;
    int i,token_p = tokenbuff ->_cp;
    obj_type o_type;
    //if (a1 = is_dcl_expr(S)) {
    if ((t=get_token(S))->type == TOKEN_SYM) {
        if ((i=string_isin(t->source->_table,dcl_string))!=-1) o_type=i; else {unget_token(S);o_type=OBJ_GEN;} 
        if (a1=is_expr(S)) {
            a1->o_type=o_type;
            v=vector_init(3);
            while (TRUE) {
                if ((t1 = get_token(S)) ->type != ',') {
                    unget_token(S);
                    push(v,(void*)a1);
                    return new_ast(AST_ARG_LIST,OBJ_NONE,v);
                }
                push(v,(void*)a1);
                //if (a1 = is_dcl_expr(S)) {
                if ((t=get_token(S))->type==TOKEN_SYM) { 
                    if ((i=string_isin(t->source->_table,dcl_string))!=-1) o_type=i; else {unget_token(S);o_type=OBJ_GEN;} 
                    if (a1=is_expr(S)) {
                        a1->o_type=o_type;
                        continue;
                    }
                } 
                printf("Syntax error in arg_list\n");
                return NULL;
            }
        }
    }
    tokenbuff -> _cp = token_p;
    return NULL;
}

    ast * is_lambda_expr(Stream *S) {
        // lambda_expr  : lambda ( expr_list ..) expr
        //              | lambda ( expr_list ) expr
        //              | lambda ( ) expr
        ast * a1, * a2;
        token*t,*t1 ;
        Vector * v;
        int token_p = tokenbuff->_cp;

        t=get_token(S);
        if ((t->type)==TOKEN_SYM && strcmp("lambda",t->source->_table)==0) {
            if ((get_token(S)->type)=='(') {
                //if (a1=is_expr_list(S)) {
                if (a1=is_arg_list(S)) {                    // check arg_list
                    if ((t1=get_token(S))->type==')'){
                        if (a2=is_expr(S)) {
                            v=vector_init(2);
                            push(v,(void*)a1);push(v,(void*)a2);
                            return new_ast(AST_LAMBDA,a2->o_type,v);
                        }
                    } else if (t1->type=='.'*256+'.' && get_token(S)->type==')') {
                        //a1->type=AST_EXP_LIST_DOTS;
                        a1->type=AST_ARG_LIST_DOTS;
                        if (a2=is_expr(S)) {
                            v=vector_init(2);
                            push(v,(void*)a1);push(v,(void*)a2);
                            return new_ast(AST_LAMBDA,a2->o_type,v);
                        }
                    }
                } else if (get_token(S)->type==')') {
                    if (a2=is_expr(S)) {
                        v=vector_init(2);
                        push(v,(void*)0);push(v,(void*)a2);
                        return new_ast(AST_LAMBDA,OBJ_UFUNC,v);
                    }
                }
            }
            printf("Syntax error in lambda\n");
            return NULL;
        }
        tokenbuff->_cp=token_p;
        return NULL;
    }

    ast*is_if_expr(Stream*S) {
        // if_expr  : if expr : expr : expr
        ast*a1,*a2,*a3;
        token*t;
        Vector*v;
        int token_p=tokenbuff->_cp;

        t=get_token(S);
        if (t->type==TOKEN_SYM && strcmp("if",t->source->_table)==0) {
            if (a1=is_expr(S)) {
                if (get_token(S)->type==':' ) {
                    if (a2=is_expr(S)) {
                        if (get_token(S)->type==':') {
                            if (a3=is_expr(S)) {
                                v=vector_init(3);
                                //if (a2->o_type != a3->o_type) {printf("Syntax error :IFtype\n");return NULL;}
                                push(v,(void*)a1);push(v,(void*)a2);push(v,(void*)a3);
                                return new_ast(AST_IF,a2->o_type,v);
                            } else {
                                printf("Syntax error! must be FALSE expression\n");
                                return NULL;
                            }
                        } else {
                            printf("Syntax error! Must be ':'\n");
                            return NULL;
                        }
                    } else {
                        printf("Syntax error! must be TRUE expression\n");
                        return NULL;
                    }
                } else {
                    printf("Syntax error! Must be ':'\n");
                    return NULL;
                }
            } else {
                printf("Syntax error! Must be cond expression!\n");
                return NULL;
            }
        }
        tokenbuff->_cp=token_p;
        return NULL;
    }

    int set_op[]={'=','*'*256+'=','/'*256+'=','%'*256+'=','+'*256+'=','-'*256+'=',0};

    ast * is_set_expr(Stream * S) {
        // set_expr : expr_0 [=|+=|-=|*=|/=] expr
        ast* a1,*a2;
        tokentype  t;
        Vector*v;
        int token_p = tokenbuff->_cp;
        //if ((a1 = is_expr_0(S)) && (t=*is_in(set_op, (void*)get_token(S)->type)) && (a2 = is_expr(S))) {
        if ((a1=is_expr_0(S)) &&
                ((t=get_token(S)->type)=='=' || t=='+'*256+'=' || t=='-'*256+'=' || t=='*'*256+'=' || t=='/'*256+'=')&&
                (a2=is_expr(S))) {
            v=vector_init(3);
            push(v,(void*)t); push(v,(void*)a1); push(v,(void*)a2);
            return new_ast(AST_SET,a1->o_type,v);
        }
        tokenbuff->_cp=token_p;
        return NULL;
    }

    ast * is_dcl_expr(Stream*S) {
        int i;
        ast*a;
        token*t;
        Vector*v;
        int token_p=tokenbuff->_cp;

        t=get_token(S);
        //if (t->type==TOKEN_SYM && strcmp("var",t->source->_table)==0) {
        if (t->type==TOKEN_SYM && (i=string_isin(t->source->_table,dcl_string))!=-1) {
            if (a=is_expr_list(S)) {
                v=vector_init(1);
                push(v,(void*)a);
                return new_ast(AST_DCL,i,v);
            }
        } //printf("!!!\n");
        tokenbuff->_cp=token_p;
        return NULL;
    }

    ast * is_expr(Stream *S) {
        ast * a;
        if (a = is_dcl_expr(S)) return a;
        if (a = is_set_expr(S)) return a;
        if (a = is_if_expr(S)) return a;
        if (a = is_lambda_expr(S)) return a;
        //if (a = is_expr_6(S)) return a;
        if (a = is_expr_2n(S,14)) return a;
        /* ||
           is_dcl_expr(p)      ||
           is_while_expr(p)    ||
           is_and_or_expr(p)   ||
           return NULL;*/
    }

    ast *  is_ml_expr(Stream * S ) {
        ast * a;
        token * t;
        int token_p = tokenbuff ->_cp;
        Vector*v;
        if (get_token(S) -> type == '{') {
            if ( a = is_ml_expr_list(S) ) {
                if (get_token(S) -> type == '}') {
                    v=vector_init(1);
                    push(v,(void*)a);
                    return new_ast(AST_ML, a->o_type,v);
                }
            }
        }
        tokenbuff -> _cp = token_p;
        return NULL;
    }
    ast * is_ml_expr_list(Stream * S) {
        // expr_list    : expr ',' expr_list
        //              | expr
        ast * a1, * a2;
        token * t1, * t2;
        Vector * v;
        int token_p = tokenbuff ->_cp;

        if (a1 = is_expr(S)) {
            v=vector_init(3);
            while (TRUE) {
                if ((t1 = get_token(S)) ->type != ';') {
                    unget_token(S);
                    push(v,(void*)a1);
                    return new_ast(AST_EXP_LIST,a1->o_type,v);
                }
                push(v,(void*)a1);
                if (a1 = is_expr(S)) {
                    ;
                } else {
                    printf("Syntax error in ml_expr_list\n");
                    return NULL;
                }
            }
        }
        tokenbuff -> _cp = token_p;
        return NULL;
    }
    /*
       int main(int argc, char argv[]) {
       ast *a;
       Stream *S=new_stream(stdin);
       int token_p;
       tokenbuff=vector_init(100);

       while (TRUE) {
    //token_p=tokenbuff->_cp;
    //token_print(tokenbuff);
    if ((a=is_expr(S)) && get_token(S)->type==';') ast_print(a,0);
    else {
    printf("Not expression!\n");
    // tokenbuff->_cp=token_p;
    //drop_token(S);
    }
    tokenbuff->_cp=0;tokenbuff->_sp=0;
    }

    }
    */