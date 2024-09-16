#include <stdio.h>
#include <errno.h>
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>

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
    if(_stat(is_path_exist,&st_dir)==0&&(st_dir.st_mode & _S_IFDIR)){//如果有这个路径，且这个路径是一个目录
        return 1;//.git存在
    }
    return 0;
}

//该创建.git了
int InitGitDir(const char *p){
    char path_git[1024];
    snprintf(path_git,sizeof(path_git),"%s\\.git",p);//拼接完整路径
    if(CreateDir(path_git)==0){
        return 0;
    }//如果创建出错，则程序退出，且会打印失败的信息
    printf("We Initialize an empty git directory in:");
    printf("%s\\.git\\\n",p);//打印路径
    return 1;
}

//创建.git中的内容
//看了一圈也就config最好写，且受到的影响小
int CreateConfig(const char *p){
    char path_config[1024];
    snprintf(path_config,sizeof(path_config),"%s\\.git\\config",p);//拼接完整路径
    FILE *file_config=fopen(path_config,"w");//要写文件
    if(file_config==NULL){//如果打不开相应路径或者找不到路径
        perror("Error when creating config!\n");
        return 0;
    }

    //写入信息(初始内容)
    fprintf(file_config,"[core]\n");
    fprintf(file_config,"\trepositoryformatversion=0\n");//仓库版本为0
    fprintf(file_config,"\tfilemode=false\n");//追踪权限位，不追踪权限变化
    fprintf(file_config,"\tbare=false\n");//是否为裸仓库，因为有工作目录，所以是非裸仓库
    fprintf(file_config,"\tlogallrefupdates = true\n");//保证更新时内容会放在日志中
    fprintf(file_config,"\tsymlinks = false\n");//不会处理符号链接（符号链接：有点像快捷方式？）
    fprintf(file_config,"\tignorecase = true\n");//不区分大小写

    fclose(file_config);

    return 1;
}

//diangit init用
int DiangitInit(const char *p){
    char path_dir[1024];

    //是否指定目录；如果不指定则当前目录
    if(p==NULL){
        GetCurrentDirectory(sizeof(path_dir),path_dir);//获取当前目录，存在path_dir中，用于创建.git
    }else{
        snprintf(path_dir,sizeof(path_dir),"%s",p);//如果指定了，就复制进缓冲数组
    }

    if(IsExistGit(path_dir)){//如果存在.git说明已经是git仓库
        printf("You are reinitializing git repository in path:");
        printf("%s\\.git\\\n",path_dir);
        return 0;
    }

    if(!InitGitDir(path_dir)){//初始化.git，有问题直接return 0
        return 0;
    }

    if(!CreateConfig(path_dir)){
        return 0;
    }

    return 1;
}