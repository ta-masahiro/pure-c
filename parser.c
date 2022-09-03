#include "lexparse.h"

void drop_token(TokenBuff *t) {
    pop(t->buff);
}
void token_print(TokenBuff * tokenbuff) {
    int i;
    printf("%d:", tokenbuff->buff -> _cp);
    for(i = 0; i<tokenbuff->buff -> _sp; i ++ ) printf("%s  ",((token*)vector_ref(tokenbuff->buff,i))->source->_table);
    printf("\n");
}

ast * new_ast(ast_type type, obj_type o_type,Vector * table) {
    ast * a = (ast * )malloc(sizeof(ast));
    a->type     = type;
    a->o_type   = o_type;
    a ->table   = table;
    //ast_print(a,0);
    return a;
}

char* ast_type_str[] = {"None","MultiFunction","If","Set","Lambda","While","Class","Operator2",
                        "Opraor1","VectorRef","VectorSlice","Literal","Variable","Vector",
                        "Dictionary","ApplyFunction","FunctionCall","Exprlist","CallC/C","Propaeity",
                        "Declear","ExprListDotted","ArgmentList","argmentListDotted","Pair","PairList","loop","class_var","for","FunctionDeclear","FunctionType",
                        "c_macro", "F_macro", "S_macro", "\0"};

void ast_print(ast*a, int tablevel) {
    if (a == NULL) {printf("ASTがNULLです!!!\n");Throw(1);}
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
        case AST_ARG_LIST_DOTS: case AST_PAIR: case AST_PAIR_LIST: case AST_CLASS_VAR: case AST_FOR: case AST_DCL_F: case AST_FTYPE: case AST_MAC_S:
            printf("type:%s\t", ast_type_str[t]);
            printf("objecttype: %d\n",a->o_type);
            if (a->table==NULL) break;
            for(i = 0; i<a->table->_sp; i ++) {
                if (a->table->_table[i] != NULL) ast_print((ast*)(a ->table->_table[i]), tablevel + 1);
            }
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
        default:printf("Unknown AST: %d\n",t);Throw(1);
    }
}

