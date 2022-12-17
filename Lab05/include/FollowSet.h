#ifndef FOLLOWSET_H_
#define FOLLOWSET_H_

#include "FirstSet.h"
#include "Grammar.h"
#include "utils.h"
#include <fstream>
#include <map>
#include <string>

class FollowSet
{
private:
    std::string _epsilon;
    std::string _end;
    Grammar _grammar;
    FirstSet _firstSet;
    std::map<std::string, std::set<std::string>> _data;
    void _getData();

public:
    FollowSet(Grammar grammar, FirstSet firstSet);
    std::map<std::string, std::set<std::string>> data();
    void print();
};

#endif
