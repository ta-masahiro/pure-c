#ifndef GENERATE
#define GENERATE
#include "vm.h"
#include "lexparse.h"

void disassy(Vector*v,int i,FILE*fp);

extern Hash* G;
extern Hash* GLOBAL_VAR;
extern Hash* PRIMITIVE_FUNC;

typedef struct {
    obj_type type;
    obj_type functon_ret_type;
    Vector  *arg_type;
    int dotted;
} code_type;

code_type * new_ct(obj_type type,obj_type frt, Vector*at,int dot ) ;

typedef struct {
    Vector      *code;              // intermediate code
    //obj_type    type;               // object type of code
    //Vector*     arg_type;           // if type is function, parameter type
    //obj_type    function_r_type;    // if type is function, function return type
    //int         dotted;             // 0: normal function 1: dotted fctunction
    code_type * ct;
} code_ret;


code_ret*new_code(Vector*code, code_type * ct);
code_ret* codegen(ast*a,Vector*v,int tail);
#endif