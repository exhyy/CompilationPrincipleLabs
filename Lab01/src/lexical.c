#include "lexical.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 32

char TOKEN[MAX_TOKEN_LENGTH + 1];
char reserved_word[][32] = {
    "void",
    "int",
    "float",
    "double",
    "if",
    "else",
    "for",
    "do",
    "while"};

int lookup(char *token)
{
    for (int i = 0; i < 9; i++)
    {
        if (strcmp(token, reserved_word[i]) == 0)
            return i + 1;
    }
    return 0;
}

void retract(FILE *fp)
{
    fseek(fp, -1, 1);
}

void out(int c, char *val, FILE *dest, int _stdout)
{
    fprintf(dest, "(%d, %s)\n", c, val);
    if (_stdout)
        fprintf(stdout, "(%d, %s)\n", c, val);
}

void report_error(char *err_msg)
{
    fprintf(stderr, "%s\n", err_msg);
}

void single_scan(FILE *src, FILE *dest, int _stdout)
{
    char ch;
    int i, c;
    ch = fgetc(src);
    if (isalpha(ch))
    {
        // 标识符
        TOKEN[0] = ch;
        ch = fgetc(src);
        i = 1;
        while (isalnum(ch))
        {
            TOKEN[i] = ch;
            i++;
            if (i >= MAX_TOKEN_LENGTH)
            {
                report_error("标识符过长！");
                exit(1);
            }
            ch = fgetc(src);
        }
        TOKEN[i] = '\0';
        if (ch != EOF)
            retract(src);
        c = lookup(TOKEN);
        if (c == 0)
            out(ID, TOKEN, dest, _stdout);
        else
            out(c, "", dest, _stdout);
    }
    else if (isdigit(ch))
    {
        // 无符号整数
        TOKEN[0] = ch;
        ch = fgetc(src);
        i = 1;
        while (isdigit(ch))
        {
            TOKEN[i] = ch;
            i++;
            if (i > MAX_TOKEN_LENGTH)
            {
                report_error("无符号整数过长！");
                exit(1);
            }
            ch = fgetc(src);
        }
        TOKEN[i] = '\0';
        if (ch != EOF)
            retract(src);
        out(UINT, TOKEN, dest, _stdout);
    }
    else
    {
        char err_msg[64]; // switch内不能定义变量
        switch (ch)
        {
        case '+':
            // +或++或+=
            ch = fgetc(src);
            if (ch == '+')
                out(INC, "", dest, _stdout);
            else if (ch == '=')
                out(ADDE, "", dest, _stdout);
            else
            {
                retract(src);
                out(ADD, "", dest, _stdout);
            }
            break;

        case '-':
            // -或--或-=
            ch = fgetc(src);
            if (ch == '-')
                out(DEC, "", dest, _stdout);
            else if (ch == '=')
                out(SUBE, "", dest, _stdout);
            else
            {
                retract(src);
                out(SUB, "", dest, _stdout);
            }
            break;

        case '*':
            // *或*=
            ch = fgetc(src);
            if (ch == '=')
                out(MULE, "", dest, _stdout);
            else
            {
                retract(src);
                out(MUL, "", dest, _stdout);
            }
            break;

        case '/':
            // /或/=
            ch = fgetc(src);
            if (ch == '=')
                out(DIVE, "", dest, _stdout);
            else
            {
                retract(src);
                out(DIV, "", dest, _stdout);
            }
            break;

        case ';':
            // ;
            out(SEMICOLON, "", dest, _stdout);
            break;

        case ',':
            // ,
            out(COMMA, "", dest, _stdout);
            break;

        case '(':
            // (
            out(LPARENTHESES, "", dest, _stdout);
            break;

        case ')':
            // )
            out(RPARENTHESES, "", dest, _stdout);
            break;

        case '{':
            // {
            out(LBRACE, "", dest, _stdout);
            break;

        case '}':
            // }
            out(RBRACE, "", dest, _stdout);
            break;

        case '>':
            // >或>=或>>
            ch = fgetc(src);
            if (ch == '=')
                out(GE, "", dest, _stdout);
            else if (ch == '>')
                out(RSHIFT, "", dest, _stdout);
            else
            {
                retract(src);
                out(GT, "", dest, _stdout);
            }
            break;

        case '<':
            // <或<=或<>或<<
            ch = fgetc(src);
            if (ch == '=')
                out(LE, "", dest, _stdout);
            else if (ch == '>')
                out(NE, "", dest, _stdout);
            else if (ch == '<')
                out(LSHIFT, "", dest, _stdout);
            else
            {
                retract(src);
                out(LT, "", dest, _stdout);
            }
            break;

        case '&':
            // &&
            ch = fgetc(src);
            if (ch == '&')
                out(AND, "", dest, _stdout);
            else
            {
                report_error("非法标识符：&。您是否要使用“&&”？");
                exit(2);
            }
            break;

        case '|':
            // ||
            ch = fgetc(src);
            if (ch == '|')
                out(OR, "", dest, _stdout);
            else
            {
                report_error("非法标识符：|。您是否要使用“||”？");
                exit(2);
            }
            break;

        case '!':
            // !
            out(NOT, "", dest, _stdout);
            break;

        default:
            sprintf(err_msg, "无法识别字符：%c", ch);
            report_error(err_msg);
            exit(2);
        }
    }
}

void full_scan(FILE *src, FILE *dest, int _stdout)
{
    char ch;
    while (1)
    {
        ch = fgetc(src);
        if (ch == EOF)
            break;
        if (ch != ' ' && ch != '\n')
        {
            retract(src);
            single_scan(src, dest, _stdout);
        }
        if (feof(src)) // 检查文件是否读取完毕
            break;
    }
}