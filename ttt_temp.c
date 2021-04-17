#include <stdio.h>
#include <stdlib.h>
 // #include <string.h>
inline int hash(char *key, int size) {
    int n, h = 0;
    for (n = 0; key[n] != '\0'; n++) {
         // h = (h * 137 + (key[n]&0xff)) & (size - 1);
        h = (h * 137 + (key[n]&0xff));
    }
     // printf("%d\n", h); 
     // return h;
    return h & (size - 1);
}

inline long hashl(char *key, int size) {
    int n;long h = 0;
    for (n = 0; key[n] != '\0'; n++) {
         // h = (h * 137 + (key[n]&0xff)) & (size - 1);
        h = (h * 137 + (key[n]&0xff)); 
    }
     // printf("%ld\n", h); 
     // return h;
    return h & (size - 1);
}

int main(int argc,char*argv[]) {
    char * s = argv[1];
     // int l = strlen(s);
    printf("%d %ld\n",hash(s,1024),hashl(s,1024));
}
