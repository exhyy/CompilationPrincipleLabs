#ifndef FIRSTSET_H_
#define FIRSTSET_H_

#include <string>
#include <fstream>
#include <map>
#include <set>
#include <vector>

struct SymbolCmp
{
    bool operator() (const std::string &str1, const std::string &str2) const
    {
        if (str1.length() != str2.length())
            return str1.length() > str2.length();
        return str1 < str2;
    }
};

class FirstSet
{
private:
    std::ifstream _infile;
    std::string _epsilon;
    std::map<std::string, std::set<std::string>> _data;
    std::set<std::string, SymbolCmp> _terminal;
    std::set<std::string, SymbolCmp> _nonterminal;
    static std::vector<std::string> _splitString(std::string str, std::string separator);
    std::vector<std::string> _splitSymbols(std::string str);
    void _getNonterminal();
    void _getTerminal();
    void _getData();

public:
    FirstSet(std::string filename, std::string epsilon);
    std::set<std::string, SymbolCmp> nonterminal();
    std::set<std::string, SymbolCmp> terminal();
    std::map<std::string, std::set<std::string>> data();
};

#endif