
#include <ctype.h>
#include "vm.h"
//#include "symbol.h"

typedef struct {
    int     _line;
    int     _pos; 
    FILE *  _fp; 
    char *  _buff;
} Stream; 
#define MAXBUFF 1024

Stream  * new_stream(FILE * f) {
    char * p; 
    Stream * S = (Stream * )malloc(sizeof(Stream) );
    S ->_pos = 0;
    S ->_line = 0;
    S ->_buff = (char * )malloc(MAXBUFF * sizeof(char));  
    p = fgets(S ->_buff, MAXBUFF, f); 
    S -> _fp = f;
    if (p == NULL) return NULL;  
    return S; 
}
/*
Symbol * new_symbol(unsigned char * str, unsigned long size) {
    char * s = (char * )malloc((size + 1) * sizeof(char)); 
    // strcpy(s, str); // printf("%s\n", s);
    memcpy(s,str,size);
    // if (strcmp(s, "LDC") == 0) printf("!LDC!\n");  
    Symbol * sym = (Symbol * )malloc(sizeof(Symbol));
    sym -> _size = size; 
    sym -> _table = s;  
    return sym;  
}
*/
char get_char(Stream * S) { return S ->_buff[(S ->_pos) ++ ]; }

void unget_char(Stream * S) { (S ->_pos) -- ; }

char * re_load(Stream * S) {
    char * p; 
    S ->_pos = 0;
    (S ->_line)++; 
    p = fgets(S ->_buff, MAXBUFF, S ->_fp);
     // if (p == NULL) printf("faile EOF\n");
    return p;   
}
Vector *  get_code(Stream * s);

int strcmp_ignorecase(const char *s1, const char *s2) {
    int i = 0;
    /* 文字が等しい間繰り返す */
    while (toupper((unsigned char)s1[i]) == toupper((unsigned char)s2[i])) 
        if (s1[i ++ ] == '\0') return 0;
    return toupper((unsigned char)s1[i]) - toupper((unsigned char)s2[i]);
}
#define strcmp strcmp_ignorecase
/* 
mpz_ptr new_long() {
    mpz_ptr long_int_p = (mpz_ptr)malloc(sizeof(MP_INT)); 
    mpz_init(long_int_p);
    return long_int_p;
}

mpz_ptr new_long_valued(long val) {
    mpz_ptr long_int_p = (mpz_ptr)malloc(sizeof(MP_INT)); 
    mpz_init_set_si(long_int_p,val);
    return long_int_p;
}
mpz_ptr new_long_str(char* s) {
    mpz_ptr long_int_p = (mpz_ptr)malloc(sizeof(MP_INT)); 
    mpz_init_set_str(long_int_p,s,10);
    return long_int_p;
}
*/ 
Vector * get_code(Stream * s) {
     // printf("in get_code \n"); 
    char c, cc; 
    Vector * code = vector_init(64); 
    char str_buff[255]; int str_buff_p = 0; 

    while (TRUE) {
        c = get_char(s); // printf("%c\n", c);  
        if (isblank(c)) continue; 
        if (c == ']'||c == ')')
            break;
        else if (c == '['||c == '(') {
            push(code, get_code(s)); 
        } else if (isalpha(c) || c == '_') {
            str_buff_p = 1;
            str_buff[0] = c;  
            while (isalnum(c = get_char(s))) {
                str_buff[str_buff_p ++ ] = c; 
            }
            unget_char(s); 
            str_buff[str_buff_p] = '\0'; 
            push(code, (void * )new_symbol(str_buff, str_buff_p + 1));
        } else if ((c == '-') && (isdigit(cc = get_char(s)))) {
            str_buff_p = 1; 
            str_buff[0] = cc; 
            while (isdigit(cc = get_char(s))) {
                str_buff[str_buff_p ++ ] = cc; 
            }
            unget_char(s); 
            str_buff[str_buff_p] = '\0';
            push(code, (void * )( - atol(str_buff)));  
        } else if (isdigit(c)) {
            str_buff_p = 1; 
            str_buff[0] = c; 
            while (isdigit(c = get_char(s))) {
                str_buff[str_buff_p ++ ] = c; 
            }
            unget_char(s); 
            str_buff[str_buff_p] = '\0';
            push(code, (void * )atol(str_buff));  
        } else if (c == '\n' || c == ';' ) {
            if (re_load(s) == NULL) return NULL;
        } else  
            printf("erro!:%c",c ); 
   
    }
     // printf("end of code\n");
     // vector_print(code);  
    return code; 
}
/*
char * code_name[] = 
    {"STOP",  "LDC",  "LD",  "ADD", "CALL", "RTN", "SEL", "JOIN", "LDF", "SET", "LEQ", "LDG", "GSET", "SUB",
     "DEC",   "TCALL","TSEL","DROP","EQ",   "INC", "MUL", "DIV",  "VEC", "REF", "VSET","HASH","LDH",  "HSET",
     "VPUSH", "VPOP", "LADD","LSUB","LMUL", "ITOL","LPR", "PCALL","LDM", "DUP", "SWAP","ROT", "_2ROT","CALLS",
     "TCALLS","RTNS", "LDP", "LDL", "FADD", "FSUB","FMUL","FDIV", "FPR", "ITOF","LCPY","OADD","OSUB", "OMUL",
     "ODIV",  "OEQ",  "OLEQ","ITOO","OPR",  "ODEC","OINC","IADD", "ISUB","IMUL","IDIV","IEQ", "ILEQ", "IDEC",
     "IINC",  "LTOO", "FTOO","IJTOO","SPR", "LDIV","OLT", "LT"  , "ILT", "GT",  "IGT", "OGT"  "GEQ",  "IGEQ",
     "OGEQ",  "NEG",  "INEG","ONEG", "BNOT","APL", "TAPL","FEQ",  "FLEQ","FGEQ","FLT", "FGT", "LEQ",  "LLEQ",
     "LGEQ",  "LLT",  "LGT", "RADD", "RSUB","RMUL","RDIV","REQ",  "RLEQ","RGEQ","RLT", "RGT", "ITOR", "_ITOF",
     "LTOR",  "LTOF", "RTOF", "RTOO","LTOI","RTOI","RTOL","FTOI", "FTOL","FTOR", "LNEG","RNEG","FNEG","LDEC",
     "LINC",  "$$$" };
*/
/*
void code_optimize(Vector * code, Hash *G){
    char * key;
    Symbol * sym; 
    void ** v;
    void * c;
    while ((c = dequeue(code)) != NULL) {
        //printf("%ld\t%s\n",(long)c,code_name[(long)c]);
        if ((long)c==LDG) {
            sym = (Symbol *)dequeue(code);
            if ((v=Hash_get(G, sym))==NULL || (long)code->_table[code->_cp] != CALL || (long)code->_table[code->_cp] != TCALL)  continue;
            code->_table[code->_cp - 1] = (void *)( *v);
            code->_table[code->_cp - 2] =(void *)LDC;
        } else if ((long)c==SEL || (long)c==TSEL) {
            code_optimize((Vector*)dequeue(code),G);
            code_optimize((Vector*)dequeue(code),G);
        } else if ((long)c==LDF) {
            code_optimize((Vector*)dequeue(code),G);
        } else (code->_cp) += code_pr_size[(long)c];
    }
    code->_cp = 0;
}
*/
int search_operand(char * s, char * code_name[]) {
    int i = 0;//printf("%s\n",s) ;
    while (TRUE) {//printf("%d:%s\n",i,code_name[i]);
        if (strcmp(code_name[i], "$$$" ) == 0) return  -1; 
        if (strcmp(s, code_name[i]) == 0) return i; 
        i ++ ; 
    }
}

