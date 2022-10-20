#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexical.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "错误：参数过少\n");
        exit(3);
    }
    else if (argc > 4)
    {
        fprintf(stderr, "错误：参数过多\n");
        exit(3);
    }
    else
    {
        FILE *src, *dest;
        int _stdout = 0;

        src = fopen(argv[1], "r");
        if (src == NULL)
        {
            fprintf(stderr, "错误：无法读取文件：%s\n", argv[1]);
            exit(4);
        }

        dest = fopen(argv[2], "w+");
        if (dest == NULL)
        {
            fprintf(stderr, "错误：无法读取文件：%s\n", argv[2]);
            exit(4);
        }

        if (argc == 4)
        {
            if (strcmp(argv[3], "--stdout") == 0)
                _stdout = 1;
            else
            {
                fprintf(stderr, "错误：未知参数：%s\n", argv[3]);
                exit(5);
            }
        }

        full_scan(src, dest, _stdout);
    }
    return 0;
}