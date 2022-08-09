#include "lexparse.h"
//#include <ctype.h>
#include "vm.h"
//#include "symbol.h"
#define MAXBUFF 1024

int strcmp_ignorecase(const char *s1, const char *s2) {
    int i = 0;
    /* 文字が等しい間繰り返す */
    while (toupper((unsigned char)s1[i]) == toupper((unsigned char)s2[i])) 
        if (s1[i ++ ] == '\0') return 0;
    return toupper((unsigned char)s1[i]) - toupper((unsigned char)s2[i]);
}

#define strcmp strcmp_ignorecase

int search_operand(char * s, char * code_name[]) {
    int i = 0;//printf("%s\n",s) ;
    while (TRUE) {//printf("%d:%s\n",i,code_name[i]);
        if (strcmp(code_name[i], "$$$" ) == 0) return  -1; 
        if (strcmp(s, code_name[i]) == 0) return i; 
        i ++ ; 
    }
}

long token_source_to_object(token *t, int mflag) {
    //char *str = t->source->_table;
    char *str = (char*)malloc(t->source->_sp + 1);
    if (mflag) {*str = '-';strcpy(str+1, t->source->_table);} else strcpy(str, t->source->_table);
    void * v;
    double d;
    switch(t->type) {
        case TOKEN_NONE:
            return 0;
        case TOKEN_INT:
            return strtol(str, NULL, 10);
        case TOKEN_LINT:
            v = malloc(sizeof(MP_INT));
            mpz_set_str((mpz_ptr)v, str, 10);
            return (long)v;
        case TOKEN_RAT:
            v = malloc(sizeof(MP_RAT));
            mpq_init(v);
            if (mpq_set_str((mpq_ptr)v, str, 10) !=0 || mpz_sgn(mpq_denref((mpq_ptr)v)) == 0) {printf("SyntaxError:IllegalRationalNumber!\n");Throw(0);}
            mpq_canonicalize((mpq_ptr)v);
            return (long)v;
        case TOKEN_STR:
            return (long)(void *)t->source;
        case TOKEN_FLT: TOKEN_EFLT: 
            d = strtod(str, NULL);
            return (long)(void*)*((long*)&d);
        case TOKEN_LEFLT:
            //char**testbuf=(char**)malloc(1024*sizeof(char));
            v = malloc(sizeof(__mpfr_struct));
            mpfr_init_set_str((mpfr_ptr)v, str, 10, MPFR_RNDN);
            return (long)v;
        case TOKEN_HEX:
            if (t->source->_size>16) { // long int
                v = malloc(sizeof(MP_INT));
                mpz_set_str((mpz_ptr)v, str, 16);
                return (long)v;
            } else {
                v = malloc(sizeof(long));
                sscanf(str, "%lx", (long*)v);
                return (long)v;
            }
        case TOKEN_OCT:
            if (t->source->_size>21) { // long int
                v = malloc(sizeof(MP_INT));
                mpz_set_str((mpz_ptr)v, str, 8);
                return (long)v;
            } else {
                v = malloc(sizeof(long));
                sscanf(str, "%lo", (long*)v);
                return (long)v;
            }
        case TOKEN_BIN:
            v = malloc(sizeof(MP_INT));
            mpz_set_str((mpz_ptr)v, str, 2);
            if (t->source->_size > 64) { // long int
                return (long)v;
            } else {
                v = malloc(sizeof(long));
                sscanf(str, "%lo", (long*)v);
                return mpz_get_si((mpz_ptr)v);
            }
        case TOKEN_CHR:
        default: printf("Unknown Token!\n");Throw(2);
    }
}

