#include <stdio.h>
#include <stdlib.h>
#define TRUE (1)
#define FALSE (0)
// 
//  https://postd.cc/build-your-own-regex/
//
int match_1(char p, char c) {
    if ( p == '\0') return TRUE; 
    if ( c == '\0') return FALSE; 
    return  p ==  c; 
}
int match(char * , char * ); 

int match_q(char * pattern, char * text) {
    return (match_1( * pattern,  * text) && (match(pattern + 2, text + 1))) || match(pattern + 2, text); 
}
int match_s(char * pattern, char * text) {
    return (match_1( * pattern,  * text) && (match(pattern, text + 1))) || match(pattern + 2, text); 
}
int match(char * pattern, char * text) {
    if ( * pattern == '\0') return TRUE;
    else if ( * pattern == '$' &&  * text  == '\0') {
        return TRUE; 
    } else if ( * (pattern + 1) == '?') {
        return match_q(pattern, text); 
    } else if ( * (pattern + 1) == '*') {
        return match_s(pattern, text); 
    }else if ( * pattern == '.' &&  * text != '\0' ) {
        return match(pattern + 1, text + 1); 
    } else return match_1( * pattern,  * text) && match(pattern + 1, text + 1); 
}
int search(char * pattern, char * text) {
    int i = 0; 
    if ( * pattern == '^') {
        return match(pattern + 1, text);
    } else {
        while  (* text != '\0') {
            if (match(pattern, text ++ )) return TRUE;
        }
        return FALSE;             
    }   
}
int main(int argc, char * argv[]) {
    char * pattern = argv[1]; 
    char * text = argv[2]; 
     printf("%d\n", search(pattern, text)); 
    // printf("%d\n", match(pattern, text)); 
}
