#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include <fstream>
#include <string>
#include <set>
#include <utility>
#include "utils.h"
#include "Rule.h"

class Grammar
{
private:
    std::string _filename;
    std::string _epsilon;
    std::string _start;
    std::string _end;
    std::ifstream _infile;
    std::set<std::string, SymbolCmp> _terminal;
    std::set<std::string, SymbolCmp> _nonterminal;
    std::vector<Rule> _data;
    void _getNonterminal();
    void _getTerminal();
    void _getData();

public:
    Grammar(std::string filename, std::string epsilon, std::string start, std::string end);
    Grammar(std::string filename, std::string epsilon, std::string end); // 选择第一个非终结符作为开始符号
    Grammar(const Grammar &that);
    ~Grammar();
    const std::set<std::string, SymbolCmp> &nonterminal();
    const std::set<std::string, SymbolCmp> &terminal();
    const std::vector<Rule> &data();
    std::string epsilon();
    std::string start();
    std::string end();
    std::vector<Rule> collectByLeft(std::string left); // 返回所有左部为left的规则
};

#endif