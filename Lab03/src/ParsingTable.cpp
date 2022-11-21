#include "ParsingTable.h"
#include <cstdio>

ParsingTable::ParsingTable(std::string filename, FirstSet &firstSet, FollowSet &followSet) : _firstSet(firstSet), _followSet(followSet)
{
    _epsilon = firstSet.epsilon();
    _infile.open(filename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }
    _getData();
}

void ParsingTable::_getData()
{
    _infile.clear();
    _infile.seekg(0, std::ios::beg);
    std::string line;
    while (!_infile.eof())
    {
        getline(_infile, line);
        int index = line.find("->");
        if (index == -1)
            continue;
        std::string left = line.substr(0, index);
        std::string right = line.substr(index + 2);
        std::vector<std::string> splitedRight = splitString(right, "|");
        for (auto alpha : splitedRight)
        {
            auto firstAlpha = _firstSet.string(alpha);
            if (firstAlpha.find(_epsilon) == firstAlpha.end())
            {
                for (auto a : firstAlpha)
                {
                    PSS key(left, a);
                    _data[key] = alpha;
                }
            }
            else
            {
                auto leftFollow = _followSet.data()[left];
                for (auto b : leftFollow)
                {
                    PSS key(left, b);
                    _data[key] = _epsilon;
                }
            }
        }
    }
}

std::map<PSS, std::string> ParsingTable::data()
{
    return _data;
}

std::string ParsingTable::epsilon()
{
    return _epsilon;
}

std::string ParsingTable::end()
{
    return _followSet.end();
}

std::set<std::string, SymbolCmp> ParsingTable::terminal()
{
    return _firstSet.terminal();
}

std::set<std::string, SymbolCmp> ParsingTable::nonterminal()
{
    return _firstSet.nonterminal();
}

void ParsingTable::print()
{
    auto terminal = _firstSet.terminal();
    auto nonterminal = _firstSet.nonterminal();
    fprintf(stdout, "%-10s", "");
    for (auto t : terminal)
        fprintf(stdout, "%-10s", t.c_str());
    fprintf(stdout, "\n");

    for (auto n : nonterminal)
    {
        fprintf(stdout, "%-10s", n.c_str());
        for (auto t : terminal)
        {
            PSS key(n, t);
            if (_data.find(key) == _data.end())
                fprintf(stdout, "%-10s", "ERROR");
            else
                fprintf(stdout, "%-10s", _data[key].c_str());
        }
        fprintf(stdout, "\n");
    }
}
