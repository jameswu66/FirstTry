#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>
#include <openssl/evp.h>
#include <zlib.h>

完成哈希值的计算
void SHA1(const char *file,unsigned char *out){
    FILE *f=fopen(file,"rb");//以二进制只读方式打开文件
    if(f=NULL){
        perror("Error! We can't open file!");//打不开就报错
        return;
    }

    EVP_MD_CTX *mdctx=EVP_MD_CTX_new();//创建并分配一个消息摘要上下文（Message Digest Context），用来存储相关状态信息
    EVP_DigestInit_ex(mdctx, EVP_sha1(), NULL);//初始化MDC，指定使用SHA-1作为哈希算法
    
    unsigned char buffer[4096];//4096字节的缓冲区，用于暂存从文件读取的数据
    size_t readbyte;
    while ((readbyte = fread(buffer, 1, sizeof(buffer), file)) != 0) {
        EVP_DigestUpdate(mdctx, buffer, readbyte);
    }
    //这里进行哈希计算，给一个变量readbyte每次从文件中读取一个字节，如果还能向后读取说明文件未读完；每读完一个字节的数据便将其传入EVP_DigestUpdate函数之中，计算其哈希值
    EVP_DigestFinal_ex(mdctx, out, NULL);//读完生成的哈希值存储在out中
    EVP_MD_CTX_free(mdctx);//释放内存
    fclose(file);//关闭文件
}