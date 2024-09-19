#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>
#include <openssl/evp.h>
#include <zlib.h>

void SHA1(const char *file,unsigned char *out){
    FILE *f=fopen(file,"rb");
    if(f=NULL){
        perror("Error! We can't open file!");
        return;
    }

    
}