ast *  is_lit(TokenBuff*S) {
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
        case TOKEN_LEFLT:
            v = vector_init(3);
            push(v, (void*)(long)t->type); push(v, (void*)t->source);
            a=new_ast(AST_LIT,OBJ_LFLT,v);
            return a;
        case TOKEN_STR: case TOKEN_RAW_STR:
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

ast * is_var(TokenBuff *S) {
    Vector * v;
    token * t = get_token(S);
    if (t -> type == TOKEN_SYM ) {
        v = vector_init(1);
        if (strcmp(t->source->_table, "None") == 0 ) {
            push(v, (void *)TOKEN_NONE);
            return new_ast(AST_LIT, OBJ_NONE, v);
        }
        push(v, (void*)t->source);//printf("!!!\n");
        return new_ast(AST_VAR, OBJ_NONE,v);
    }else {
        unget_token(S);
        return NULL;
    }
}

ast * is_nomad(TokenBuff *S) {
    ast*a;
    //printf("!!!\n");
    if (a=is_lit(S)) return a;
    if (a=is_var(S)) return a;
    return NULL;
}

ast * is_factor(TokenBuff *S) {
    // factor       : nomad
    //              | '(' expr ')'
    //              | '{' ml_expr_list '}'
    //              | '{' pair_list '}'
    //              | '[' expr_list ']'
    ast*a;
    token *tkn;
    tokentype t;
    Vector *v;
    int token_p = S->buff->_cp;

    if (a=is_nomad(S)) return a;
    if ((t=get_token(S)->type)=='(') {
        if ((a=is_expr_ex(S))) { //} || (a=is_expr_list(S))) {
            if ((get_token(S)->type)==')') {
                return a;
            }
            //error()
        }
    } else if (t=='[') {
        if (get_token(S)->type == ';') {
            if (get_token(S)->type == '@') {
                if ((tkn = get_token(S))->type == TOKEN_INT) {

                }
                unget_token(S);
            }
            unget_token(S);
        }
        unget_token(S);

        if (get_token(S)->type==']') {   //[]の場合
            v=vector_init(1);
            push(v,(void*)new_ast(AST_LIT,OBJ_NONE,(void*)0));
            return new_ast(AST_VECT,OBJ_VECT,v);
        } else {
            unget_token(S);
            if (a=is_expr_list(S)) {
                if (get_token(S)->type==']') {
                    v=vector_init(1);
                    a->o_type=OBJ_GEN;
                    push(v,(void*)a);
                    return new_ast(AST_VECT,OBJ_VECT,v);
                }
            }
        }
        printf("SyntaxError:Not a exper list or ']' error\n");
        Throw(1);
    } else if (t=='{') {
        if (get_token(S)->type == '}') { // {}の場合
            //v=vector_init(1);
            //push(v,(void*)0);
            return new_ast(AST_PAIR_LIST,OBJ_NONE,(void*)0);
        }
        unget_token(S);  
        if ((a=is_pair_list(S)) && get_token(S)->type=='}') return a;
        else {
            unget_token(S);
            if (a=is_ml_expr(S)) {
                return a;
            }
        }
    }
    S->buff->_cp=token_p;
    return NULL;
}

ast * is_pair(TokenBuff * S) {
    // pair     = expr ':' expr
    //          =>
    // AST_PAIR, none, [left_expr_ast, right_expr_AST]
    ast *a1,*a2;
    token *t;
    Vector * v;
    int token_p = S->buff ->_cp;

    if ((t=get_token(S))->type == ':' && (a1=is_expr(S))) {
        v=vector_init(2);
        push(v,(void*)0);push(v,a1);        
        return new_ast(AST_PAIR,OBJ_NONE,v); 
    }
    unget_token(S);
    if ((a1=is_expr(S)) && (t=get_token(S))->type == ':') {
        v = vector_init(2);
        push(v,a1);
        if ((t=get_token(S))->type == ',' || t->type == ']') {
            unget_token(S);
            push(v,(void*)0);
        } else {
            unget_token(S);
            if (a2=is_expr(S))  push(v,a2);
        }
        return new_ast(AST_PAIR,OBJ_NONE,v); 
    }
    S->buff -> _cp = token_p;
    return NULL;
}

ast * is_pair_list(TokenBuff * S) {
    // pair_list    = pair {',' pair}
    //              =>
    // AST_PAIR_LIST, none, [pair, ..., pair]
    ast * a1, * a2;
    token * t1, * t2;
    Vector * v;
    int token_p = S->buff ->_cp;

    if (a1 = is_pair(S)) {
        v=vector_init(3);
        while (TRUE) {
            if ((t1 = get_token(S)) ->type != ',') {
                unget_token(S);
                push(v,(void*)a1);
                return new_ast(AST_PAIR_LIST,OBJ_NONE,v);
            }
            push(v,(void*)a1);
            if (a1 = is_pair(S)) {
                ;
            } else {
                printf("SyntaxError: pair_list中にあるべきpairが見つかりません\n");
                Throw(1);
            }
        }
    }
    S->buff -> _cp = token_p;
    return NULL;
}
ast * is_expr_list(TokenBuff * S) {
    // expr_list    : expr {',' expr_list}
    //
    ast * a1, * a2;
    token * t1, * t2;
    Vector * v;
    int token_p = S->buff ->_cp;
    //printf("in...is_expr_list\n");
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
                Throw(1);
            }
        }
    }
    S->buff -> _cp = token_p;
    return NULL;
}

