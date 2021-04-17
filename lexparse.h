#include <ctype.h>
#include "vector.h"
#include "symbol.h"
#include "object.h"
/*
typedef struct {
    unsigned long _size;
    unsigned char *_table;
} Symbol;
*/
typedef struct {
    int     _line;
    int     _pos; 
    FILE *  _fp; 
    char *  _buff;
} Stream;

typedef enum {
    TOKEN_NONE, TOKEN_INT, TOKEN_LINT, TOKEN_RAT, TOKEN_FLT, TOKEN_EFLT, TOKEN_SYM, TOKEN_STR, TOKEN_CHR,TOKEN_HEX,TOKEN_OCT,TOKEN_BIN
//  初期状態、整数、分数、小数、指数付き少数、シンボル、文字列、文字 
} tokenstate; 

typedef tokenstate tokentype; 

typedef struct {
    int         type; 
    Symbol *    source;
    int         line, pos;
    union {
        long    intg; 
        double  flt; 
        void *  ptr; 
    } value; 
} token;

typedef enum {
    AST_NONE,       // 0:AST_NONE,[]
    AST_ML,         // 1:AST_ML,[ast_list] 
    AST_IF,         // 2:AST_IF,[cond_ast,true_ast, false_ast]
    AST_SET,        // 3:AST_SET,[left_ast,right_ast]
    AST_LAMBDA,     // 4:AST_LAMBDA,[[ast_list],ast]
    AST_WHILE,      // 5:AST_WILE,[cond_ast,ast]
    AST_CLASS,      // 6:AST_CLASS,[] 
    AST_2OP,        // 7:AST_2OP,[op_type,left_ast,right_ast]
    AST_1OP,        // 8:AST_1OP,[op_type,ast] 
    AST_VREF,       // 9:AST_VREF [vect_ast,index_ast] 
    AST_SLS,        //10:
    AST_LIT,        //11:AST_LIT,[type,value] 
    AST_VAR,        //12:AST_VAR,[value]
    AST_VECT,       //13:AST_VECT,[ast_expr_list]
    AST_DICT,       //14:
    AST_APPLY,      //15:AST_APPLY,[ast_expr_list]
    AST_FCALL,      //16:AST_FCALL,[ast,[ast_expr_list]] 
    AST_EXP_LIST,   //17:AST_EXP_LIST,[ast_list] 
    AST_CALLCC,     //18:
    AST_PROP,       //19:
    AST_DCL,        //20:AST_DCL expr_list_ast
    AST_EXP_LIST_DOTS,
    AST_ARG_LIST,   //22:AST_ARG_LIST,[ast,ast,ast...]
    AST_ARG_LIST_DOTS,
} ast_type; 


typedef struct ast{
    ast_type type;
    obj_type o_type; 
    Vector * table; 
} ast; 
/*
ast * new_ast(ast_type type, Vector * table) {
    ast * a = (ast * )malloc(sizeof(ast)); 
    a -> type = type; 
    a -> table = table; 
    return a; 
}
*/
extern char*dcl_string[];

Stream  * new_stream(FILE * f);  
//Symbol * new_symbol(unsigned char * str, unsigned long size);  
token * new_token(int type, Symbol * s, void * val, Stream * S); 
token * _get_token(Stream * S); 
token * get_token(Stream * S) ; 
void unget_token(Stream * S); 
void token_print(Vector*buff); 
ast * new_ast(ast_type type, obj_type o_type,Vector * table) ;
void ast_print(ast*a, int tablevel) ; 
ast * is_lit(Stream*S) ; 
ast * is_var(Stream *S) ; 
ast * is_nomad(Stream *S) ;
ast * is_expr_list(Stream * S) ; 
ast * is_factor(Stream*s);
ast * is_expr_0(Stream *S) ; 
ast * is_expr_1(Stream * S) ; 
ast * is_expr_2(Stream * S) ; 
ast * is_expr_3(Stream * S) ; 
ast * is_expr_4(Stream * S) ; 
ast * is_expr_5(Stream * S) ; 
ast * is_expr(Stream *S) ;
ast * is_set_expr(Stream * S);
ast * is_if_expr(Stream * S); 
ast * is_ml_expr(Stream * S ) ; 
ast * is_ml_expr_list(Stream * S) ; 
