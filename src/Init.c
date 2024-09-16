#include <errno.h>
#include <direct.h>

//创建一个目录
int CreateDir(const char *p){
    if(_mkdir(p)==-1){//如果创建目录失败
        perror("Error when creating directory!");
        return 0;
    }
    return 1;
}

//检查是否已经有.git
int IsExistGit(const char *p){
    
}

