#ifndef PARSINGTABLE_H_
#define PARSINGTABLE_H_

#include "FirstSet.h"
#include "FollowSet.h"
#include "utils.h"
#include <utility>
#include <map>
#include <fstream>

typedef std::pair<std::string, std::string> PSS;

class ParsingTable
{
private:
    std::ifstream _infile;
    std::string _epsilon;
    FirstSet &_firstSet;
    FollowSet &_followSet;
    std::map<PSS, std::string> _data;
    void _getData();

public:
    ParsingTable(std::string filename, FirstSet &firstSet, FollowSet &followSet);
    std::map<PSS, std::string> data();
};

#endif