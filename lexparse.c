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
    S -> _fileid = -1;
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
    return S->_buff;
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
/*
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
*/
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
    _unget_char(S);
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
    char c, cc, ccc;
    Symbol * buff;
    if ((c = _get_char(S)) == 'e' || c == 'E') {
        if((cc = _get_char(S)) == '+' || cc == '-') {
            if (isdigit(ccc = _get_char(S))) {
                symbol_push_c(buff,c);
                symbol_push_c(buff,cc);
                symbol_push_c(buff, ccc);
                while (isdigit(ccc = _get_char(S))) {
                    symbol_push_c(buff, ccc);
                }
                _unget_char(S);
                return buff;
            }
            _unget_char(S);_unget_char(S); _unget_char(S);
            return NULL;
        } else if (isdigit(cc)) {
            symbol_push_c(buff,c);
            symbol_push_c(buff,cc);
            while (isdigit(cc = _get_char(S))) {
                symbol_push_c(buff, cc);
            }
            _unget_char(S);
            return buff;
        }
        _unget_char(S); _unget_char(S);
        return NULL;
    }
    _unget_char(S);
    return NULL;
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
            if ((c = sym->_table[0]) == 'f' || c == 'F') type = TOKEN_EFLT;
            else type = TOKEN_FLT;
            return new_token(type, symbol_append(t->source, sym), t->line, t->pos);                     // 整数部+指数部
        }else if ((c = _get_char(S)) == '.') {
            symbol_push_c(t->source, c);         
            if (sym =  _get_exp_part(S)) {
                if ((c = sym->_table[0]) == 'f' || c == 'F') type = TOKEN_EFLT;
                else type = TOKEN_FLT;
                return new_token(type, symbol_append(t->source, sym), t->line, t->pos);                     // 整数部+指数部
            } else if (t1 = _get_dec(S)) {
                sym = symbol_append(t->source, t1->source);
                if (sym1 =_get_exp_part(S)) {
                    if ((c = sym1->_table[0]) == 'f' || c == 'F') type = TOKEN_EFLT;
                    else type = TOKEN_FLT;
                    return new_token(type, symbol_append(sym, sym1), t->line, t->pos);                     // 整数部+指数部
                }
                return new_token(TOKEN_FLT, sym, t->line, t->pos);                           // 整数部+'.'+整数部
            }
            return new_token(TOKEN_FLT, sym, t->line, t->pos) ;                              // 整数部+'.'
        } else if (c == '/') {
            if ((t1=_get_dec(S))) {
                symbol_push_c(t->source, c);
                sym1 = symbol_append(t->source, t1->source);
                return new_token(TOKEN_RAT,sym1,t->line, t->pos);
            }
            _unget_char(S);
        }
        return new_token(TOKEN_INT, t->source, t->line, t->pos - t->source->_sp);
    }else if ((c = _get_char(S)) == '.') {
        if ((sym == _get_exp_part(S))) { 
            if ((c = sym->_table[0]) == 'f' || c == 'F') type = TOKEN_EFLT;
            else type = TOKEN_FLT;
            return new_token(type, symbol_append(t->source, sym), t->line, t->pos);                     // 整数部+指数部
        }
    }
    if ((t=_get_hex_oct_bin(S))) {
        return t;
    }   
}

Token * _get_sym(Stream *S) {
    unsigned char c;
    //while ((c = _get_char(S)) != '\0' && isblank(c)) ;
    //if (c != '_' && ! isalpha(c)) { _unget_char(S); return NULL;}
    Symbol * buff =new_symbol("", 0); symbol_push_c(buff, c);
    while (((c = _get_char(S)) == ' '|| isalnum(c))) symbol_push_c(buff, c);
    _unget_char(S);
    return new_token(TOKEN_SYM, buff, S->_line, S->_pos - buff->_sp);
}

