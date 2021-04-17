#include <time.h>
#include "vm.h"
#define KEYSIZE 8 
#define TABLESIZE 100000
#define HASHSIZE 3000000

Symbol * new_symbol(unsigned char * str, unsigned  long size) {
    unsigned char * s = (unsigned char * )malloc(size * sizeof(unsigned char) + 1); 
    strcpy(s, str); // printf("%s\n", s);
     // if (strcmp(s, "LDC") == 0) printf("!LDC!\n");  
    Symbol * sym = (Symbol * )malloc(sizeof(Symbol));
    sym -> _size = size; 
    sym -> _table = s;  
    return sym;  
}

int main(){
	char randchar[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	int randchar_num = sizeof(randchar)-1;
	clock_t start, end;
	int i;
//	double sum=0;
	srand(time(NULL));
	
	 // getchar();
	for(i=1; i<=10; ++i){
		int size = i * TABLESIZE;
		int j;
		int recv;
		Data *data_table = (Data *)malloc(sizeof(Data) * size);
		Hash *hash;
		
		// create randkey=>val pare
		for(j=0; j<size; ++j){
			int randkey_size = KEYSIZE;
			int k;
			char *randkey = (char *)malloc(randkey_size + 1);
			int *randval = (int *)malloc(sizeof(int));
			*randval = j;
			for(k=0; k<randkey_size; ++k) randkey[k] = randchar[rand()%randchar_num];
			randkey[randkey_size] = '\0';
			data_table[j].key=new_symbol(randkey,randkey_size);
			//data_table[j].key = randkey;
			//node[j].keysize = randkey_size;
			data_table[j].val = (void*)randval;
			//node[j].valsize = sizeof(int);
		}
		// benchmark!!
		//printf("start!\n");
		start=clock();
		hash = Hash_init(HASHSIZE);
		for(j=0; j<size; ++j){
			Hash_put(hash, data_table[j].key, data_table[j].val);
			if (data_table[j].val !=  (void * )* Hash_get(hash, data_table[j].key)) printf("misswd!\n");
//			sum += recv;
		}
		free(hash);
		end=clock();
		printf("MyHash => %7f 秒 [n=%d]\n", (double)(end-start) / CLOCKS_PER_SEC, size);
		
		// free randkey=>val
		//for(j=0; j<size; ++j){
		//	freeHashNode(node+j);
		//}
		free(data_table);
	}
//	printf("sum: %f\n", sum);
	return 0;
}
