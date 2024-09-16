#ifndef __INIT_H__
#define __INIT_H__

int CreateDir(const char *p);//创建目录
int IsExistGit(const char *p);//判断.git是否已经存在
int InitGitDir(const char *p);//创建.git
int CreateConfig(const char *p);//创建.git内的config文件
int DiangitInit(const char *p);//初始化函数

#endif