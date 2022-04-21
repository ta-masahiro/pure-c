#include "lexparse2.h"
#define MAXBUFF 4096
#include <fcntl.h>

Stream  * new_stream(Symbol *s) {
    int file_id ;
    size_t n;

    if ((file_id = open(s->_table, O_RDONLY)) == -1) {printf("FileI/O Error!\n");Throw(3);}   

    Stream * S = (Stream * )malloc(sizeof(Stream) );
    S ->_pos = 0;
    S ->_line = 0;
    S ->_buff = (char * )malloc(MAXBUFF * sizeof(char));  
    S -> _fileid = file_id;

    if ((n = read(file_id, S ->_buff, MAXBUFF-1)) == 0) return NULL;; 
    if (n <= 0) {printf("FileI/O Error!\n"); Throw(3);}  
    S->_max = n; S->_buff[S->_max] = '\0';
    return S; 
}
Stream  * new_str_stream(Symbol * str) {
    Stream * S = (Stream * )malloc(sizeof(Stream) );
    S ->_pos = 0;
    S ->_line = 0;
    S ->_buff = str->_table;  
    S -> _fileid = NULL;
    S->_max = str->_size;
    return S; 
};

char * _reload(Stream *S) {
    size_t n;
    if (S->_fileid == -1) return NULL;
    S->_pos = 0;
    //(S ->_line) ++;
    if ((n = read(S->_fileid, S->_buff, MAXBUFF-1)) == 0) return NULL;
    if (n < 0) {printf("FileI/O Error!\n");Throw(3);}
    S->_max = n; S->_buff[S->_max] = '\0';
    return S;
}

unsigned char _get_char(Stream *S) {
    char *p;
    char c = S->_buff[(S->_pos) ++];
    if (c == '\0') {
        if ((p = _reload(S)) == NULL) return '\0';
        c = S->_buff[(S->_pos) ++];
    }
    if (c == '\n') S->_line ++;
    return c;
}

void _unget_char(Stream *S) {
    (S->_pos) --;
}

Symbol * get_line(Stream *S) {
    // stramのbufferから改行記号まで取り出してsymbolに変換する
    if (S->_pos >=S->_max -1) reload(S);
    char * p = strchar(S->_buff + S->_pos, '\n');
    if (p == NULL) {
        S->_pos = S->_max;
        return new_symbol(S->_buff + S->_pos, S->_max);
    }
    S->_pos = p - S->_buff;
    return new_symbol(S->_buff + S->_pos, p - S->_pos +1);
}

Token * new_token(int type, Symbol * s, int line, int pos) {
    Token * t = (Token*)malloc(sizeof(Token));
    t->type=type;
    t->source=s; 
    //t->value.ptr=val; 
    t->line = line;
    t->pos = pos;
    return t;
}

Token * _get_dec(Stream *S) {
    unsigned char c;
    while ((c = _get_char(S)) != '\0' && isblank(c)) ;
    if (c == '\0' || c != '0') { _unget_char(S); return NULL;}
    Symbol * buff = new_symbol("",0);
    while (isdigit(c = _get_char(S))) {
        symbol_push_c(buff, c);
    }
    _unget_char(S);
    return new_token(TOKEN_INT, buff, S->_line, S->_pos - buff->_size);
}

Symbol * _get_hex_part(Stream *S) {
    unsigned char c;
    
    if ((c = _get_char(S)) == '\0') return NULL;
    Symbol * buff = new_symbol("",0);
    while (((c = _get_char(S)) >='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F')) symbol_push_c(buff, c);
    _unget_char(S);
    return buff;
}
Token * _get_hex(Stream *S) {
    unsigned char c;
    //while ((c = _get_char(S)) != '\0' && isblank(c)) ;
    //if (c == '\0' || c != '0') { _unget_char(S); return NULL;}
    if ((c = _get_char(S)) != 'x' && c != 'X') { _unget_char(S); return NULL;}
    Symbol * buff = new_symbol("0X",2);

    while (((c = _get_char(S)) >='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F')) symbol_push_c(buff, c);
    _unget_char(S);
    return new_token(TOKEN_HEX, buff, S->_line, S->_pos - buff->_size);
}

Token * _get_oct(Stream *S) {
    unsigned char c;
    //while ((c = _get_char(S)) != '\0' && isblank(c)) ;
    //if (c == '\0' || c != '0') { _unget_char(S); return NULL;}
    if ((c = _get_char(S)) != 'o' && c != 'O') { _unget_char(S); return NULL;}
    Symbol * buff = new_symbol("0O",2);

    while ((c = _get_char(S)) >='0' && c<='8') symbol_push_c(buff, c);
    _unget_char(S);
    return new_token(TOKEN_OCT, buff, S->_line, S->_pos);
}

Token * _get_bin(Stream *S) {
    unsigned char c;
    //while ((c = _get_char(S)) != '\0' && isblank(c)) ;
    //if (c == '\0' || c != '0') { _unget_char(S); return NULL;}
    if ((c = _get_char(S)) != 'b' && c != 'B') { _unget_char(S); return NULL;}
    Symbol * buff = new_symbol("0B",2);

    while ((c = _get_char(S)) =='0' || c =='1') symbol_push_c(buff, c);
    unget_token(S);
    return new_token(TOKEN_BIN, buff, S->_line, S->_pos);
}

Token *_get_hex_oct_bin(Stream *S) {
    unsigned char c;
    Token *t;
    while ((c = _get_char(S)) != '\0' && isblank(c)) ;
    if (c == '\0' ) { _unget_char(S); return NULL;}
    if ((t =_get_dec(S)) || (t =_get_hex(S))||(t =_get_oct(S))||(t =_get_bin(S))) return t;
}
// 指数部を求める [e|E][+|-]?[0-9]+
Symbol * _get_exp_part(Stream *S) {

}

