#include <stdio.h>
#include "Init.h"
#include <string.h>
#include <windows.h>
#include "hash.h"
#include <openssl/evp.h>

int main(int argc,char *argv[]){
    const char *dir=NULL;

    if(argc<2){
        fprintf(stderr,"Wrong input!\nTry using: diangit <command>, ...");
        return 1;
    }

    //进行初始化
    if(strcmp(argv[1], "init")==0){
        //是否提供了目录
        if((argc!=3&&argc!=2)){
            fprintf(stderr,"Wrong input!\nTry using: diangit init <directory>\n");
            return 1;
        }else if(argc==2){
            char path_dir[1024];
            GetCurrentDirectory(sizeof(path_dir),path_dir);
            dir=path_dir;
        }else{
            dir=argv[2];
        }

        if(DiangitInit(dir)){
            printf("Initialize git  repository successfully.\n");
        }else{
            printf("Fail to initialize git repository.\n");
        }
    }else if(strcmp(argv[1],"hash-object")==0){
        if(argc!=3){
            fprintf(stderr,"Wrong input!\nTry using: diangit hash-object <filename>\n");
            return 1;
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        SHA1(argv[2],hash);

        //需要将得到的二进制转换成十六进制字符串作为可阅读的哈希值
        char objecthash[EVP_MAX_MD_SIZE*2+1];//每个字节转换成一个十六进制
        for(int i=0;i<EVP_MD_size(EVP_sha1());i++){
            //i小于哈希算法生成的哈希值的长度（SHA-1）
            sprintf(&objecthash[i*2],"%02x",hash[i]);//每两个hash[]中的内容存储在objecthash[]
        }

        if(StoreSHA(argv[2],objecthash)){
            printf("We store your object in hash: %s\n",objecthash);//表明存入了以哈希值命名的文件
        }else{
            printf("Fail when storing your object!\n");
        }
    }else if(strcmp(argv[1],"cat-file")==0){
        if(argc!=3){
            fprintf(stderr,"Wrong input!\nTry using: diangit cat-file <hash of object>\n");
            return 1;
        }

        if(!Catfile(argv[2])){
            printf("Fail when reading your object!\n");
        }
    }else{
        fprintf(stderr,"Unknown command\n");
    }
    return 0;
}