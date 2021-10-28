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
    int     _max; 
    FILE *  _fp; 
    char *  _buff;
} Stream;

typedef enum {
    TOKEN_NONE, TOKEN_INT, TOKEN_LINT, TOKEN_RAT, TOKEN_FLT, TOKEN_EFLT, TOKEN_LEFLT, TOKEN_SYM, TOKEN_STR, TOKEN_CHR,TOKEN_HEX,TOKEN_OCT,TOKEN_BIN,TOKEN_COMM,TOKEN_LCOMM
//  初期状態、整数、分数、小数、指数付き少数、シンボル、文字列、文字、コメント、1行コメント
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

//token type definition 
#define t_ADDSET    '+'*256+'='     // += 
#define t_SUBSET    '-'*256+'='     // -= 
#define t_MULSET    '*'*256+'='     // *= 
#define t_DIVSET    '/'*256+'='     // /= 
#define t_MODSET    '%'*256+'='     // %=
#define t_ANDSET    '&'*256+'='     // &=
#define t_ORSET     '|'*256+'='     // |=
#define t_XORSET    '^'*256+'='

#define t_DPLUS     '+'*256+'+' 
#define t_DMINUS    '-'*256+'-'
#define t_DAST      '*'*256+'*'
#define t_DSLA      '/'*256+'/'
#define t_DPAR      '%'*256+'%'
#define t_LAND      '&'*256+'&'
#define t_LOR       '|'*256+'|'
#define t_LXOR      '^'*256+'^'
#define t_DRAR      '>'*256+'>'
#define t_DLAR      '<'*256+'<'
#define t_DEXC      '!'*256+'!'
#define t_MRAR      '-'*256 +'>'
#define t_MLAR      '<'*256 +'-'
#define t_NEQ       '!'*256+'='
#define t_LE        '<'*256+'='
#define t_EEQ       '='*256+'='
#define t_GE        '>'*256+'='
#define t_DRARSET   '>'*65536+'>'*245+'='
#define t_DLARSET   '<'*65536+'<'*256+'='
#define t_DDOT      '.'*256+'.'

typedef struct {
    Stream * S;
    Vector * buff;
} TokenBuff;

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
    AST_SLS,        //10:AST_SLS,[ast_pair_list]
    AST_LIT,        //11:AST_LIT,[type,value] 
    AST_VAR,        //12:AST_VAR,[value]
    AST_VECT,       //13:AST_VECT,[ast_expr_list]
    AST_DICT,       //14:AST_DICT,[ast_pair_list]
    AST_APPLY,      //15:AST_APPLY,[ast_expr_list]
    AST_FCALL,      //16:AST_FCALL,[ast,[ast_expr_list]] 
    AST_EXP_LIST,   //17:AST_EXP_LIST,[ast_list] 
    AST_CALLCC,     //18:
    AST_PROP,       //19:
    AST_DCL,        //20:AST_DCL expr_list_ast
    AST_EXP_LIST_DOTS,
    AST_ARG_LIST,   //22:AST_ARG_LIST,[ast,ast,ast...]
    AST_ARG_LIST_DOTS,
    AST_PAIR,       //24:AST_PAIR,[car_ast,cdrast]
    AST_PAIR_LIST,  //25:AST_PAIR_LIST,[pair_ast,pair_ast,...]
    AST_LOOP        //26:AST_LOOP,[count_ast,ast]
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
//char get_char(Stream*S);
Stream  * new_stream(FILE * f);  
//void unget_char(Stream *S);
//Symbol * new_symbol(unsigned char * str, unsigned long size);  
token * new_token(int type, Symbol * s, void * val, Stream * S); 
TokenBuff *new_tokenbuff(FILE *f);
token * _get_token(Stream *S); 
token * get_token(TokenBuff *t) ; 
void unget_token(TokenBuff *t); 
void token_print(TokenBuff*buff); 
ast * new_ast(ast_type type, obj_type o_type,Vector * table) ;
void ast_print(ast*a, int tablevel) ; 
ast * is_lit(TokenBuff*S) ; 
ast * is_var(TokenBuff *S) ; 
ast * is_nomad(TokenBuff *S) ;
ast * is_expr_list(TokenBuff * S) ; 
ast * is_pair_list(TokenBuff * S) ; 
ast * is_factor(TokenBuff*s);
ast * is_expr_0(TokenBuff *S) ; 
ast * is_expr_1(TokenBuff * S) ; 
ast * is_expr_2(TokenBuff * S) ; 
ast * is_expr_3(TokenBuff * S) ; 
ast * is_expr_4(TokenBuff * S) ; 
ast * is_expr_5(TokenBuff * S) ; 
ast * is_expr(TokenBuff *S) ;
ast * is_set_expr(TokenBuff * S);
ast * is_if_expr(TokenBuff * S); 
ast * is_ml_expr(TokenBuff * S ) ; 
ast * is_ml_expr_list(TokenBuff * S) ; 
