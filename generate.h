#ifndef GENERATE
#define GENERATE
#include "vm.h"
#include "lexparse.h"

void disassy(Vector*v,int i,FILE*fp);

extern Hash* G;
extern Hash* GLOBAL_VAR;
extern Hash* PRIMITIVE_FUNC;
/*
typedef struct {
    obj_type type;
    obj_type functon_ret_type;  // todo -> code_typeを返すようにする
    Vector  *arg_type;          // todo -> arg_typeにはobj_typeでなくcode_typeを入れるようにする
    int dotted;
} code_type;
*/
typedef struct code_type{
    obj_type type;
    struct code_type * functon_ret_type;  
    Vector  *arg_type;
    int dotted;                             // 1: 任意個数引数の関数の場合 0:そうでない場合
} code_type;
//code_type * new_ct(obj_type type,obj_type frt, Vector*at,int dot ) ;
typedef struct {
    Vector      *code;              // intermediate code
    //obj_type    type;               // object type of code
    //Vector*     arg_type;           // if type is function, parameter type
    //obj_type    function_r_type;    // if type is function, function return type
    //int         dotted;             // 0: normal function 1: dotted fctunction
    code_type * ct;
    int         sc;                              // 1: small code(大域変数以外のスコープ外の変数にアクセスしない)
} code_ret;

code_type * new_ct(obj_type type, code_type * frt, Vector*at,int dot);

code_ret* new_code(Vector*code, code_type * ct, int small) ;
code_ret* codegen(ast*a,Vector*v,int tail);
code_ret* codegen_ml(ast*a,Vector*v,int tail);
code_ret* codegen_if(ast*a,Vector*v,int tail);
code_ret* codegen_set(ast*a,Vector*v,int tail);
code_ret* codegen_lambda(ast*a,Vector*v,int tail);
code_ret* codegen_dcl(ast*a,Vector*v,int tail);
code_ret* codegen_fcall(ast*a,Vector*v,int tail);
code_ret* codegen_apply(ast*a,Vector*v,int tail);
code_ret* codegen_2op(ast*a,Vector*v,int tail);
code_ret* codegen_1op(ast*a,Vector*v,int tail);
code_ret* codegen_vref(ast*a,Vector*v,int tail);
code_ret* codegen_sls(ast*a,Vector*v,int tail);
code_ret* codegen_var(ast*a,Vector*v,int tail);
code_ret* codegen_lit(ast*a,Vector*v,int tail);
code_ret* codegen_vect(ast*a,Vector*v,int tail);
code_ret* codegen_while(ast*a,Vector*v,int tail);

void disassy(Vector *code, int indent, FILE *fp);
code_ret *str_compile(Symbol *s);
object *code_eval(code_ret *code);

#endif