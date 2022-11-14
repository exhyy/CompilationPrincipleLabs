#include "Lab02.h"
#include <string>

Lab02::Lab02(std::string filename)
{
    fp = fopen(filename.c_str(), "r");
}

void Lab02::S()
{
    V();
    if (match(EQ))
        advance();
    else
    {
        this->pass = false;
        fprintf(stderr, "错误：缺少“=”\n");
    }
    E();
}

void Lab02::E()
{
    T();
    E_prime();
}

void Lab02::E_prime()
{
    if (match(ADD) || match(SUB))
    {
        advance();
        T();
        E_prime();
    }
}

void Lab02::T()
{
    F();
    T_prime();
}

void Lab02::T_prime()
{
    if (match(MUL) || match(DIV))
    {
        advance();
        F();
        T_prime();
    }
}

void Lab02::F()
{
    if (match(LP))
    {
        advance();
        E();
        if (match(RP))
            advance();
        else
        {
            this->pass = false;
            fprintf(stderr, "错误：缺少“)”\n");
        }
    }
    else if (match(ID))
        advance();
    else
    {
        this->pass = false;
        fprintf(stderr, "错误：缺少“(”或标识符\n");
    }
}

void Lab02::V()
{
    if (match(ID))
        advance();
    else
    {
        this->pass = false;
        fprintf(stderr, "错误：缺少标识符\n");
    }
}

void Lab02::advance()
{
    char ch;
    std::string tmp_word = "";
    ch = fgetc(fp);
    if (isalpha(ch))
    {
        tmp_word.push_back(ch);
        ch = fgetc(fp);
        while (isalnum(ch))
        {
            tmp_word.push_back(ch);
            ch = fgetc(fp);
        }
        fseek(fp, -1, 1);
        lookahead = tmp_word;
    }
    else
    {
        tmp_word.push_back(ch);
        lookahead = tmp_word;
    }
}

bool Lab02::match(int word_id)
{
    if (word_id == EOI)
        return lookahead[0] == EOF;
    if (word_id == EQ)
        return lookahead == "=";
    if (word_id == LP)
        return lookahead == "(";
    if (word_id == RP)
        return lookahead == ")";
    if (word_id == ADD)
        return lookahead == "+";
    if (word_id == SUB)
        return lookahead == "-";
    if (word_id == MUL)
        return lookahead == "*";
    if (word_id == DIV)
        return lookahead == "/";
    if (word_id == ID)
    {
        for (int i = 0; i < int(lookahead.length()); i++)
        {
            if (i == 0)
            {
                if (!isalpha(lookahead[i]))
                    return false;
            }
            else
            {
                if (!isalnum(lookahead[i]))
                    return false;
            }
        }
        return true;
    }
    return false;
}

void Lab02::run()
{
    this->pass = true;
    advance();
    S();
    if (this->pass)
        fprintf(stdout, "语法分析通过！\n");
}