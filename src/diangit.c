#include <stdio.h>
#include "Init.h"
#include <string.h>
#include <windows.h>

int main(int argc,char *argv[]){
    const char *dir=NULL;

    //是否提供了目录
    if((argc!=3&&argc!=2)||strcmp(argv[1], "init") != 0){
        fprintf(stderr,"Wrong input!\nTry using: diagit init <directory>\n");
        return 1;
    }else if(argc==2&&strcmp(argv[1], "init")==0){
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

    return 0;
}