Vector * get_code(TokenBuff *S, Hash * G) {
    enum CODE op;char * c;
    token * tk;
    int *type, i, mflag ;   
    Vector * t = vector_init(64),*ref; 
    Symbol * sym; 
    long ll;

    while (TRUE) {
        tk = get_token(S);
        if (tk->type == TOKEN_EOF) {printf("Error! 予期しないEOF\n");}
        if (tk->type == ']') return t;
        c = tk->source->_table; //printf("%s\n", c); 
        op = search_operand(c, code_name);// printf("%d\n", op);  
        push(t, (void * )op); 
        switch (op) {
            case LDC:
                tk = get_token(S); if (tk->type == '-') mflag = TRUE; else {mflag = FALSE;unget_token(S);}
                push(t, (void*)token_source_to_object(tk = get_token(S), mflag));
                break;
            case LD:
                if ((tk = get_token(S))->type != '[' && tk->type != '(') {printf("Error! [がない\n");break;}
                ref = vector_init(2);
                if ((tk = get_token(S))->type != TOKEN_INT) {printf("Error!数値が必要(LD第一引数)\n");}
                push(ref, (void*)strtol(tk->source->_table, NULL, 10));
                if ((tk = get_token(S))->type != TOKEN_INT) {printf("Error!数値が必要(LD第二引数)\n");}
                push(ref, (void*)strtol(tk->source->_table, NULL, 10));
                if ((tk = get_token(S))->type != ']' && tk->type != ')') {printf("Error! ]がない\n");break;}
                push(t,(void*)ref);
                break; 
            case SEL:case TSEL:
                for(i = 0; i <= 1; i ++) {
                    if ((tk = get_token(S))->type != '[') {printf("Error! [がない\n");break;}
                    push(t, get_code (S, G));
                }
                break;  
            case LDF:case LDP:
                if ((tk = get_token(S))->type != '[') {printf("Error! [がない\n");break;}
                push(t, get_code (S, G));
                break;
            case WHILE:
                if ((tk = get_token(S))->type != TOKEN_INT) {printf("Error!数値が必要(WHILE)\n");}
                push(t, (void*)strtol(tk->source->_table, NULL, 10));
                if ((tk = get_token(S))->type != '[') {printf("Error! [がない\n");break;}
                push(t, get_code (S, G));
                break;
            case LDG: case GSET:
                if ((tk = get_token(S))->type != TOKEN_SYM) {printf("Error!グローバルネームが必要です\n");}
                push(t, (void*)tk->source);
                break;
            default:
                if (op == -1) {printf("illegal OPcode! %s\n",tk->source->_table);exit(-1);}
                if (op_size[op] == 1) {
                    if ((tk = get_token(S))->type != TOKEN_INT && tk->type != '-') {printf("Error!数値が必要(一般) %s\n", code_name[op]);}
                    if (tk->type == '-') mflag = TRUE; else {mflag = FALSE;unget_token(S);}
                    ll = strtol((get_token(S))->source->_table, NULL, 10);
                    push(t, (void*)(mflag ? ll * (-1) : ll));
                }
        }
    }
    return t; 
}
void disassy(Vector * code, int indent) {
    int i; long c; 
    Vector * v; char * s; 
    for(i = 0; i<indent; i ++ ) printf("\t"); 
    printf("[\n"); indent ++ ; 

    while (TRUE) {
        if (is_queu_empty(code)) break;
        c = (long)dequeue(code); 
        for(i= 0; i< indent; i ++ ) printf("\t"); 
        switch(c) {  
            case SEL:   case TSEL:
                printf("%s\n", code_name[c]); 
                disassy((Vector * ) dequeue(code), indent ); 
                disassy((Vector * ) dequeue(code), indent ); 
                break; 
            case LDF:   case LDP:
                printf("%s\n", code_name[c]); 
                disassy((Vector *)dequeue(code), indent ); 
                break;
            case LD:    case SET:
                v = (Vector * )dequeue(code); 
                printf("%s\t(%ld %ld)\n", code_name[c], (long)vector_ref(v, 0), (long)vector_ref(v, 1));  
                break; 
            case LDG:   case GSET: 
                s = ((Symbol * )dequeue(code)) -> _table;
                printf("%s\t%s\n", code_name[c], s);
                break;   
            case WHILE:
                printf("%s\t%ld\n", code_name[c], (long)dequeue(code));
                disassy((Vector*)dequeue(code), indent);
                break;
            default:
                if (op_size[c] == 0) printf("%s\n", code_name[c]);
                else if (op_size[c] == 1) printf("%s\t%ld\n", code_name[c], (long)dequeue(code)); 
                else printf("Unknkown Command %s\n", (char * )c); 
        }
    }
    indent--;
    for(i= 0; i< indent; i ++ ) printf("\t"); 
    printf("]\n"); 
    code ->_cp = 0;  
} 
//仮置きprimitive関数
//Vector*を引数にとりvoid*を返すこと
void *sum(Vector*v) {
    int i;
    long n=(long)(v->_sp);
    long s=0;
    for (i=0;i<n;i++) s+=(long)v->_table[i];
    return (void*)s;
}
void*list(Vector*v) {
    return (void*)vector_copy0(v);
}
void*iprint(Vector*v) {
    for(long i=0;i<(long)(v->_sp);i++) printf("%ld ",(long)(v->_table[i]));
    printf("\n");
    return (void*)v;
}
void vprint(void **sp, int n) {
    Vector*vv=(Vector*)*(sp);
    printf("[ "); 
    for(long i=0;i<(long)(vv->_sp);i++) printf("%ld ",(long)(vv->_table[i]));
    printf("]\n");
    *sp = NULL;
}
void * lprint(Vector * v) {
    for(long i=0;i<(long)(v->_sp);i++) gmp_printf("%Zd ",(mpz_ptr)(v->_table[i]));
    printf("\n"); 
    }
 /* void * hash_delete(Vector * v){
    Hash * h = (Hash * )v[0]; 
    Symbol * s = (Symbol * )v[1]; 
    Hash_delete(h, s); 
    return NULL; 
} */ 
void * _realloc(void * ptr, size_t old_size, size_t new_size) {
    return GC_realloc(ptr, new_size); 
}

