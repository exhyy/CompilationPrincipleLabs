#ifndef FIRSTSET_H_
#define FIRSTSET_H_

#include "utils.h"
#include "Grammar.h"
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <vector>

class FirstSet
{
private:
    Grammar _grammar;
    std::string _epsilon;
    std::map<std::string, std::set<std::string>> _data;
    void _getData();

public:
    FirstSet(Grammar grammar);
    FirstSet(const FirstSet &firstSet);
    std::map<std::string, std::set<std::string>> data();
    std::set<std::string> string(std::string str);
    void print();
};

#endif
