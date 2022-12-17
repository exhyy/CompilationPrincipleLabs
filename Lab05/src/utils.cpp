#include "utils.h"
#include <iostream>

std::vector<std::string> splitString(std::string str, std::string separator)
{
    std::vector<std::string> result;
    int index = str.find(separator);
    while (index != -1)
    {
        result.push_back(str.substr(0, index));
        str.erase(0, index + 1);
        index = str.find(separator);
    }
    result.push_back(str);
    return result;
}

std::vector<std::string> splitSymbols(std::string str, const std::set<std::string, SymbolCmp> &terminal, const std::set<std::string, SymbolCmp> &nonterminal, const std::string &epsilon)
{
    std::vector<std::string> result;
    while (str.length() > 0)
    {
        std::string symbol = "";
        for (auto t : terminal)
        {
            if (str.find(t) == 0 && t.length() > symbol.length())
            {
                symbol = t;
            }
        }
        if (symbol == "")
        {
            for (auto n : nonterminal)
            {
                if (str.find(n) == 0 && n.length() > symbol.length())
                {
                    symbol = n;
                }
            }
        }
        if (symbol == "")
        {
            if (str.find(epsilon) == 0)
            {
                symbol = epsilon;
            }
        }
        if (symbol == "")
        {
            std::cerr << "错误：字符串并非全由符号组成" << std::endl;
            std::vector<std::string> emptyVector;
            return emptyVector;
        }
        result.push_back(symbol);
        str.erase(0, symbol.length());
    }
    return result;
}