Vector * chg_byte_code(Vector * code, Hash * G) {
    enum CODE op;char * c;   
    Vector * t = vector_init(64); 
    Symbol * sym; 

    while (TRUE) {
        if ((sym = (Symbol * )dequeue(code)) == NULL) break;
        c = sym -> _table; //printf("%s\n", c); 
        op = search_operand(c, code_name);// printf("%d\n", op);  
        switch (op) {
            case SEL:case TSEL:
                push(t, (void * )op); 
                push(t, chg_byte_code ((Vector * ) dequeue(code), G));
                push(t, chg_byte_code ((Vector * ) dequeue(code), G));
                break;  
            case LDF:case LDP:
                push(t, (void * )op); 
                push(t, chg_byte_code ((Vector * ) dequeue(code), G));
                break;
            default: 
                if (op==-1) {printf("illegal OPcode! %s\n",(char*)c);exit(-1);}
                if (op_size[op] == 0) {
                    push(t, (void * )op) ;        
                } else if (op_size[op] == 1) {
                    push(t, (void * )op) ;        
                    push(t, dequeue(code)); 
                } else {
                    printf("Unknkown Command %s\n", (char * )c);         
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
void*vprint(Vector*v) {
    Vector*vv=(Vector*)(v->_table[0]);
    printf("[ "); 
    for(long i=0;i<(long)(vv->_sp);i++) printf("%ld ",(long)(vv->_table[i]));
    printf("]\n");
    return (void*)v;
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

int main(int argc, char * argv[]) {
    char c; 
    long v; 
    FILE * fp; 
    Stream  * s;

    // mp_set_memory_functions((void * )GC_malloc, (void * )GC_realloc,(void * )GC_free);
    mp_set_memory_functions((void *)GC_malloc, (void * )_realloc, (void * ) GC_free);

    Vector  * code,  * t; 
    Vector * S = vector_init(500000); 
    Vector * E = vector_init(5); 
    Vector * C, * CC ; 
    Vector * R = vector_init(500); 
    Vector * EE = vector_init(50); 
    Hash * G = Hash_init(128); // must be 2^n 

    //primitive関数のセット
    Hash_put(G, new_symbol("sum", 4), (void*)sum);
    Hash_put(G, new_symbol("list", 5), (void*)list);
    Hash_put(G, new_symbol("iprint", 7), (void*)iprint);
    Hash_put(G, new_symbol("vprint", 7), (void*)vprint);
    Hash_put(G, new_symbol("lprint", 7), (void*)lprint);
     // Hash_put(G, "hash_delete", (void * )hash_delete); 
    // print_hashTable(G); 
    if (argc <= 1 ) s = new_stream(stdin);
    else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {printf("file %s doesn't exist\n", argv[1]); return  - 1; }
        s = new_stream(fp); 
    }
    while (TRUE) {
         while (TRUE) {
            if ((c = get_char(s)) == '[') { 
                code = get_code(s);
                vector_print(code);
                C = chg_byte_code(code, G); 
                vector_print(C);
                //disassy(C, 0);
                //code_optimize(C,G);
                disassy(C,0);
                v = (long)eval(S, E, C, R, EE, G);
                printf("%ld\n", v);    
                break;   
            } else if (isblank(c)) continue;  
            else if (c == '\n') { 
                if (re_load(s) == NULL) return 0;
                break;
            } else {printf("error!\n");break; }
        }
    }
}
