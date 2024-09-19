#ifndef __HASH_H__
#define __HASH_H__

void SHA1(const char *file,unsigned char *out);//哈希值的运算
int StoreSHA(const char *file, const char *store);//存入哈希值命名、压缩后的文件
int Catfile(const char *object);//实行内容解压缩和读取

#endif