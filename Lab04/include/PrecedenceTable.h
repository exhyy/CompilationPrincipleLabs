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
    std::map<std::string, std::set<std::string>> _firstvt;
    std::map<std::string, std::set<std::string>> _lastvt;
    std::map<PSS, int> _data;
    void _getNonterminal();
    void _getTerminal();
    void _getFirstvt();
    void _getLastvt();
    void _getData();

public:
    PrecedenceTable(std::string filename);
    std::set<std::string, SymbolCmp> terminal();
    std::set<std::string, SymbolCmp> nonterminal();
    std::map<std::string, std::set<std::string>> firstvt();
    std::map<std::string, std::set<std::string>> lastvt();
    std::map<PSS, int> data();
};

#endif
