#ifndef FIRSTSET_H_
#define FIRSTSET_H_

#include "utils.h"
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <vector>

class FirstSet
{
private:
    std::string _filename;
    std::ifstream _infile;
    std::string _epsilon;
    std::map<std::string, std::set<std::string>> _data;
    std::set<std::string, SymbolCmp> _terminal;
    std::set<std::string, SymbolCmp> _nonterminal;
    void _getNonterminal();
    void _getTerminal();
    void _getData();

public:
    FirstSet(std::string filename, std::string epsilon);
    FirstSet(const FirstSet &firstSet);
    std::set<std::string, SymbolCmp> nonterminal();
    std::set<std::string, SymbolCmp> terminal();
    std::map<std::string, std::set<std::string>> data();
    std::string epsilon();
    std::set<std::string> string(std::string str);
};

#endif