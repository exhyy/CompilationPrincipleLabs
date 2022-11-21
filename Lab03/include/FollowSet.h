#ifndef FOLLOWSET_H_
#define FOLLOWSET_H_

#include "FirstSet.h"
#include "utils.h"
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
    void _getData();

public:
    FollowSet(std::string filename, FirstSet firstSet, std::string end);
    std::map<std::string, std::set<std::string>> data();
    std::string epsilon();
    std::string end();
    void print();
};

#endif
