#include <iostream>
#include <string.h>
#include "md5.h"
#include "file.h"

using namespace std;
#define DEBUG

char* fileMd5(const char* filename){
    char** lines = loadfile(filename);
    int i=0;
	md5_state_t state;
	md5_byte_t digest[16];
	int di;
	md5_init(&state);
    while(lines[i]!=NULL){
    #ifdef DEBUG
    printf("%s\n",lines[i]);
    #endif
	md5_append(&state, (const md5_byte_t *)lines[i], strlen(lines[i]));
    i++;
    }
	md5_finish(&state, digest);
    char *a=new char[32];
	for (di = 0; di < 16; ++di)
        {
            sprintf(a+di*2,"%02x",digest[di]);
        }      
    return a;
}

char* getmd5(char *test)
{
	md5_state_t state;
	md5_byte_t digest[16];
	int di;

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)test, strlen(test));
	md5_finish(&state, digest);
    char *a=new char[32];
	for (di = 0; di < 16; ++di)
        {
            sprintf(a+di*2,"%02x",digest[di]);
        }      
    return a;
}

int main(int argc,const char** argv){
    char buf[20];
    if(argc<2){
        // string
        cin>>buf;
        char* output = getmd5(buf);
        printf("%s",output);
    }else{
        // file
        char* output = fileMd5(argv[1]);
        printf("%s",output);
    }
        return 0;
}