char _get_esc_char(Stream *S) {
    //     エスケープ文字かどうかを判定し、エスケープ文字ならそれをbuffにいれてbuffの次のアドレスを返す
    //     そうでなければNULLを返す
    char s; 
    //if (get_char(S) == '\\') {
    char c = _get_char(S);
    switch(c) {
        case 'a': s = '\a'; break;  
        case 'b': s = '\b'; break;
        case 'n': s = '\n'; break;
        case 'r': s = '\r'; break;
        case 'f': s = '\f'; break;
        case 't': s = '\t'; break;
        case 'v': s = '\v'; break;
        case '\\': s = '\\'; break;
        //case '\'': s = '\''; break;
        //case '?'
        case '"': s = '"'; break;
        case '0': s = '\0'; break;
        // case 'o': case 'O':
        // default:_unget_char(S);unget_char(S);return NULL; 
        default:_unget_char(S);return -1; 
    }
    return s;    
}

Token *_get_str(Stream *S) {
    //     文字列かどうか判定し、文字列ならそれをトークンとして返す
    //     そうでないならNULLを返す
    //     「"」と「"」で囲まれた文字を文字列リテラルとする
    //     「\」が現れたらエスケープ文字があるとする
    Symbol * buff;
    char cc,*_buff;
    char c = _get_char(S);

    if (c == '"') {
        if ( _get_char(S) == '"' ) {
            if ( _get_char(S) == '"') {
            // RAW string
                while (TRUE) {
                    c = _get_char(S);
                    if (c == '"') {
                        if (_get_char(S) == '"') {
                            if (_get_char(S) == '"') {
                                break;
                            }
                            _unget_char(S);
                        }
                        _unget_char(S);
                    }
                    symbol_push_c(buff,c); 
                }
                return new_token(TOKEN_STR, buff, S->_line, S->_pos - buff->_sp);
            }
            _unget_char(S);
        } 
        // normal string
        _unget_char(S);
        while ((c = _get_char(S)) != '"') {
            if (c == '\\') {
                if ((c = _get_esc_char(S)) < 0) {
                    symbol_push_c(buff, cc);
                }
            } 
            symbol_push_c(buff, c); 
        }
        return new_token(TOKEN_STR, buff, S->_line, S->_pos - buff->_sp);
    }
}

int _get_comm(Stream *S) {
    // commentを読み飛ばす
    // file end に達したらFALSEを返す
    char c,cc,*p;
    int _lvl=0;
    if ((c = _get_char(S))=='/') {
        if ((cc = _get_char(S))=='*') {
            // multi line comment
            _lvl++;
            while ((c = _get_char(S)) != '\0') {
                if ((c = _get_char(S)) == '/') {
                    if ((cc = _get_char(S)) =='*') {
                        _lvl--;
                    }
                } else if (c == '*') {
                    if ((cc = _get_char(S))=='/') {
                        if ((_lvl--) == 0) return TRUE;
                    }
                } 
            }
            return FALSE;
        } else if (cc=='/') {
            // single line comment
            while ((c = _get_char(S)) != '\n') ;
            if (c=='\0') return FALSE;
            _unget_char(S); 
            return TRUE; 
        }
        _unget_char(S);
    }
    _unget_char(S);
    return TRUE;

}

