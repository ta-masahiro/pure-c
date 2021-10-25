#include "lexparse.h"

#define MAXBUFF 1024

Stream  * new_stream(FILE * f) {
    Stream * S = (Stream * )malloc(sizeof(Stream) );
    S ->_pos = 0;
    S ->_line = 0;
    S ->_buff = (char * )malloc(MAXBUFF * sizeof(char));  
    char * p = fgets(S ->_buff, MAXBUFF, f); 
    S -> _fp = f;
    if (p == NULL) return NULL;  
    return S; 
}

char * re_load(Stream * S) {
    char * p; 
    S ->_pos = 0;
    (S ->_line)++; 
    p = fgets(S ->_buff, MAXBUFF, S ->_fp);
     // if (p == NULL) printf("faile EOF\n");
    return p;   
}

char get_char(Stream * S) {
    char  * p;  
    char c = S ->_buff[(S ->_pos) ++ ];
    // if (c == '\n') {
    //     p = re_load(S);
    //     if (p) return EOF;
    // }
    return c;  
}

void unget_char(Stream * S) { (S ->_pos) -- ; }

token * new_token(int type, Symbol * s, void * val, Stream * S) {
    token * t = (token*)malloc(sizeof(token));
    t->type=type;
    t->source=s; 
    t->value.ptr=val; 
    t->line = S->_line;
    // t->pos = (S->_pos) - (s -> _size);
    t->pos = S->_pos;
    return t;
} 

char STR_BUFF[4096];

token*is_NUM(Stream*S, tokenstate s, char* buff);
token*is_SYM(Stream*S, tokenstate s, char* buff);
token*is_STR(Stream*S, tokenstate s, char* buff);
token*is_CHR(Stream*S, tokenstate s, char* buff);
token*is_DEL(Stream*S, tokenstate s, char* buff);

