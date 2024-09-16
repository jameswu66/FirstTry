#include <stdio.h>
#include <errno.h>
#include <direct.h>

int CreateDir(const char *p){
    if(_mkdir(p)==-1){//如果创建目录失败
        if(errno!=EEXIST){//文件已存在
            perror("Error when creating directory!");
            return -1;
        }
    }
    return 0;
}