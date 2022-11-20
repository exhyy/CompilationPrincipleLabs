#ifndef FOLLOWSET_H_
#define FOLLOWSET_T_

#include "FirstSet.h"
#include <fstream>
#include <map>
#include <string>

class FollowSet
{
private:
    std::ifstream _infile;
    std::string _epsilon;
    std::string _end;
    FirstSet _firstSet;
    std::set<std::string, SymbolCmp> _terminal;
    std::set<std::string, SymbolCmp> _nonterminal;
    std::map<std::string, std::set<std::string>> _data;
    static std::vector<std::string> _splitString(std::string str, std::string separator);
    std::vector<std::string> _splitSymbols(std::string str);
    void _getData();

public:
    FollowSet(std::string filename, FirstSet firstSet, std::string end);
    std::map<std::string, std::set<std::string>> data();
};

#endif