token * is_NUM(Stream * S, tokenstate s, char* buff ) {
    //     数値かどうか判定し、数値ならそれをトークンとして返す
    //     そうでないならNULLを返す
    //     数値にはINT(整数)、RAT(分数)、FLT(小数)、EFLT(指数付き少数)がある
    token * t; char cc,ccc;
    char c = get_char(S);
    switch (s) {
        case TOKEN_NONE:                          // 状態なしの場合
            if (c == '.') {                 // 「.」で始まれば小数
                cc=get_char(S);
                if (isdigit(cc)) {
                    *(buff++)=c;*(buff++)=cc;
                    return is_NUM(S,TOKEN_FLT,buff);
                } else {
                    unget_char(S);
                    unget_char(S);
                    return NULL;
                }
            } else if (c=='0') {
                cc=get_char(S);
                if (cc=='x' || cc=='X') {   // 16進
                    return is_NUM(S,TOKEN_HEX,buff);                
                } else if (cc=='o' || cc=='O') {
                    return is_NUM(S,TOKEN_OCT,buff);                
                } else if (cc=='b' || cc=='B') {
                    return is_NUM(S,TOKEN_BIN,buff);                
                } else {
                    unget_char(S);
                    *(buff++)=c;
                    return is_NUM(S,TOKEN_INT,buff);
                }
            } else if (isdigit(c)) {        // 数字で始まれば整数(仮) 
                * (buff ++) =c; 
                return is_NUM(S, TOKEN_INT, buff); 
            } else {                        // それ以外は「数」ではない
                unget_char(S); 
                return NULL; 
            }
        case TOKEN_INT:                           // 「整数」状態
            if (c == '.') {                 // 「.」が来たら小数に移行 
                *(buff ++) = c; 
                return is_NUM(S, TOKEN_FLT, buff);
            } else if (c=='L' || c=='l') {
                return new_token(TOKEN_LINT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            //} else if (c=='F' || c=='f') {
            //    return new_token(TOKEN_LEFLT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            } else if (c =='/') {           // 「/」が来て数字が続けば分数に移行
                cc = get_char(S); 
                if (isdigit(cc)) {
                    * (buff ++) = c;
                    * (buff ++) = cc; 
                    return is_NUM(S,TOKEN_RAT,buff);
                } else {
                    unget_char(S); 
                    unget_char(S); 
                    return new_token(TOKEN_INT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
                }
            } else if ( c=='e' || c=='E' || c=='f' ||c=='F' ) {// 「e」「E」「f」「F」が来たら指数付き小数かどうか試す 
                cc=get_char(S);
                if (isdigit(cc)) {
                    //*(buff ++) = c; 
                    *(buff ++) = 'e'; 
                    *(buff++) = cc; 
                    return is_NUM(S, (c=='e' || c=='E') ? TOKEN_EFLT :TOKEN_LEFLT, buff);
                } else if (cc=='+' || cc=='-') { 
                    ccc=get_char(S);
                    if (isdigit(ccc)) {
                        //*(buff++)=c;
                        *(buff++)='e';
                        *(buff++)=cc;
                        *(buff++)=ccc;
                        return is_NUM(S,(c=='e' || c=='E') ? TOKEN_EFLT : TOKEN_LEFLT,buff);
                    } else {
                        unget_char(S); 
                        unget_char(S); 
                        unget_char(S);
                        return new_token(TOKEN_INT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
                    }
                } else {
                    unget_char(S); 
                    unget_char(S); 
                    return new_token(TOKEN_INT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
                }
            } else if (isdigit(c)) { 
                * (buff ++) =c; 
                return is_NUM(S, s, buff); 
            } else {
                unget_char(S); 
                return new_token(TOKEN_INT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            }
        case TOKEN_RAT:
            if (isdigit(c)) {
                *(buff ++) = c; 
                return is_NUM(S,s,buff); 
            } else {
                unget_char(S); 
                return new_token(TOKEN_RAT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            }
        case TOKEN_FLT:
            if ( c=='e' || c=='E' || c=='f' || c=='F') { 
                cc=get_char(S);
                if (isdigit(cc)) {
                    //*(buff ++) = c; 
                    *(buff ++) = 'e'; 
                    *(buff++) = cc; 
                    return is_NUM(S, (c=='e' || c=='E') ? TOKEN_EFLT : TOKEN_LEFLT, buff);
                } else if (cc=='+' || cc=='-') { 
                    ccc=get_char(S);
                    if (isdigit(ccc)) {
                        //*(buff++)=c;
                        *(buff++)='e';
                        *(buff++)=cc;
                        *(buff++)=ccc;
                        return is_NUM(S,(c=='e' || c=='E') ? TOKEN_EFLT : TOKEN_LEFLT, buff);
                    } else {
                        unget_char(S); 
                        unget_char(S); 
                        unget_char(S);
                        return new_token(TOKEN_INT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
                    }
                } else {
                    unget_char(S); 
                    unget_char(S); 
                    return new_token(TOKEN_FLT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
                }
            } else if (isdigit(c)) {
                *(buff ++) = c; 
                return is_NUM(S, TOKEN_FLT, buff);
            } else {
                unget_char(S);
                return new_token(TOKEN_FLT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            }
        case TOKEN_EFLT:
            if (isdigit(c)) {
                *(buff ++) = c; 
                return is_NUM(S,s,buff); 
            } else {
                unget_char(S); 
                return new_token(TOKEN_EFLT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            }
        case TOKEN_LEFLT:
            if (isdigit(c)) {
                *(buff ++) = c; 
                return is_NUM(S,s,buff); 
            } else {
                unget_char(S); 
                return new_token(TOKEN_LEFLT, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            }
        case TOKEN_HEX:
            if ((c>='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F')) {
                *(buff++)=c;
                return is_NUM(S,s,buff);
            } else {
                unget_char(S);
                return new_token(TOKEN_HEX,new_symbol(STR_BUFF,buff-STR_BUFF),(void*)0,S);
            }
        case TOKEN_OCT:
            if (c>='0' && c<='7') {
                *(buff++)=c;
                return is_NUM(S,s,buff);
            } else {
                unget_char(S);
                return new_token(TOKEN_OCT,new_symbol(STR_BUFF,buff-STR_BUFF),(void*)0,S);
            }
        case TOKEN_BIN:
            if (c>='0' && c<='1') {
                *(buff++)=c;
                return is_NUM(S,s,buff);
            } else {
                unget_char(S);
                return new_token(TOKEN_BIN,new_symbol(STR_BUFF,buff-STR_BUFF),(void*)0,S);
            }
    }
}

token * is_SYM(Stream*S,tokenstate s, char* buff) {
    //     SYMBOLかどうか判定し、SYMBOLならそれをトークンとして返す
    //     そうでないならNULLを返す
    //     「_」が英字で始まり、「_」か英数字が続く文字列をSYMBOLリテラルとする
    char c = get_char(S);
    switch (s) {
        case TOKEN_NONE:
            if (c=='_' || isalpha(c)) { 
                *(buff++) = c; 
                return is_SYM(S, TOKEN_SYM, buff);
            } else { 
                unget_char(S);
                return NULL;
            } 
        case TOKEN_SYM:
            if (c == '_' || isalnum(c)) {
                 *(buff++) = c; 
                 return is_SYM(S, s, buff);
             }else {
                 unget_char(S); 
                 return new_token(TOKEN_SYM, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
             }
    }
}

char * get_esc_char(Stream *S, char * buff) {
    //     エスケープ文字かどうかを判定し、エスケープ文字ならそれをbuffにいれてbuffの次のアドレスを返す
    //     そうでなければNULLを返す
    char s; 
    //if (get_char(S) == '\\') {
        char c=get_char(S);
        switch(c) {
            case 'a': s = '\a'; break;  
            case 'b': s = '\b'; break;
            case 'n': s = '\n'; break;
            case 'r': s = '\r'; break;
            case 'f': s = '\f'; break;
            case 't': s = '\t'; break;
            case 'v': s = '\v'; break;
            case '\\': s = '\\'; break;
            case '\'': s = '\''; break;
            case '"': s = '"'; break;
            case '0': s = '\0'; break;
            // case 'o': case 'O':
           // default:unget_char(S);unget_char(S);return NULL; 
            default:unget_char(S);return NULL; 
        }    
        * (buff ++ ) = s;
        //printf("%d\n",(int)s);
        return buff;   
    //} else {
    //    unget_char(S); 
    //    return NULL;
    //    } 
}   
token * is_STR(Stream*S,tokenstate s, char* buff) {
    //     文字列かどうか判定し、文字列ならそれをトークンとして返す
    //     そうでないならNULLを返す
    //     「"」と「"」で囲まれた文字を文字列リテラルとする
    //     「\」が現れたらエスケープ文字があるとする
    char cc,*_buff;
    char c = get_char(S);
    switch (s) {
        case TOKEN_NONE:
            if (c == '"') return is_STR(S, TOKEN_STR, buff);
            else { 
                unget_char(S);
                return NULL;
            }
        case TOKEN_STR:
            if (c == '"') { 
                return new_token(TOKEN_STR, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            } else if (c=='\\') {
                if (_buff=get_esc_char(S, buff)) return is_STR(S, s, _buff);
            } else if (c=='\n') {
                *(buff++) = c;
                re_load(S);
                return is_STR(S,s,buff);
            } else { 
                *(buff++) = c; 
                return is_STR(S, s, buff);
            }
    }
}

token * is_CHR(Stream*S,tokenstate s, char* buff) {
    //     文字かどうか判定し、文字ならそれをトークンとして返す
    //     そうでないならNULLを返す'
    //     「'」と「'」で囲まれた１文字を文字リテラルとする
    //     「\」が現れたらエスケープ文字があるとする
    char cc, ccc, *_buff;
    char c = get_char(S);
    if (c == '\'') { 
        cc = get_char(S);
        if (cc == '\\') {
            if (_buff = get_esc_char(S, buff)) {    // エスケープ文字
                if ((ccc = get_char(S)) == '\'') {
                    //*(_buff++) = cc; 
                    return new_token(TOKEN_CHR, new_symbol(STR_BUFF, _buff - STR_BUFF), (void*)0, S);
                } else {
                    unget_char(S); 
                    unget_char(S); 
                    unget_char(S); 
                    return NULL;
                }
            } else {
                unget_char(S); 
                unget_char(S); 
                return NULL;
            }
        } else if (cc == '\'') {      // 空文字
            return new_token(TOKEN_CHR,new_symbol(STR_BUFF,0),(void*)0,S);
        } else {
            if ((ccc=get_char(S)) == '\'') {
                *(buff++) = cc; 
                return new_token(TOKEN_CHR, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0, S);
            } else {
                unget_char(S); 
                unget_char(S); 
                unget_char(S); 
                return NULL;
            }
        }
    } else { 
        unget_char(S);
        return NULL;
    }
}


token * is_DEL(Stream*S, tokenstate s, char* buff) {
    //  区切詞 (= 記号)かどうか調べてtokenに入れて返す
    //  記号でないならNULLを返す
    //  tokentypは記号のコードそのものとする
    char c=get_char(S),cc,ccc;
    switch(c) {
        case '+':case '-':case '*': case '/':case '%':case '&':case '|':case '^':case '!': case '<':case '=':case '>':
            cc=get_char(S);
            if (cc == c || cc == '=' || ( c == '-' && cc == '>') || (c=='<' && cc=='-') || (c=='/' && cc=='*')) {
                *(buff ++ ) = c; *(buff ++ ) = cc;
                if ((c=='>' && cc=='>') || (c=='<' && cc=='<')) {
                    ccc=get_char(S);
                    if (ccc=='=') {
                        *(buff ++) =ccc;
                        return new_token(c*65536+cc*256+ccc,new_symbol(STR_BUFF, buff-STR_BUFF),(void*)0,S);
                    }
                    unget_char(S);
                }
                //if (c=='/' && cc=='/') return is_comm(S,TOKEN_LCOMM);   // '//'はコメント行
                //if (c=='/' && cc=='*') return is_comm(S,TOKEN_COMM);    // '/*'はコメントの開始
                return new_token(c*256+cc, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0,S);
            } else { 
                unget_char(S); 
                *(buff ++ ) = c; 
                return new_token(c, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0,S);
            }
        case '.':
            cc=get_char(S);
            if (cc == c) { 
                *(buff ++ ) = c; 
                *(buff ++ ) = cc; 
                return new_token(c*256+cc, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0,S);
            } else {
                unget_char(S);
                *(buff ++ ) = c; 
                return new_token(c, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0,S);
            }
        case '"': case '#': case '$': case '(': case ')': case '\'':case ':': case ';': case '?': \
        case '@': case '[': case '\\':case ']': case '_': case '`': case '{': case '}': case '~': case ',':
            *(buff ++ ) = c; 
            return new_token(c, new_symbol(STR_BUFF, buff - STR_BUFF), (void*)0,S);
        default:
            return NULL;
    }
}
/*
int is_comm(Stream *S, tokenstate s,int comm_lvl) {
    char c=get_char(S),cc,*p;
    if (s==TOKEN_NONE) {
        if (c=='/') {
            if ((cc=get_char(S))=='*') {
                return is_comm(S,TOKEN_COMM,1);
            } else if (cc=='/') {
                return is_COMM(S,TOKEN_LCOMM,1);
            }
            unget_char(S);unget_char(S);
        }
        unget_char(S);return TRUE;
    } else if (s==TOKEN_LCOMM) {
        while ((c=get_char(S) != '\n')) {;}
        if ((p=re_load(S))==NULL) return FALSE; 
        return TRUE; 
    } else if (s==TOKEN_COMM) {
        while ((c=get_char))

    }
}
*/
token * _get_token(Stream * S){
    // ストリームからtokenを取り出す
    // tokenがなくなったらNULLを返す  
    token * t;
    char c,*p;
    while (isblank(c=get_char(S))) ;            // 空白を読み飛ばして 
    while (c == '\n') { 
        p=re_load(S);                           // 改行文字ならもう1行読み込んで 
        if (p==NULL) return NULL;
        while (isblank(c=get_char(S)));               // 
    }
    unget_char(S);                         // そうでなければ読み込んだ文字を返して
    // if (c == EOF) return NULL; 
    // unget_char(S); 
    // is_comm(S,TOKEN_NONE);                              // コメントを取り去る

    if (t = is_NUM(S, TOKEN_NONE, STR_BUFF)) return t;  // 数値ならそれをtokenに入れて返す  
    if (t = is_SYM(S, TOKEN_NONE, STR_BUFF)) return t;  // シンボルなら
    if (t = is_STR(S, TOKEN_NONE, STR_BUFF)) return t;  // 文字列なら  
    if (t = is_CHR(S, TOKEN_NONE, STR_BUFF)) return t;  // 文字なら  
    if (t = is_DEL(S, TOKEN_NONE, STR_BUFF)) return t;  // 記号なら 

    if (t == NULL) return NULL;                        // NULLならNULLを返す
    // いずれでもない場合
    printf("SyntaxError:Ileagal Token!\n");
    Throw(0); 
}


/*
Vector * tokenbuff; 

token * get_token(Stream * S) {
    if (is_queu_empty(tokenbuff)) return _get_token(S);
    else return dequeue(tokenbuff); 
}
void unget_token(Stream * S) {
    if ((tokenbuff -> _cp) == 0) printf("Unget_token is too much!\n"); 
    else (tokenbuff -> _cp) --; 
}

int main(int argc, char * argv[]) {
    Stream * S = new_stream(stdin);
    tokenbuff = vector_init(100);

    while (TRUE) {
        token * t = get_token(S);
        printf("tokentype:%d\ttokensouce:%s\ttokenline:%d\ttokenpos:%d\n", \
            t -> type, t -> source->_table,t->line,t->pos);
        if (t == NULL) return  - 1;  
    }
}

*/