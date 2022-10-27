#include <stdio.h>
#include <stdlib.h>

void remove_comment(FILE *src, FILE *dest)
{
    char ch;
    while (1)
    {
        ch = fgetc(src);
        if (ch == EOF)
            break;
        if (ch == '/')
        {
            ch = fgetc(src);
            if (ch == '/')
            {
                // "//"类型注释
                while (ch != '\n' && ch != EOF)
                {
                    ch = fgetc(src);
                }
            }
            else if (ch == '*')
            {
                // "/**/"类型注释
                while (1)
                {
                    ch = fgetc(src);
                    if (ch == '*')
                    {
                        ch = fgetc(src);
                        if (ch == '/')
                        {
                            // 到达注释结尾
                            break;
                        }
                    }
                }
            }
            else
                fputc(ch, dest);
        }
        else
            fputc(ch, dest);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "错误：参数过少\n");
        exit(1);
    }
    else if (argc > 3)
    {
        fprintf(stderr, "错误：参数过多\n");
        exit(1);
    }
    else
    {
        FILE *src, *dest;
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

        remove_comment(src, dest);

        fclose(src);
        fclose(dest);
    }
    return 0;
}