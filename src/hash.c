#include <stdio.h>
#include <stdlib.h>
#include "Init.h"
#include <errno.h>
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>
#include <openssl/evp.h>
#include <zlib.h>


//完成哈希值的计算
void SHA1(const char *file, unsigned char *out)
{
    FILE *f = fopen(file, "rb"); // 以二进制只读方式打开文件
    if (f = NULL)
    {
        perror("Error! We can't open file!"); // 打不开就报错
        return;
    }

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();       // 创建并分配一个消息摘要上下文（Message Digest Context），用来存储相关状态信息
    EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL); // 初始化MDC，指定使用SHA-1作为哈希算法

    unsigned char buffer[4096]; // 4096字节的缓冲区，用于暂存从文件读取的数据
    size_t readbyte;
    while ((readbyte = fread(buffer, 1, sizeof(buffer), f)) != 0)
    {
        EVP_DigestUpdate(mdctx, buffer, readbyte);
    }
    // 这里进行哈希计算，给一个变量readbyte每次从文件中读取一个字节，如果还能向后读取说明文件未读完；每读完一个字节的数据便将其传入EVP_DigestUpdate函数之中，计算其哈希值
    EVP_DigestFinal_ex(mdctx, out, NULL); // 读完生成的哈希值存储在out中
    EVP_MD_CTX_free(mdctx);               // 释放内存
    fclose(f);                         // 关闭文件
}

// 将哈希值运算的结果存储在文件中
int StoreSHA(const char *file, const char *store)
{
    char Sdir[1024]; // 用于保存路径名
    snprintf(Sdir, sizeof(Sdir), ".git/objects/%c/%c", store[0], store[1]);//以哈希值的前两个字符作为子目录的路径
    CreateDir(Sdir);//创建目录

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", Sdir, store + 2);//完整路径：从第三个字符开始的后半部分字符作为文件名，也存储在path中

    FILE *f=fopen(file,"rb");
    if(f=NULL){
        perror("Error when opening file when you want to store your file!");
        return 0;
    }//打开想要压缩的文件

    fseek(f,0,SEEK_END);//从第一位开始移动到末尾，以计算大小
    size_t Sizefile=ftell(f);//获取以字节为单位的文件的大小
    fseek(f,0,SEEK_SET);//准备从头开始读文件

    //读取文件内容
    unsigned char *data=(unsigned char *)malloc(Sizefile);
    fread(data,1,Sizefile,f);//读取文件，存储在data中
    fclose(f);

    //压缩文件
    uLong CompressSize=compressBound(Sizefile);//在zlib中将unsigned char typedef为uLong
    //计算压缩后的最大可能大小
    unsigned char *compressdata=(unsigned char *)malloc(CompressSize);//分配空间
    compress(compressdata,&CompressSize,data,Sizefile);//将data中的Sizefile这么多内容压缩到compressdata中，并将压缩后的大小存入CompressSize中

    //将压缩后的内容写在文件中
    FILE *object =fopen(path,"wb");//写入二进制的模式打开path路径
    if(object==NULL){
        perror("Error when creating file!");
        free(data);
        free(compressdata);
        //如果没打开说明有问题，释放占用的所有内存
        return 0;
    }

    //写入对象的格式blob
    char head[128];
    int Sizehead=snprintf(head,sizeof(head),"blob%lu\0",Sizefile);//给blob开头，并写入文件的大小
    fwrite(head,1,Sizehead,object);//写入文件头的信息
    fwrite(compressdata,1,CompressSize,object);//写入压缩后的文件内容

    fclose(object);
    free(data);
    free(compressdata);
    return 1;
}