extern void make_primitive();
Hash * G;
Hash * GLOBAL_VAR;
Hash * PRIMITIVE_FUNC;  

int main(int argc, char * argv[]) {
    char c; 
    long v; 
    FILE * fp; 
    TokenBuff * s;
    token * tk;
#ifndef DEBUG
    // mp_set_memory_functions((void * )GC_malloc, (void * )GC_realloc,(void * )GC_free);
    mp_set_memory_functions((void *)GC_malloc, (void * )_realloc, (void * ) GC_free);
#endif
    Vector  * code,  * t; 
    //Vector * S = vector_init(500000); 
    Vector * S = vector_init(400000); 
    Vector * E = vector_init(5); 
    Vector * C, * CC ; 
    Vector * R = vector_init(500); 
    Vector * EE = vector_init(50); 
    //Hash * G = Hash_init(128); // must be 2^n 
    G = Hash_init(256); // must be 2^n 
    GLOBAL_VAR = Hash_init(256);
    make_primitive();
    //primitive関数のセット
    Hash_put(G, new_symbol("sum", 4), (void*)sum);
    Hash_put(G, new_symbol("list", 5), (void*)list);
    Hash_put(G, new_symbol("iprint", 7), (void*)iprint);
    Hash_put(G, new_symbol("vprint", 6), (void*)vprint);
    Hash_put(G, new_symbol("lprint", 7), (void*)lprint);
     // Hash_put(G, "hash_delete", (void * )hash_delete); 
    // print_hashTable(G); 
    if (argc <= 1 ) s = new_tokenbuff(stdin);
    else if (strcmp(argv[1], "-f") == 0) {
        fp = fopen(argv[2], "r");
        if (fp == NULL) {printf("file %s doesn't exist\n", argv[2]); return  - 1; }
        s = new_tokenbuff(fp); 
    } else if (strcmp(argv[1], "-s") == 0) {
        s = new_str_tokenbuff(new_symbol(argv[2], strlen(argv[2])));
    }
    while (TRUE) {
         while (TRUE) {
            if ((tk = get_token(s))->type == TOKEN_EOF) exit(0);
            if (tk->type == '[') { 
                C = get_code(s, G);
                //vector_print(code);
                //C = chg_byte_code(code, G); 
                //vector_print(C);
                //disassy(C, 0);
                //code_optimize(C,G);
                disassy(C,0);
                v = (long)eval(S, E, C, R, EE, G);
                printf("%ld\n", v);
                //printf("stack size:%d\n",S->_sp);    
                break;   
            } else if (tk->type == TOKEN_EOF) {
                break;  
            //else if (c == '\n') { 
            //    if (re_load(s) == NULL) return 0;
            //    break;
            } else {printf("error!\n");break; }
        }
    }
}