ast * is_list_br(TokenBuff * S, char bl, int pair_flg) {
    // blが'('か'['か、およびpair_flgがTRUEかFALSEかで以下を判定しastを作る
    //
    // expr_list_br  = '(' [expr_list] {',' expr_list} ')' 
    // expr_list_bk  = '[' [expr_list] {',' expr_list} ']' 
    // pair_list_bk  = '[' [pair_list] {',' pair_list} ']'
    //
    ast * a1, * a2;
    token * t1;
    Vector * v;
    char br;
    ast * (* is_list)(TokenBuff * S);
    ast_type terget_ast_type;

    if (bl == '(') br = ')';
    else if (bl == '[') br = ']';
    else if (bl == '{') br = '}';

    is_list = (pair_flg) ? is_pair_list : is_expr_list;  
    terget_ast_type  = (pair_flg) ? AST_PAIR_LIST :AST_EXP_LIST;
    
    if ((get_token(S)->type)== bl) {
        //printf("!!!!!!!!!!!!!!!!!!%c %d\n",br,pair_flg );
        if (a1 = is_list(S)) {                                                          // check arg_list
            if ((t1=get_token(S))->type== br){                                          //  '(' exp_list ')' ならばそのままAST_ARG_LISTを返す
                    return a1;                                                          //
            } else if (t1->type=='.'*256+'.' && get_token(S)->type == br) {             //  '(' exp_list '..' ')'ならば 
                if (pair_flg) {printf("SyntaxError:'%c'が必要です\n", br); Throw(1);}
                a1->type=AST_EXP_LIST_DOTS;                                             // type をAST_EXP_LIST_DOTSにして返す
                return a1;
            }
            printf("SyntaxError:'%c'または'..'が必要です\n", br); Throw(1);
        } else if (get_token(S)->type==br) {                                            //引数がない場合
            a1=new_ast(terget_ast_type, OBJ_NONE, vector_init(1));                      //空のexp_listを作り
            return a1;                                                                  // それを返す
        }//「引数リストが)で終わっていない」というエラー
        unget_token(S);
        //printf("SyntaxError:式リストまたは'%c'が必要です\n", br); Throw(1);
    }//「'('で始まらない」というエラー
    //printf("SyntaxError:'('が必要です\n"); Throw(1);
    unget_token(S);
    return NULL;
}

ast * is_arg_list(TokenBuff * S);
ast * is_arg_list_br(TokenBuff * S);

ast * is_expr_0(TokenBuff *S) {
    //
    // ※expr_0以下が形式上左辺式に使える
    //
    // exp_0            = APPLY exp_list_br
    //                  | factor {pair_list_bk}
    //                  | factor {arg_list_br}
    //                  | factor {expr_list_br}
    //                  | factor {expr_list_bk}
    ast * a1, * a2;
    token*t;
    tokentype t1,t2;
    int token_p = S->buff -> _cp,i;
    Vector*v,*v1;
    // applyの処理
    if ((t=get_token(S))->type==TOKEN_SYM && strcmp(t->source->_table,"apply")==0) {
        if ((get_token(S)->type =='(') && (a1=is_expr_list(S)) && get_token(S)->type==')') {
            v=vector_init(1);push(v,(void*)a1);
            return new_ast(AST_APPLY,OBJ_UFUNC,v);
        }
        printf("Syntax error in apply\n");
        Throw(1);
    }
    unget_token(S);
    //
    if (a1 = is_factor(S)) {
        while (TRUE) {
            if (a2 = is_list_br(S,'[',TRUE)) {              // pair_list_bk?...スライス
                v = vector_init(2);
                push(v, (void*)a1);push(v, (void*)a2);
                a1 = new_ast(AST_SLS, OBJ_NONE, v);
                continue;
            } else if (a2 = is_arg_list_br(S)) {            // arg_list_br?...関数定義
                v = vector_init(2);
                push(v, (void*)a1);push(v, (void*)a2);
                a1 = new_ast(AST_FCALL, OBJ_NONE, v);
                continue;
            } else if (a2 = is_list_br(S, '(', FALSE)) {    // expr_list_br?...関数呼び出し
                v = vector_init(2);
                push(v, (void*)a1);push(v, (void*)a2);
                a1 = new_ast(AST_FCALL, OBJ_NONE, v);
                continue;
            } else if (a2 = is_list_br(S, '[', FALSE)) {    // expr_list_bk?...添字参照
                v = vector_init(2);
                push(v, (void*)a1);push(v, (void*)a2);
                a1 = new_ast(AST_VREF, OBJ_NONE, v);
                continue;
            } else return a1;
        }
    }
    S->buff -> _cp = token_p;
    return NULL;
}

ast * is_expr_05(TokenBuff *S) {
    //  expr_05     = expr_0 {'.' expr_0}                   ドット演算子
    //              =>
    //  AST_2OP, '.'type, [left_expr_ast, right_expr_ast]
    ast * a, * a1;
    Vector * v;
    if (a = is_expr_0(S)) {
        while (TRUE) {
            if (get_token(S)->type != '.') {
                unget_token(S);
                return a;
            }
            if (a1 = is_expr_0(S)) {
                v = vector_init(3);
                push(v, (void*)(long)'.'); push(v, (void*)a); push(v, (void*)a1);
                a = new_ast(AST_2OP, OBJ_NONE, v);
            } else {
                printf("Syntax error! not expression\n");
                Throw(1);
            }
        }
    }
    return NULL;
}

#define max(a,b) (a ? a >= b: b)

