#include "FirstSet.h"
#include <iostream>

FirstSet::FirstSet(Grammar grammar) : _grammar(grammar)
{
    _epsilon = _grammar.epsilon();
    _getData();
    _data[_epsilon] = std::set<std::string>({_epsilon});
}

FirstSet::FirstSet(const FirstSet &firstSet) : _grammar(firstSet._grammar)
{
    _epsilon = firstSet._epsilon;
    _data = firstSet._data;
}

void FirstSet::_getData()
{
    auto &_terminal = _grammar.terminal();
    auto &_nonterminal = _grammar.nonterminal();
    auto grammar = _grammar.data();

    // 步骤1：终结符t的FIRST集为{t}
    for (auto t : _terminal)
    {
        std::set<std::string> first;
        first.insert(t);
        _data[t] = first;
    }

    // 步骤2
    for (auto n : _nonterminal)
    {
        std::set<std::string> first;
        for (auto rule : grammar)
        {
            auto left = rule.left;
            if (left != n)
                continue;

            auto firstSymbol = rule.right[0];
            if (_terminal.find(firstSymbol) != _terminal.end())
            {
                first.insert(firstSymbol);
            }
            if (firstSymbol == _epsilon && rule.right.size() == 1)
            {
                first.insert(_epsilon);
            }
        }
        _data[n] = first;
    }

    // 步骤3
    bool flag = false;
    do
    {
        flag = false;
        for (auto rule : grammar)
        {
            auto left = rule.left;
            auto symbols = rule.right;
            if (_nonterminal.find(symbols[0]) != _nonterminal.end())
            {
                int oldSize = _data[left].size();
                auto newSet = _data[symbols[0]];
                newSet.erase(_epsilon);
                _data[left].merge(newSet);
                flag = (oldSize != int(_data[left].size()));
            }

            int maxNonterminalLength = -1;
            if (symbols.size() >= 2)
            {
                maxNonterminalLength = 0;
                for (auto symbol : symbols)
                {
                    if (_nonterminal.find(symbol) == _nonterminal.end())
                        break;
                    maxNonterminalLength++;
                }
                if (maxNonterminalLength >= 1)
                {
                    int limit;
                    if (maxNonterminalLength == int(symbols.size()))
                        limit = symbols.size() - 1;
                    else
                        limit = maxNonterminalLength;
                    for (int i = 0; i < limit; i++)
                    {
                        auto symbol1 = symbols[i];
                        auto symbol2 = symbols[i + 1];
                        if (_data[symbol1].find(_epsilon) != _data[symbol1].end())
                        {
                            int oldSize = _data[left].size();
                            auto newSet = _data[symbol2];
                            newSet.erase(_epsilon);
                            _data[left].merge(newSet);
                            flag = (oldSize != int(_data[left].size()));
                        }
                        else
                            break;
                    }
                }
            }

            if (maxNonterminalLength == -1)
            {
                if (_data[symbols[0]].find(_epsilon) != _data[symbols[0]].end())
                {
                    int oldSize = _data[left].size();
                    _data[left].insert(_epsilon);
                    flag = (oldSize != int(_data[left].size()));
                }
            }
            else
            {
                if (maxNonterminalLength == int(symbols.size()))
                {
                    bool allEpsilon = true;
                    for (auto symbol : symbols)
                    {
                        if (_data[symbol].find(_epsilon) != _data[symbol].end())
                        {
                            allEpsilon = false;
                            break;
                        }
                    }
                    if (allEpsilon)
                    {
                        int oldSize = _data[left].size();
                        _data[left].insert(_epsilon);
                        flag = (oldSize != int(_data[left].size()));
                    }
                }
            }
        }
    } while (flag);
}

std::map<std::string, std::set<std::string>> FirstSet::data()
{
    return _data;
}

std::set<std::string> FirstSet::string(std::string str)
{
    std::set<std::string> result;
    auto &_terminal = _grammar.terminal();
    auto &_nonterminal = _grammar.nonterminal();
    auto symbols = splitSymbols(str, _terminal, _nonterminal, _epsilon);
    auto first1 = _data[symbols[0]];
    first1.erase(_epsilon);
    result.merge(first1);
    for (int i = 0; i < int(symbols.size()); i++)
    {
        auto first = _data[symbols[i]];
        if (i != int(symbols.size()) - 1)
        {
            if (first.find(_epsilon) != first.end())
            {
                auto firstNext = _data[symbols[i + 1]];
                firstNext.erase(_epsilon);
                result.merge(firstNext);
            }
            else
                break;
        }
        else
        {
            if (first.find(_epsilon) != first.end())
            {
                result.insert(_epsilon);
            }
        }
    }
    return result;
}

void FirstSet::print()
{
    for (auto d : _data)
    {
        std::cout << "FIRST(" << d.first << "): ";
        for (auto item : d.second)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}
