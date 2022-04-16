#include "lexparse2.h"

Token * new_token(int type, Symbol * s) {
    Token * t = (Token*)malloc(sizeof(Token));
    t->type=type;
    t->source=s; 
    //t->value.ptr=val; 
    //t->line = line;
    //t->pos = pos;
    return t;
}

Symbol * _get_dec(char ** S) {
    unsigned char * s = * S;
    unsigned char c;
    while ((c = *s++) != '\0' && isblank(c)) ;
    if (c == '\0' || c == '0') { s--; return NULL;}
    Symbol * buff = new_symbol("",0);

    while isdigit(c = *s++) {
        symbol_push_c(buff, c);
    }
    return buff;
}

Symbol * _get_hex_part(char **S) {
    unsigned char * s = * S;
    unsigned char c;
    
    if ((c = *s) == '\0') return NULL;
    Symbol * buff = new_symbol("",0);
    while (((c = *s++) >='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F')) symbol_push_c(buff, c);
    //s--;
    return buff;
}
Symbol * _get_hex(char **S) {
    unsigned char * s = * S;
    unsigned char c;
    while ((c = *s++) != '\0' && isblank(c)) ;
    if (c == '\0' || c != '0') { s--; return NULL;}
    if ((c = *s++) != 'x' && c != 'X') { s--; return NULL;}
    Symbol * buff = new_symbol("0X",2);

    while (((c = *s++) >='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F')) symbol_push_c(buff, c);
    //s--;
    return buff;
}

Symbol * _get_oct(char **S) {
    unsigned char * s = * S;
    unsigned char c;
    while ((c = *s++) != '\0' && isblank(c)) ;
    if (c == '\0' || c != '0') { s--; return NULL;}
    if ((c = *s++) != 'o' && c != 'O') { s--; return NULL;}
    Symbol * buff = new_symbol("0O",2);

    while ((c = *s++) >='0' && c<='8') symbol_push_c(buff, c);
    //s--;
    return buff;

}

Symbol * _get_bin(char **S) {
    unsigned char * s = * S;
    unsigned char c;
    while ((c = *s++) != '\0' && isblank(c)) ;
    if (c == '\0' || c != '0') { s--; return NULL;}
    if ((c = *s++) != 'b' && c != 'B') { s--; return NULL;}
    Symbol * buff = new_symbol("0B",2);

    while ((c = *s++) =='0' || c =='1') symbol_push_c(buff, c);
    //s--;
    return buff;

}

Symbol *_get_int(char **S) {
    unsigned char * s = * S;
    unsigned char c;
    Symbol * sym;
    while ((c = *s++) != '\0' && isblank(c)) ;
    if (c == '\0' ) { s--; return NULL;}
    if ((sym =_get_dec(&s)) || (sym =_get_hex(&s))||(sym =_get_oct(&s))||(sym =_get_bin(&s))) return sym;
}
// 指数部を求める [e|E][+|-]?[0-9]+
Symbol * _get_exp_part(char **S) {

}
// 小数を求める
// 小数: 整数部+指数部 | 整数部+'.'+指数部 | 整数部+'.'+整数部 | 整数部+'.'+整数部+指数部
Symbol * _get_float(char **S) {
    unsigned char * s = * S;
    unsigned char c;
    Symbol * sym, *sym2, *sym3;
    while ((c = *s++) != '\0' && isblank(c)) ;
    if (c == '\0' ) { s--; return NULL;}

    if ((sym = _get_dec(&s))) {
        if ((sym2 == _get_exp_part(&s))) { 
            return symbol_append(sym, sym2);        // 整数部+指数部
        }else if ((c = *s++) == '.') {
            symbol_push_c(sym, c);         
            if (sym2 =  _get_exp_part(&s)) {
                return symbol_append(sym, sym2);        // 整数部+'.'+指数部
            } else if (sym2 = _get_dec(&s)) {
                sym = symbol_append(sym, sym2);
                if (sym2 =_get_exp_part) {
                    return symbol_append(sym, sym2);    // 整数部+'.'+整数部+指数部
                }
                return sym;                             // 整数部+'.'+整数部
            }
            return sym;                                 // 整数部+'.'
        }else if ((c == '/')) {

        }
    }else if ((c = *s++) == '.') {

    }    
}

Symbol * _get_sym(char **S) {
    unsigned char * s = * S;
    unsigned char c;
    while ((c = *s++) != '\0' && isblank(c)) ;
    if (c != '_' && ! isalpha(c)) { s--; return NULL;}
    Symbol * buff =new_symbol(c, 1);
    while (((c = *s++) == ' '|| isalnum(c))) symbol_push_c(buff, c);
    s--;
    return buff;
}

Symbol *_get_str(char **S) {

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
        if (token_sym = _get_dec(&s)) return new_token(TOKEN_INT, token_sym);  // 数値ならそれをtokenに入れて返す  
        if (token_sym = _get_hex(&s)) return new_token(TOKEN_HEX, token_sym);  // 数値ならそれをtokenに入れて返す  
        if (token_sym = _get_oct(&s)) return new_token(TOKEN_OCT, token_sym);  // 数値ならそれをtokenに入れて返す  
        if (token_sym = _get_bin(&s)) return new_token(TOKEN_BIN, token_sym);  // 数値ならそれをtokenに入れて返す  
        if (token_sym = _get_sym(&s)) return new_token(TOKEN_SYM, token_sym);  // 数値ならそれをtokenに入れて返す  
        if (token_sym = _get_str(&s)) return new_token(TOKEN_STR, token_sym);  // 数値ならそれをtokenに入れて返す  
        if (token_sym = _get_del(&s)) return new_token(TOKEN_, token_sym);  // 数値ならそれをtokenに入れて返す  
        if (token_sym = _get_str(&s)) return new_token(TOKEN_STR, token_sym);  // 数値ならそれをtokenに入れて返す  
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