ast * is_expr_1(TokenBuff *S) {
    // expr_1       = expr_05
    //              | expr_05 '**' expr_1
    //              =>
    // AST_2OP, op_type, [left_expr_ast, right_expr_ast]
    ast*a1,*a2;
    tokentype t;
    int token_p = S->buff->_cp;
    Vector*v;
    int op='*'*256+'*';
    if (a1=is_expr_05(S)) {
        if (t=(get_token(S)->type) != op) {
            unget_token(S);
            return a1;
        }
        if (a2=is_expr_1(S)) {  // 累乗は右結合演算子なので再起で回すと簡単
            v=vector_init(3);
            push(v,(void*)(long)op);push(v,(void*)a1);push(v,(void*)a2);
            return new_ast(AST_2OP,max(a1->o_type,a2->o_type),v);
        }
    }
    S->buff->_cp=token_p;
    return NULL;
}

ast * is_expr_2(TokenBuff * S) {
    // expr_2       = expr_1
    //              | (-|~|@) expr_2
    //              =>
    // AST_1OP, op_type, [expr_ast]
    ast * a;
    tokentype t;
    int token_p = S->buff ->_cp;
    Vector*v;

    if (((t = (get_token(S) -> type)) == '-') || (t == '~') || (t=='@')){
        //if (a = is_expr_1(S)) {
        if (a = is_expr_2(S)) {
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
    S->buff -> _cp = token_p;
    return NULL;
}

ast * is_expr_3(TokenBuff * S) {
    // expr_3       = expr_2 {(--|++|->)}
    //              =>
    // AST_1OP, op_type, [expr_AST]
    ast * a;
    tokentype t;
    int token_p = S->buff ->_cp;
    Vector*v;

    if (a=is_expr_2(S)) {
        while (TRUE) {
            if ((t=get_token(S)->type)!='-'*256+'-' &&  t!= '+'*256+'+'  && t != '-'*256+'>') {
                unget_token(S);
                return a;
            }
            v=vector_init(2);
            push(v,(void*)t);push(v, (void * )a);
            a=new_ast(AST_1OP,a->o_type, v);
        }
    }
    S->buff->_cp=token_p;
    return NULL;
}

int* is_in(int*s,void* v) {
    int i=0;//printf("value:%ld\n",(long)v);
    while (TRUE) { //printf("check:%d\n",s[i]);
        if (s[i]==0) return NULL;
        if (v==(void*)(long)s[i]) return &s[i];
        i++;
    }
}

int op_type[][7]={  {0}, {0}, {0}, {0}, {'<'*256+'-',0},
    {'*','/','/'*256+'/','%',0},      {'+','-',0}, {'<'*256+'<','>'*256+'>',0},
    {'&',0}, {'^',0}, {'|',0},        {'<','<'*256+'=','>','>'*256+'=','!'*256+'=','='*256+'=',0},
    {'!'*256+'!',0},  {'&'*256+'&',0},{'|'*256+'|',0}
};

ast * is_expr_2n(TokenBuff * S,int n) {
    // expr_n       = expr_n-1 {2op expr_n-1}
    //      =>
    // AST_2OP, type, [left_expr_ast, right_expr_ast]
    ast * a1, * a2;
    tokentype t;
    int *tp;
    Vector * v;

    if (n==3) return is_expr_3(S);

    int token_p = S->buff -> _cp;
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
                Throw(1);
            }
        }
        S->buff->_cp = token_p;
        return NULL;
    }
}