Token * _get_del(Stream *S) {
    //  区切詞 (= 記号)かどうか調べてtokenに入れて返す
    //  記号でないならNULLを返す
    //  tokentypは記号のコードそのものとする
    char c = _get_char(S),cc,ccc;
    Symbol * buff = new_symbol("",0);
    switch(c) {
        case '+':case '-':case '*': case '/':case '%':case '&':case '|':case '^':case '!': case '<':case '=':case '>':
            cc = _get_char(S);
            if (cc == c || cc == '=' || ( c == '-' && cc == '>') || (c=='<' && cc=='-') || (c=='/' && cc=='*')) {
                symbol_push_c(buff, c); symbol_push_c(buff, cc);
                if ((c=='>' && cc=='>') || (c=='<' && cc=='<')) {
                    ccc = _get_char(S);
                    if (ccc=='=') {
                        symbol_push_c(buff, ccc);
                        return new_token(c*65536+cc*256+ccc, buff, S->_line, S->_pos - buff->_sp);
                    }
                    _unget_char(S);
                }
                //if (c=='/' && cc=='/') return is_comm(S,TOKEN_LCOMM);   // '//'はコメント行
                //if (c=='/' && cc=='*') return is_comm(S,TOKEN_COMM);    // '/*'はコメントの開始
                return new_token(c*256+cc, buff, S->_line, S->_pos - buff->_sp);
            } else { 
                _unget_char(S); 
                symbol_push_c(buff, c); 
                return new_token(c, buff, S->_line, S->_pos - buff->_sp);
            }
        case '.':
            cc = _get_char(S);
            if (cc == c) { 
                symbol_push_c(buff, c); 
                symbol_push_c(buff, cc); 
                return new_token(c*256+cc, buff, S->_line, S->_pos -buff->_sp);
            } else {
                _unget_char(S);
                symbol_push_c(buff, c); 
                return new_token(c, buff, S->_line, S->_pos - buff->_sp);
            }
        case '"': case '#': case '$': case '(': case ')': case '\'':case ':': case ';': case '?': \
        case '@': case '[': case '\\':case ']': case '_': case '`': case '{': case '}': case '~': case ',':
            symbol_push_c(buff, c); 
            return new_token(c, buff, S->_line, S->_pos - buff->_sp);
        default:
            return NULL;
    }

}

Token * _get_token(Stream *S) {
    // Stream*Sからtokenを取り出す バッファリングはしないので必要ならget_tokenを使うこと
    // 文字列を最後まで読んだらNULLを返す
    Token * t;
    char c,*p;
    Symbol * token_sym;
    while (TRUE) {
        while ((c = _get_char(S)) != '\0' && (isblank(c) || c == '\n')) ;          // 空白と改行を読み飛ばして
        if (c=='\0') return NULL;                           // NULL文字なら

        if (!_get_comm(S)) return NULL;                                       // コメントを読み飛ばす
        if ((t = _get_num(S)) || (t = _get_sym(S)) || (t = _get_str(S)) ||(t = _get_del(S)) ) return t;  // 数値ならそれをtokenに入れて返す  
        //return NULL;
    }


}
TokenBuff * new_tokenbuff(int fno) {
    TokenBuff * tokens = (TokenBuff*)malloc(sizeof(TokenBuff));
    tokens->S = new_stream(fno);
    tokens->buff = vector_init(10);
    return tokens;
}

TokenBuff * new_str_tokenbuff(Symbol *f) {
    TokenBuff * tokens = (TokenBuff*)malloc(sizeof(TokenBuff));
    tokens->S = new_str_stream(f);
    tokens->buff = vector_init(10);
    return tokens;
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
    t=_get_token(S);//printf("%s\n",t->source->_table);
    if (t == NULL)  {   // tがNULLになったら1行読み込む

        return new_token(TOKEN_EOF,NULL,S->_line,S->_pos);   // EOFの場合TOKEN_EOFを返す
    }
    t->line = S->_line;t->pos = 131312;    
    push(tokenbuff,(void*)t);(tokenbuff->_cp)++;
    return  t; 
}

void unget_token(TokenBuff *tokens) {
    if ((tokens->buff->_cp) == 0) printf("Unget_token is too much!\n"); 
    else (tokens->buff->_cp) --;
    //token_print(tokens); 
}

int main(int argc, char * argv[]) {
    //Stream * S = new_stream(stdin);
    TokenBuff *tokens = new_str_tokenbuff(argv[1]);

    while (TRUE) {
        Token * t = get_token(tokens);
        printf("tokentype:%d\ttokensouce:%s\ttokenline:%d\ttokenpos:%d\n", \
            t -> type, t -> source->_table,t->line,t->pos);
        if (t == NULL) return  - 1;  
    }
}