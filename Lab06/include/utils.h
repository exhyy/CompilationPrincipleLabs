#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>
#include <set>

struct SymbolCmp
{
    bool operator()(const std::string &str1, const std::string &str2) const
    {
        if (str1.length() != str2.length())
            return str1.length() > str2.length();
        return str1 < str2;
    }
};

std::vector<std::string> splitString(std::string str, std::string separator);
std::vector<std::string> splitSymbols(std::string str, const std::set<std::string, SymbolCmp> &terminal, const std::set<std::string, SymbolCmp> &nonterminal, const std::string &epsilon);

#endif