char*dcl_string[]=     {"none",     "int",      "long",     "rational", "float",    "lfloat",   "complex",  "var",\
    //                  OBJ_NONE,   OBJ_INT,    OBJ_LINT,   OBJ_RAT,    OBJ_FLT,    OBJ_LFLT,   OBJ_CMPLX,  OBJ_GEN,
                        "__function",   "_function",    "function", "code",\
    //                  OBJ_SYSFUNC,    OBJ_PFUNC,      OBJ_UFUNC,  OBJ_CNT
                        "vector",   "dict",   "pair",   "string",   "array",    \
    //                   OBJ_VECT,  OBJ_DICT,  OBJ_PAIR,OBJ_SYM,    OBJ_ARRAY
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

ast * is_arg_list_br(TokenBuff *);

ast * is_arg(TokenBuff * S, int dcl_flg) {
    // 「型」指定構文かどうかを判定する　※arg_listおよびdeclearで呼ばる
    // arg          = [dcl_string] arg_list_br {arg_list_br}  expr　
    // 
    ast * a, * a1;
    token * t;
    Vector * v;
    int i;
    obj_type o_type;
    ast_type a_type;
    int ftype_flg = FALSE;

    int token_p = S->buff -> _cp;
    if ((t =get_token(S))->type == TOKEN_SYM) {
        if ((i = string_isin(t->source->_table, dcl_string)) != -1) o_type = i; 
        else if (dcl_flg) {S->buff->_cp = token_p;return NULL;}                 // declearから呼ばれたときは型指定文字が必要
        else {unget_token(S); o_type = OBJ_GEN;}                                // それ以外(arg_list)から呼ばれたときは型指定文字を省略可

        if (a = is_arg_list_br(S)) {
            ftype_flg = TRUE;
             v=vector_init(1);
             push(v, a);
            while (TRUE) {
                if ((a = is_arg_list_br(S)) == 0) {
                    //type_ast = new_ast(AST_ARG_LIST, o_type, v);
                    break; 
                }
                push(v, (void * )a);
            }
        }
        if (a1 = is_expr(S)) {
            if (ftype_flg == FALSE) {a1->o_type = o_type; return a1;}
            push(v, a1);
            return new_ast(AST_FTYPE, o_type,v);
        }
        //printf("SyntaxError式が必要です\n"); Throw(1);
    }
    S->buff -> _cp = token_p;
    return NULL;    
}

ast * is_arg_list(TokenBuff * S) {
    // arg_list           = arg {',' arg}
    // 
    ast * a, * a1;
    token *t,* t1, * t2;
    Vector * v;
    int i,token_p = S->buff->_cp;
    obj_type o_type;
    if (a = is_arg(S, FALSE)) {
        v = vector_init(1);
        while (TRUE) {
            if (get_token(S)->type != ',') {
                unget_token(S);
                push(v,  (void*)a);
                return new_ast(AST_ARG_LIST, OBJ_NONE, v);
            }
            push(v, (void*)a);
            if (a = is_arg(S, FALSE)) {
                continue;
            }
            //printf("SyntaxError:型指定式が必要です\n"); Throw(1);
            //S->buff -> _cp = token_p;
            //return NULL;
            break;
        }
    }
    S->buff -> _cp = token_p;
    return NULL;
}

ast * is_arg_list_br(TokenBuff *S) {
    // arg_list_br      = '(' ')'
    //                  | '(' arg_list  ')'
    //                  | '(' arg_list '..' ')'
    ast * a1, * a2;
    token * t1;
    Vector * v;
    if ((get_token(S)->type)=='(') {
        if (a1 = is_arg_list(S)) {                                          // check arg_list
            if ((t1=get_token(S))->type==')'){                              //  '(' arg_list ')' ならばそのままAST_ARG_LISTを返す
                    return a1;                                              //
            } else if (t1->type=='.'*256+'.' && get_token(S)->type==')') {  //  '(' arg_list '..' ')'ならば 
                a1->type=AST_ARG_LIST_DOTS;                                 // type をAST_ARG_LIST_DOTSにして返す
                return a1;
            }
            printf("SyntaxError:')'または'..'が必要です\n"); Throw(1);
        } else if (get_token(S)->type==')') {                               //引数がない場合
            a1=new_ast(AST_ARG_LIST,OBJ_NONE,vector_init(1));               //空のarg_listを作り
            return a1;                                                      // それを返す
        }//「引数リストが)で終わっていない」というエラー
        //printf("SyntaxError:arg listが必要です\n"); Throw(1);
        unget_token(S);
    }//「'('で始まらない」というエラー
    //printf("SyntaxError:'('が必要です\n"); Throw(1);
    unget_token(S);
    return NULL;
}

ast * is_lambda_expr(TokenBuff *S) {
    // lambda_expr  : lambda ( expr_list ..) expr
    //                      | lambda ( expr_list ) expr
    //                      | lambda ( ) expr
    ast * a1, * a2;
    token*t,*t1 ;
    Vector * v;
    int token_p = S->buff->_cp;

    t=get_token(S);
    if ((t->type)==TOKEN_SYM && strcmp("lambda",t->source->_table)==0) {
        if (a1=is_arg_list_br(S)) {                    // check arg_list
            if (a2=is_expr(S)) {
                v=vector_init(2);
                push(v,(void*)a1);push(v,(void*)a2);
                return new_ast(AST_LAMBDA,a2->o_type,v);
            }//「関数本体がない」というerrorにすること
            printf("SynraxError:式が必要です\n"); Throw(1);
        }// 「')'または'...)'で終わっていない」というerrorにすること
        printf("SyntaxError:'arg list'が必要です\n"); Throw(1);
    }
    S->buff->_cp=token_p;
    return NULL;
}

char      *sp_exp1_string[]  = {"lambda",  "macro",     NULL};
ast_type sp_exp1_ast_type[]  = {AST_LAMBDA, AST_MAC_F,      };

ast * is_spcial1_expr(TokenBuff *S) {
    // spi1_exp   = <key_word>  [ expr_list_br ] expr
    //            =>
    // AST_<key_word>, [expr_list, expr]
    ast * a1, * a2;
    ast_type a_type;
    token*t,*t1 ;
    Vector * v;
    int i;
    int token_p = S->buff->_cp;

    if ((t =get_token(S))->type == TOKEN_SYM) {
        if ((i = string_isin(t->source->_table, sp_exp1_string)) != -1) {
            a_type = sp_exp1_ast_type[i];
            if (a1=is_arg_list_br(S)) {                    // check arg_list
                if (a2=is_expr(S)) {
                    v=vector_init(2);
                    push(v,(void*)a1);push(v,(void*)a2);
                    return new_ast(a_type, a2->o_type, v);
                }//「関数本体がない」というerrorにすること
                printf("SynraxError:式が必要です\n"); Throw(1);
            } else if (a1 = is_expr(S))

        
            printf("SyntaxError:'arg list'が必要です\n"); Throw(1);
        }
    }
    S->buff->_cp=token_p;
    return NULL;
}

char      *sp_exp_string[]  = {"if",  "while",   "for",   "loop",    "macro",  NULL};
ast_type sp_exp_ast_type[]  = {AST_IF, AST_WHILE, AST_FOR, AST_LOOP, AST_MAC_S};

ast * is_special_expr(TokenBuff *S, char delm) {
    // sp_exp   = <key_word> expr {':' expr}
    //          =>
    // AST_<key_word>, none, [expr_ast, ..., expr_ast]
    token * t;
    int i;
    ast * a, * a1;
    ast_type a_type;
    Vector * v = vector_init(1);
    int token_p = S->buff -> _cp;

    if ((t =get_token(S))->type == TOKEN_SYM) {
        if ((i = string_isin(t->source->_table, sp_exp_string)) != -1) {
            a_type = sp_exp_ast_type[i];
            if (a = is_expr(S)) {
                push(v, (void*)a);
                while (TRUE) {
                    if (get_token(S)->type != delm) {
                        unget_token(S);
                        //push(v, (void *)a);
                        return new_ast(a_type, OBJ_NONE, v);
                    }
                    if (a = is_expr(S)) {
                        push(v, (void*)a);
                        continue;
                    }
                }
            }
        }
    }
    S->buff->_cp=token_p;
    return NULL;
}
ast * is_set_expr(TokenBuff * S) {
    // set_expr = expr_0 ('='|'+='|'-='|'*='|'/='|'%='|'|='|'&='|'^='|'>>='|'<<=') expr
    //      =>
    // AST_SET,[type of set, ast of left_expr, ast of right_expr]
    ast* a1,*a2;
    tokentype  t;
    Vector*v;
    int token_p = S->buff->_cp;

    if ((a1=is_expr_0(S)) &&      ((t=get_token(S)->type)=='=' ||
            t=='+'*256+'=' || t=='-'*256+'=' || t=='*'*256+'=' || t=='/'*256+'=' || 
            t=='%'*256+'=' || t=='|'*256+'=' || t=='&'*256+'=' || t=='^'*256+'=' ||
            t=='>'*65536+'>'*256+'=' || t== '<'*65536+'<'*256+'=') && (a2=is_expr(S))) {
        v=vector_init(3);
        push(v,(void*)t); push(v,(void*)a1); push(v,(void*)a2);
        return new_ast(AST_SET,a1->o_type,v);
    }
    S->buff->_cp=token_p;
    return NULL;
}

ast * is_dcl_expr(TokenBuff*S) {
    // dcl_expr = arg [','expr_list]
    //      =>
    // AST_DCL,type of arg,[AST_EXP_LIST]
    ast * a, * a1;
    token*t;
    Vector*v = vector_init(1);
    int token_p = S->buff->_cp;

    if (a = is_arg(S, TRUE)) { //printf("arg...ok\n");
        //
        if (get_token(S)->type == ',') {
            if (a1 = is_expr_list(S)) {
                // expr_listの各exprに型を与える
                for(int i = 0; i < a1->table->_sp; i++) ((ast *)(a1->table->_table[i]))->o_type = a->o_type;
                vector_insert(a1->table, 0, a);
                push(v, (void *)a1);
                return new_ast(AST_DCL, a->o_type, v);
            }
            printf("SYnraxError:式リストが必要です\n");Throw(1);
        }
        unget_token(S);
        push(v, (void *)a);
        a1 = new_ast(AST_EXP_LIST, OBJ_NONE,v);
        v = vector_init(1);push(v, (void*)a1);
        return new_ast(AST_DCL, a->o_type, v);
    }
    S->buff->_cp=token_p;
    return NULL;
}

ast * is_class_def_expr(TokenBuff *S) {
    ast * a;
    int token_p = S->buff->_cp;
    Vector * v;
    token * t = get_token(S);
    if (t->type==TOKEN_SYM && strcmp(t->source->_table, "class") == 0) {
        if (a=is_arg_list(S)) {
            v = vector_init(10);
            push(v,(void*)a);
            return new_ast(AST_CLASS, OBJ_NONE,v);
        } else {
            printf("Syntax error! Must be dclr expression!\n");
            Throw(1);
        }
    }
    S->buff->_cp=token_p;
    return NULL;
}

ast * is_expr(TokenBuff *S) {
    ast * a;
    //if (get_token(S)->type == TOKEN_EOF) return NULL;  // tokenがない場合
    //unget_token(S);
    //if (a = is_dcl_expr(S)) return a;
    if (a = is_set_expr(S)) return a;
    //if (a = is_if_expr(S)) return a;
    if (a = is_lambda_expr(S)) return a;
    //if (a = is_while_expr(S)) return a;
    //if (a = is_for_expr(S)) return a;
    //if (a = is_loop_expr(S)) return a;
    if (a = is_special_expr(S, ':')) return a;
    if (a = is_class_def_expr(S)) return a;
    //if (a = is_expr_6(S)) return a;
    if (a = is_expr_2n(S,14)) return a;
    /* ||
       is_dcl_expr(p)      ||
       is_while_expr(p)    ||
       is_and_or_expr(p)   ||
       return NULL;*/
    //printf("SyntaxErroor:Not a exprssion!\n");
    //Throw(1);
    return NULL;
}

ast * is_expr_ex(TokenBuff *S) {
    ast * a;
    if (get_token(S)->type == TOKEN_EOF) return NULL;  // tokenがない場合
    unget_token(S);
    if (a = is_dcl_expr(S)) return a;                   // dcl式はtopレベルでしか定義されないので(mlを除く)exprから外す
    if (a = is_expr(S)) return a;
    printf("SyntaxErroor:Not a exprssion!\n");
    Throw(1);
}

ast *  is_ml_expr(TokenBuff * S ) {
    ast * a;
    token * t;
    int token_p = S->buff ->_cp;
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
    S->buff -> _cp = token_p;
    return NULL;
}
ast * is_ml_expr_list(TokenBuff * S) {
    // ml_expr_list = expr {',' expr}
    //              =>
    // AST_EXP_LIST, non, [expr_ast, ..., expr_ast]
    ast * a1, * a2;
    token * t1, * t2;
    Vector * v;
    int token_p = S->buff ->_cp;

    //if (a1 = is_expr(S)) {
    if (a1 = is_expr_ex(S)) {                                   // mlの最上位にはdcl式を許す
        v=vector_init(3);
        while (TRUE) {
            if ((t1 = get_token(S)) ->type != ';') {
                unget_token(S);
                push(v,(void*)a1);
                return new_ast(AST_EXP_LIST,a1->o_type,v);
            }
            push(v,(void*)a1);
            if (a1 = is_expr_ex(S)) {                           // mlの最上位にはdclを許す
                ;
            } else {
                printf("Syntax error in ml_expr_list\n");
                Throw(1);
            }
        }
    }
    S->buff -> _cp = token_p;
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
