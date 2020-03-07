/*
 * @
 * @Description  : keeloq 编码 解码
 * @Author       : LiPingSheng
 * @Date         : 2020-03-05 11:41:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long u32;
typedef unsigned long long u64;

#define KeeLoq_NLF 0x3A5C742E
#define bit(x, n) (((x) >> (n)) & 1)
#define g5(x, a, b, c, d, e) (bit(x, a) + bit(x, b) * 2 + bit(x, c) * 4 + bit(x, d) * 8 + bit(x, e) * 16)

// 加密
u32 KeeLoq_Encrypt(u32 data, u32 key)
{
    u32 x = data, r;

    for (r = 0; r < 528; r++)
    {
        x = (x >> 1) ^ ((bit(x, 0) ^ bit(x, 16) ^ (u32)bit(key, r & 63) ^ bit(KeeLoq_NLF, g5(x, 1, 9, 20, 26, 31))) << 31);
    }

    return x;
}

// 解密
u32 KeeLoq_Decrypt(u32 data, u32 key)
{
    u32 x = data, r;
    for (r = 0; r < 528; r++)
    {
        x = (x << 1) ^ (bit(x, 31)) ^ (bit(x, 15)) ^ (bit(key, (15 - r) & 63)) ^ bit(KeeLoq_NLF, g5(x, 0, 8, 19, 25, 30));
    }

    return x;
}

int tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + 'a' - 'A';
    }
    else
    {
        return c;
    }
}

u64 htoi(char s[])
{
    int i;
    u64 n = 0;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
        i = 2;
    }
    else
    {
        i = 0;
    }
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'); ++i)
    {
        if (tolower(s[i]) > '9')
        {
            n = 16 * n + (10 + tolower(s[i]) - 'a');
        }
        else
        {
            n = 16 * n + (tolower(s[i]) - '0');
        }
    }
    return n;
}

void showHelp()
{
    printf("\n");
    printf("KEELOQ Encrypt or Decrypt v1.0.0\n");
    printf("example \"keeloq encrypt -k 0x0123456789abcdef -d 0x76543210\"\n");
    printf("example \"keeloq decrypt -k 0x0123456789abcdef -d 0x76543210\"\n");
    printf("[encrypt] [decrypt] [-k] [-d]\n");
    printf("-k 64bit key\n");
    printf("-d 32bit data\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        showHelp();
        return 0;
    }

    u64 key = 0;
    u32 data = 0;
    char *action;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp("encrypt", argv[i]) == 0 || strcmp("decrypt", argv[i]) == 0)
        {
            action = argv[i];
            continue;
        }

        if (strcmp("-k", argv[i]) == 0)
        {
            if ((i + 1) >= argc)
            {
                showHelp();
                return 0;
            }

            key = htoi(argv[i + 1]);
            continue;
        }

        if (strcmp("-d", argv[i]) == 0)
        {
            if ((i + 1) >= argc)
            {
                showHelp();
                return 0;
            }
            data = htoi(argv[i + 1]);
            continue;
        }
    }
    printf("------------------------------------\n");
    printf("Primary Data:0x%08lx\n", data);
    printf("Key:0x%016llx\n", key);

    if (strcmp("encrypt", action) == 0)
    {
        u32 enData = KeeLoq_Encrypt(data, key);
        printf("Encrypt Data:0x%08lx\n", enData);
        printf("------------------------------------\n");
        return 0;
    }

    if (strcmp("decrypt", action) == 0)
    {
        u32 deData = KeeLoq_Decrypt(data, key);
        printf("Decrypt Data:0x%08lx\n", deData);
        printf("------------------------------------\n");
        return 0;
    }
    showHelp();

    return 0;
}