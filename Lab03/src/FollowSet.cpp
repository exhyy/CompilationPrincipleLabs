#include "FollowSet.h"
#include <iostream>

std::vector<std::string> FollowSet::_splitString(std::string str, std::string separator)
{
    std::vector<std::string> result;
    int index = str.find(separator);
    while (index != -1)
    {
        result.push_back(str.substr(0, index));
        str.erase(0, index + 1);
        index = str.find(separator);
    }
    result.push_back(str);
    return result;
}

std::vector<std::string> FollowSet::_splitSymbols(std::string str)
{
    std::vector<std::string> result;
    while (str.length() > 0)
    {
        std::string symbol = "";
        for (auto t : _terminal)
        {
            if (str.find(t) == 0 && t.length() > symbol.length())
            {
                symbol = t;
            }
        }
        if (symbol == "")
        {
            for (auto n : _nonterminal)
            {
                if (str.find(n) == 0 && n.length() > symbol.length())
                {
                    symbol = n;
                }
            }
        }
        if (symbol == "")
        {
            if (str.find(_epsilon) == 0)
            {
                symbol = _epsilon;
            }
        }
        if (symbol == "")
        {
            std::cerr << "错误：字符串并非全由符号组成" << std::endl;
            std::vector<std::string> emptyVector;
            return emptyVector;
        }
        result.push_back(symbol);
        str.erase(0, symbol.length());
    }
    return result;
}

FollowSet::FollowSet(std::string filename, FirstSet firstSet, std::string end) : _firstSet(firstSet)
{
    _epsilon = firstSet.epsilon();
    _end = end;
    _infile.open(filename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }
    _terminal = firstSet.terminal();
    _nonterminal = firstSet.nonterminal();
    std::set<std::string> emptySet;
    for (auto n : _nonterminal)
        _data[n] = emptySet;
    _getData();
}

void FollowSet::_getData()
{
    // #加入到FOLLOW(S)
    _infile.clear();
    _infile.seekg(0, std::ios::beg);
    std::string line;
    getline(_infile, line);
    int index = line.find("->");
    std::string left = line.substr(0, index);
    std::string right;
    _data[left].insert(_end);

    bool flag = false;
    do
    {
        flag = false;
        _infile.clear();
        _infile.seekg(0, std::ios::beg);
        while (!_infile.eof())
        {
            getline(_infile, line);
            index = line.find("->");
            if (index == -1)
                continue;
            left = line.substr(0, index);
            right = line.substr(index + 2);
            auto splitedRight = _splitString(right, "|");
            for (auto singleRight : splitedRight)
            {
                auto symbols = _splitSymbols(singleRight);
                for (int i = 0; i < int(symbols.size()) - 1; i++)
                {
                    if (_nonterminal.find(symbols[i]) != _nonterminal.end())
                    {
                        auto oldSize = _data[symbols[i]].size();
                        std::string behind = "";
                        for (int j = i + 1; j < int(symbols.size()); j++)
                            behind += symbols[j];
                        auto newSet = _firstSet.string(behind);
                        newSet.erase(_epsilon);
                        _data[symbols[i]].merge(newSet);
                        if (oldSize != _data[symbols[i]].size())
                            flag = true;
                    }
                }
            }
        }

        _infile.clear();
        _infile.seekg(0, std::ios::beg);
        while (!_infile.eof())
        {
            getline(_infile, line);
            index = line.find("->");
            if (index == -1)
                continue;
            left = line.substr(0, index);
            right = line.substr(index + 2);
            auto splitedRight = _splitString(right, "|");
            for (auto singleRight : splitedRight)
            {
                auto symbols = _splitSymbols(singleRight);
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
        }
    } while (flag);
}

std::map<std::string, std::set<std::string>> FollowSet::data()
{
    return _data;
}