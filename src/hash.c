#include <stdio.h>
#include <stdlib.h>
#include "Init.h"
#include <errno.h>
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>
#include <openssl/evp.h>
#include <zlib.h>

// 完成哈希值的计算
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
    fclose(f);                            // 关闭文件
}

// 将哈希值运算的结果存储在文件中
int StoreSHA(const char *file, const char *store)
{
    char Sdir[1024];                                                        // 用于保存路径名
    snprintf(Sdir, sizeof(Sdir), ".git/objects/%.2s", store); // 以哈希值的前两个字符作为子目录的路径
    CreateDir(Sdir);                                                        // 创建目录

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", Sdir, store + 2); // 完整路径：从第三个字符开始的后半部分字符作为文件名，也存储在path中

    FILE *f = fopen(file, "rb");
    if (f = NULL)
    {
        perror("Error when opening file when you want to store your file!");
        return 0;
    } // 打开想要压缩的文件

    fseek(f, 0, SEEK_END);      // 从第一位开始移动到末尾，以计算大小
    size_t Sizefile = ftell(f); // 获取以字节为单位的文件的大小
    fseek(f, 0, SEEK_SET);      // 准备从头开始读文件

    // 读取文件内容
    unsigned char *data = (unsigned char *)malloc(Sizefile);
    fread(data, 1, Sizefile, f); // 读取文件，存储在data中
    fclose(f);

    // 压缩文件
    uLong CompressSize = compressBound(Sizefile); // 在zlib中将unsigned char typedef为uLong
    // 计算压缩后的最大可能大小
    unsigned char *compressdata = (unsigned char *)malloc(CompressSize); // 分配空间
    compress(compressdata, &CompressSize, data, Sizefile);               // 将data中的Sizefile这么多内容压缩到compressdata中，并将压缩后的大小存入CompressSize中

    // 将压缩后的内容写在文件中
    FILE *object = fopen(path, "wb"); // 写入二进制的模式打开path路径
    if (object == NULL)
    {
        perror("Error when creating file!");
        free(data);
        free(compressdata);
        // 如果没打开说明有问题，释放占用的所有内存
        return 0;
    }

    // 写入对象的格式blob
    char head[128];
    int Sizehead = snprintf(head, sizeof(head), "blob%lu\0", Sizefile); // 给blob开头，并写入文件的大小
    fwrite(head, 1, Sizehead, object);                                  // 写入文件头的信息
    fwrite(compressdata, 1, CompressSize, object);                      // 写入压缩后的文件内容

    fclose(object);
    free(data);
    free(compressdata);
    return 1;
}

// 读取file
int Catfile(const char *object)
{
    char path[1024];
    snprintf(path, sizeof(path), ".git/objects/%.2s/%s", object, object + 2);// 读取想要读取文件的路径
    FILE *file = fopen(path, "rb");                                           // 只读模式打开

    if (file == NULL)
    {
        perror("Error when opening file!");
        return 0;
    }

    // 读取文件头
    char head[128];
    fread(head, 1, sizeof(head), file);               // 读文件头，存在head里
    
    char *endhead = memchr(head, '\0', sizeof(head)); // 搜索第一个\0，这个\0的位置存在endhead中，表示文件头的结束
    if (endhead == NULL)
    {
        printf("Wrong head!\n");
        fclose(file);
        return 0;
    } // 找不到则说明文件头有问题

    size_t headlength = endhead - head;         // 计算文件头的字节长度，从末尾的\0的位置到开始
    long Sizefile = strtol(head + 5, NULL, 10); // 跳过blob 的部分，只读后面的文件大小，将十进制的大小存在filesize里
    // 文件可能很大，所以用long类型
    //printf("%lld",Sizefile);//测试

    // 读取压缩后的数据
    unsigned char *compressdata = (unsigned char *)malloc(Sizefile);
    if (compressdata == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return 0;
    }
    fread(compressdata, 1, Sizefile, file); // 读取压缩后Sizefile个字节的数据
    fclose(file);

    // 解压缩
    unsigned char *data = (unsigned char *)malloc(Sizefile * 2); // 由于解压缩数据，所以需要多留空间
    uLongf uncompresssize = Sizefile * 10;                        // 假设最大的解压缩后的大小为原大小的十倍
    if (data == NULL) {
        perror("Memory allocation error");
        free(compressdata);
        return 0;
    }
    uncompress(data, &uncompresssize, compressdata, Sizefile);    // 解压compressdata中的数据，并将数据存在data中，将解压后的大小存在uncompresssize中
    //printf("%s",data);

    fwrite(data, 1, uncompresssize, stdout); // 将文件内容打印至终端
    free(compressdata);
    free(data);
    return 1;
}