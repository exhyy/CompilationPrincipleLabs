#include "Lab02.h"
#include <string>
#include <iostream>
#include <sstream>

Lab02::Lab02(std::string filename)
{
    infile.open(filename, std::ios::in);
    if (!infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }
}

void Lab02::S()
{
    V();
    if (match(ASSIGN))
        advance();
    else
    {
        this->pass = false;
        std::cerr << "错误：缺少“=”" << std::endl;
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
    if (match(LPARENTHESES))
    {
        advance();
        E();
        if (match(RPARENTHESES))
            advance();
        else
        {
            this->pass = false;
            std::cerr << "错误：缺少“)”" << std::endl;
        }
    }
    else if (match(ID))
        advance();
    else
    {
        this->pass = false;
        std::cerr << "错误：缺少“(”或标识符" << std::endl;
    }
}

void Lab02::V()
{
    if (match(ID))
        advance();
    else
    {
        this->pass = false;
        std::cerr << "错误：缺少标识符" << std::endl;
    }
}

void Lab02::advance()
{
    std::string line;
    getline(infile, line);
    if (line[0] == '(')
    {
        int index = line.find(',');
        std::string wordId = line.substr(1, index - 1);
        std::stringstream stream;
        stream << wordId;
        stream >> this->wordId;
    }
}

bool Lab02::match(int wordId)
{
    return this->wordId == wordId;
}

bool Lab02::run()
{
    this->pass = true;
    advance();
    S();
    return this->pass;
}