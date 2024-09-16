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

//该创建.git了
int Init_git_dir(const char *p){
    char path_git[1024];
    snprintf(path_git,sizeof(path_git),"%s\\.git",p);//拼接完整路径
    if(CreateDir(path_git)==0){
        return 0;
    }//如果创建出错，则程序退出，且会打印失败的信息
    printf("We Initialize an empty git directory in:\n");
    printf("%s\\.git\\\n",p);//打印路径
    return 1;
}

//创建.git中的内容
//看了一圈也就config最好写，且受到的影响小

