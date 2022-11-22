#ifndef PRECEDENCETABLE_H_
#define PRECEDENCETABLE_H_

#include "utils.h"
#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <map>

#define PRECEDENCE_LT -1
#define PRECEDENCE_EQ 0
#define PRECEDENCE_GT 1

typedef std::pair<std::string, std::string> PSS;

class PrecedenceTable
{
private:
    std::ifstream _infile;
    std::set<std::string, SymbolCmp> _terminal;
    std::set<std::string, SymbolCmp> _nonterminal;
    std::map<PSS, int> _data;
    void _getNonterminal();
    void _getTerminal();
    void _getData();

public:
    PrecedenceTable(std::string filename);
};

#endif
