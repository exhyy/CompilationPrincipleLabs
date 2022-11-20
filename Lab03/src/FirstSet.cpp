#include "FirstSet.h"
#include <iostream>

std::vector<std::string> FirstSet::_splitString(std::string str, std::string separator)
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

std::vector<std::string> FirstSet::_splitSymbols(std::string str)
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

FirstSet::FirstSet(std::string filename, std::string epsilon)
{
    _epsilon = epsilon;
    _infile.open(filename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }
    _getNonterminal();
    _getTerminal();
    _getData();
}

void FirstSet::_getNonterminal()
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
        std::string nonterminal = line.substr(0, index);
        _nonterminal.insert(nonterminal);
    }
}

std::set<std::string, SymbolCmp> FirstSet::nonterminal()
{
    return _nonterminal;
}

void FirstSet::_getTerminal()
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
        std::string right = line.substr(index + 2);

        // 删除epsilon
        index = right.find(_epsilon);
        while (index != -1)
        {
            right.erase(index, _epsilon.length());
            index = right.find(_epsilon);
        }

        // 删除非终结符
        for (auto n : _nonterminal)
        {
            index = right.find(n);
            while (index != -1)
            {
                right.erase(index, n.length());
                index = right.find(n);
            }
        }
        std::vector<std::string> subStrings = _splitString(right, "|");
        for (auto str : subStrings)
        {
            for (int i = 0; i < int(str.length()); i++)
            {
                std::string terminal = "";
                terminal += str[i];
                _terminal.insert(terminal);
            }
        }
    }
}

std::set<std::string, SymbolCmp> FirstSet::terminal()
{
    return _terminal;
}

void FirstSet::_getData()
{
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
            if (left != n)
                continue;
            std::string right = line.substr(index + 2);
            std::vector<std::string> subStrings = _splitString(right, "|");
            for (auto str : subStrings)
            {
                std::string firstSymbol = "";
                firstSymbol += str[0];
                if (_terminal.find(firstSymbol) != _terminal.end())
                {
                    first.insert(firstSymbol);
                }
                index = line.find(_epsilon);
                if (index != -1)
                {
                    first.insert(_epsilon);
                }
            }
        }
        _data[n] = first;
    }

    // 步骤3
    // TODO: 设置一个flag检测FIRST(x)是否发生变化
    bool flag = false;
    do
    {
        flag = false;
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
            std::vector<std::string> splitedRight = _splitString(right, "|");
            for (auto singleRight : splitedRight)
            {
                auto symbols = _splitSymbols(singleRight);
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
                        // TODO: 第四章(2)PPT第29页的ii
                        // 遍历[1, maxNonterminalLength-1]，下标为i
                        // 若epsilon属于FIRST(Y_i)，则将FIRST(Y_(i+1))-{epsilon}加入到FIRST(x)中
                        // 若epsilon不属于FIRST(Y_i)，则直接结束循环
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
        }
    } while (flag);
}

std::map<std::string, std::set<std::string>> FirstSet::data()
{
    return _data;
}