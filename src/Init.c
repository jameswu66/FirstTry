#include <stdio.h>
#include <errno.h>
#include <direct.h>
#include <sys/stat.h>

//创建一个目录
int CreateDir(const char *p){
    if(_mkdir(p)==-1){//如果创建目录失败
        perror("Error when creating directory!");
        return 0;
    }
    return 1;
}

//检查是否已经有.git
//逻辑如下：写一段路径，路径中拼接.git
int IsExistGit(const char *p){
    struct _stat st_dir;//目录信息结构体
    char is_path_exist[1024];//路径名
    snprintf(is_path_exist, sizeof(is_path_exist), "%s\\.git", p);//将拼接的路径传入is_path_exist
    if(_stat(is_path_exist,&st_dir)==0&&(st.st_mode & _S_IFDIR)){//如果有这个路径，且这个路径是一个目录
        return 1;//.git存在
    }
    return 0;
}