// 小数を求める
// 小数: 整数部+指数部 | 整数部+'.'+指数部 | 整数部+'.'+整数部 | 整数部+'.'+整数部+指数部
Token * _get_num(Stream *S) {
    unsigned char c;
    Symbol * sym, *sym1;
    Token * t, *t1;
    int type;
    while ((c = _get_char(S)) != '\0' && isblank(c)) ;
    if (c == '\0' ) { _unget_char(S); return NULL;}

    if ((t = _get_dec(S))) {
        if ((sym == _get_exp_part(S))) { 
            if ((c = _get_char(S))== 'f' || c == 'F') type = TOKEN_EFLT;
            else {_unget_char(S); type = TOKEN_FLT;}
            return new_token(type, symbol_append(t->source, sym), t->line, t->pos);                     // 整数部+指数部
        }else if ((c = _get_char(S)) == '.') {
            symbol_push_c(t->source, c);         
            if (sym =  _get_exp_part(S)) {
                return new_token(TOKEN_EFLT, symbol_append(t->source, sym), t->line, S->_pos - t->pos); // 整数部+'.'+指数部
            } else if (t1 = _get_dec(S)) {
                sym = symbol_append(sym, t1->source);
                if (sym1 =_get_exp_part) {
                    return new_token(TOKEN_FLT, symbol_append(sym, sym1), t->line, t->pos - S->_pos);   // 整数部+'.'+整数部+指数部
                }
                return new_token(TOKEN_FLT, sym, t->line, t->pos - sym->_sp);                           // 整数部+'.'+整数部
            }
            return new_token(TOKEN_FLT, sym, t->line, t->pos - sym->_sp) ;                              // 整数部+'.'
        } else if (c == '/') {
            if ((t1=_get_num(S)->type == )) {

            }
        }
        return new_token(TOKEN_INT, t->source, t->line, t->pos - t->source->_sp);
    }else if ((c = _get_char(S)) == '.') {

    }
    _unget_char(S);
    if ((t=_get_hexoctbin())) {

    }else if ((t=_get_rat(S))) {

    }   
}

Symbol * _get_sym(Stream *S) {
    unsigned char c;
    //while ((c = _get_char(S)) != '\0' && isblank(c)) ;
    //if (c != '_' && ! isalpha(c)) { _unget_char(S); return NULL;}
    Symbol * buff =new_symbol(c, 1);
    while (((c = _get_char(S)) == ' '|| isalnum(c))) symbol_push_c(buff, c);
    _unget_char(S);
    return new_token(TOKEN_SYM, buff, S->_line, S->_pos - buff->_sp);
}

Symbol *_get_str(char **S) {
    //     文字列かどうか判定し、文字列ならそれをトークンとして返す
    //     そうでないならNULLを返す
    //     「"」と「"」で囲まれた文字を文字列リテラルとする
    //     「\」が現れたらエスケープ文字があるとする
    char cc,*_buff;
    char c = _get_char(S);

    if (c == '"') {
        if ( _get_char(S) == '"' && _get_char(S) == '"') {
            // RAW string
        
        } else {
            // normal string
            _unget_char(S);(
            _unget_char(S);
            while ((c = _get_char(S)) != '"') {
                
            }

        }

}

Symbol * _get_raw_str(char **S) {

}

void _get_com(char **S) {

}

Symbol * _get_del(char **S) {

}

void _get_com_l(char **S) {

}

Token * _str2token(char **S) {
    // 文字列*Sからtokenを取り出す バッファリングはしないので必要ならget_tokenを使うこと
    // 文字列を最後まで読んだらNULLを返す
    unsigned char * s = *S;            
    Token * t;
    char c,*p;
    Symbol * token_sym;
    while (TRUE) {
        while ((c = *s++) != '\0' && (isblank(c) || c == '\n')) ;          // 空白と改行を読み飛ばして
        if (c=='\0') return NULL;                           // NULL文字なら

        get_comm(&s);                                       // コメントを読み飛ばす
        get_comm_l(&s);
        if ((t = _get_num(S)) || (t = _get_sym(S)) || (t = _get_str(S)) ||(t = _get_del(S)) ) return t;  // 数値ならそれをtokenに入れて返す  
        //return NULL;
    }

}

Token * get_token(TokenBuff *tokens) {
    // token bufferからtokenを取り出し返す
    // bufferが空なら_get_ktokenでBuffに読み込み、それを返す
    // EOFの時はTOKEN_EOFを返し続ける
    Vector * tokenbuff =tokens->buff;
    Stream * S = tokens->S;
    unsigned char * str = S->_buff;
    Token * t;
    //token_print(tokens);
    if (!is_queu_empty(tokenbuff)) return (Token*)dequeue(tokenbuff);
    t=_get_token(&str);//printf("%s\n",t->source->_table);
    if (t == NULL)  {   // tがNULLになったら1行読み込む

        return new_token(TOKEN_EOF,NULL,NULL,S);   // EOFの場合TOKEN_EOFを返す
    }
    t->line = S->_line;t->pos =     
    push(tokenbuff,(void*)t);(tokenbuff->_cp)++;
    return  t; 
}

void unget_token(TokenBuff *tokens) {
    if ((tokens->buff->_cp) == 0) printf("Unget_token is too much!\n"); 
    else (tokens->buff->_cp) --;
    //token_print(tokens); 
}

}