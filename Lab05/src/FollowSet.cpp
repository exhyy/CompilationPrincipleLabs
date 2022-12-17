#include "FollowSet.h"
#include <iostream>

FollowSet::FollowSet(Grammar grammar, FirstSet firstSet) : _grammar(grammar), _firstSet(firstSet)
{
    _epsilon = _grammar.epsilon();
    _end = _grammar.end();
    auto &_nonterminal = _grammar.nonterminal();
    std::set<std::string> emptySet;
    for (auto n : _nonterminal)
        _data[n] = emptySet;
    _getData();
}

void FollowSet::_getData()
{
    auto grammar = _grammar.data();
    auto &_nonterminal = _grammar.nonterminal();

    // #加入到FOLLOW(S)
    _data[_grammar.start()].insert(_end);

    // 第二步
    for (auto rule : grammar)
    {
        auto symbols = rule.right;
        for (int i = 0; i < int(symbols.size()) - 1; i++)
        {
            if (_nonterminal.find(symbols[i]) != _nonterminal.end())
            {
                std::string behind = "";
                for (int j = i + 1; j < int(symbols.size()); j++)
                    behind += symbols[j];
                auto newSet = _firstSet.string(behind);
                newSet.erase(_epsilon);
                _data[symbols[i]].merge(newSet);
            }
        }
    }

    // 第三步
    bool flag = false;
    do
    {
        flag = false;
        for (auto rule : grammar)
        {
            auto left = rule.left;
            auto symbols = rule.right;
            for (int i = 0; i < int(symbols.size()); i++)
            {
                if (_nonterminal.find(symbols[i]) != _nonterminal.end())
                {
                    if (i == int(symbols.size()) - 1)
                    {
                        auto oldSize = _data[symbols[i]].size();
                        auto newSet = _data[left];
                        _data[symbols[i]].merge(newSet);
                        if (oldSize != _data[symbols[i]].size())
                            flag = true;
                    }
                    else
                    {
                        std::string behind = "";
                        for (int j = i + 1; j < int(symbols.size()); j++)
                            behind += symbols[j];
                        auto firstBehind = _firstSet.string(behind);
                        if (firstBehind.find(_epsilon) != firstBehind.end())
                        {
                            auto oldSize = _data[symbols[i]].size();
                            auto newSet = _data[left];
                            _data[symbols[i]].merge(newSet);
                            if (oldSize != _data[symbols[i]].size())
                                flag = true;
                        }
                    }
                }
            }
        }
    } while (flag);
}

std::map<std::string, std::set<std::string>> FollowSet::data()
{
    return _data;
}

void FollowSet::print()
{
    for (auto d : _data)
    {
        std::cout << "FOLLOW(" << d.first << "): ";
        for (auto item : d.second)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}