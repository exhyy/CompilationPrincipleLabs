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
    idToWord.assign({"", "void", "int", "float", "double", "if", "else", "for", "do", "while", "ID", "UINT", "<", ">", "==", "<=", ">=", "<>", "&&", "||", "!", "+", "-", "*", "/", ";", ",", "(", ")", "{", "}", "++", "--", "<<", ">>", "+=", "-=", "*=", "/=", "="});
}

bool Lab02::S()
{
    if (match(ID))
    {
        if (V())
        {
            if (match(ASSIGN))
            {
                advance();
                return E();
            }
            else
            {
                errorMessage = "缺少“=”";
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        errorMessage = "缺少标识符";
        return false;
    }
}

bool Lab02::E()
{
    if (match(LPARENTHESES) || match(ID))
    {
        if (T())
        {
            return E_prime();
        }
        else
        {
            return false;
        }
    }
    else
    {
        errorMessage = "缺少“(”或标识符";
        return false;
    }
}

bool Lab02::E_prime()
{
    if (match(ADD) || match(SUB))
    {
        if (A())
        {
            if (T())
            {
                return E_prime();
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else if (match(RPARENTHESES) || match(EOI))
    {
        return true;
    }
    else
    {
        errorMessage = "缺少“+”、“-”、“)”之一，或未到结尾";
        return false;
    }
}

bool Lab02::T()
{
    if (match(LPARENTHESES) || match(ID))
    {
        if (F())
        {
            return T_prime();
        }
        else
        {
            return false;
        }
    }
    else
    {
        errorMessage = "缺少“(”或标识符";
        return false;
    }
}

bool Lab02::T_prime()
{
    if (match(MUL) || match(DIV))
    {
        if (M())
        {
            if (F())
            {
                return T_prime();
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else if (match(ADD) || match(SUB) || match(RPARENTHESES) || match(EOI))
    {
        return true;
    }
    else
    {
        errorMessage = "缺少“+”、“-”、“*”、“/”、“)”之一，或未到结尾";
        return false;
    }
}

bool Lab02::F()
{
    if (match(LPARENTHESES))
    {
        advance();
        if (E())
        {
            if (match(RPARENTHESES))
            {
                advance();
                return true;
            }
            else
            {
                errorMessage = "缺少“)”";
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else if (match(ID))
    {
        advance();
        return true;
    }
    else
    {
        errorMessage = "缺少“(”或标识符";
        return false;
    }
}

bool Lab02::A()
{
    if (match(ADD) || match(SUB))
    {
        advance();
        return true;
    }
    else
    {
        errorMessage = "缺少“+”或“-”";
        return false;
    }
}

bool Lab02::M()
{
    if (match(MUL) || match(DIV))
    {
        advance();
        return true;
    }
    else
    {
        errorMessage = "缺少“*”或“/”";
        return false;
    }
}

bool Lab02::V()
{
    if (match(ID))
    {
        advance();
        return true;
    }
    else
    {
        errorMessage = "缺少标识符";
        return false;
    }
}

void Lab02::advance()
{
    current++;
    std::string line;
    getline(infile, line);
    if (line[0] == '(')
    {
        int index1 = line.find(',');
        std::string wordId = line.substr(1, index1 - 1);
        std::stringstream stream;
        stream << wordId;
        stream >> this->wordId;

        int index2 = line.find(')');
        this->wordValue = line.substr(index1 + 2, index2 - index1 - 2);
    }
    else
    {
        this->wordId = EOI;
    }
}

bool Lab02::match(int wordId)
{
    return this->wordId == wordId;
}

bool Lab02::run()
{
    errorMessage = "";
    current = 0;
    advance();
    return S();
}

void Lab02::printError()
{
    int location = 0, cnt = 1;
    int t = current;
    std::cerr << " ";
    infile.seekg(0, std::ios::beg);
    advance();
    while (!match(EOI))
    {
        if (match(ID))
        {
            std::cerr << wordValue;
            if (cnt < t)
                location += wordValue.size();
        }
        else
        {
            std::cerr << idToWord[wordId];
            if (cnt < t)
                location += idToWord[wordId].size();
        }
        advance();
        cnt++;
    }
    std::cerr << std::endl;

    for (int i = 0; i < location; i++)
        std::cerr << " ";
    std::cerr << "^--" << errorMessage << std::endl;
}