#include "PrecedenceTable.h"
#include <stack>

PrecedenceTable::PrecedenceTable(std::string filename)
{
    _infile.open(filename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }
    _getNonterminal();
    _getTerminal();
    std::set<std::string> emptySet;
    for (auto n : _nonterminal)
    {
        _firstvt[n] = emptySet;
        _lastvt[n] = emptySet;
    }
    _getFirstvt();
    _getLastvt();
    _getData();
}

void PrecedenceTable::_getNonterminal()
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

void PrecedenceTable::_getTerminal()
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
        std::vector<std::string> subStrings = splitString(right, "|");
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

void PrecedenceTable::_getFirstvt()
{
    std::stack<PSS> stack;

    // 初始
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
        auto splitedRight = splitString(right, "|");
        for (auto singleRight : splitedRight)
        {
            auto symbols = splitSymbols(singleRight, _terminal, _nonterminal, "");
            if (_terminal.find(symbols[0]) != _terminal.end())
            {
                _firstvt[left].insert(symbols[0]);
                PSS item(left, symbols[0]);
                stack.push(item);
            }
            else if (symbols.size() >= 2 && _terminal.find(symbols[1]) != _terminal.end())
            {
                _firstvt[left].insert(symbols[1]);
                PSS item(left, symbols[1]);
                stack.push(item);
            }
        }
    }

    // 传递
    while (!stack.empty())
    {
        auto top = stack.top();
        stack.pop();
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
            auto splitedRight = splitString(right, "|");
            for (auto singleRight : splitedRight)
            {
                auto symbols = splitSymbols(singleRight, _terminal, _nonterminal, "");
                if (symbols[0] != top.first)
                    continue;
                if (_firstvt[left].find(top.second) == _firstvt[left].end())
                {
                    _firstvt[left].insert(top.second);
                    PSS item(left, top.second);
                    stack.push(item);
                }
            }
        }
    }
}

void PrecedenceTable::_getLastvt()
{
    std::stack<PSS> stack;

    // 初始
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
        auto splitedRight = splitString(right, "|");
        for (auto singleRight : splitedRight)
        {
            auto symbols = splitSymbols(singleRight, _terminal, _nonterminal, "");
            if (_terminal.find(symbols[symbols.size() - 1]) != _terminal.end())
            {
                _lastvt[left].insert(symbols[symbols.size() - 1]);
                PSS item(left, symbols[symbols.size() - 1]);
                stack.push(item);
            }
            else if (symbols.size() >= 2 && _terminal.find(symbols[symbols.size() - 2]) != _terminal.end())
            {
                _lastvt[left].insert(symbols[symbols.size() - 2]);
                PSS item(left, symbols[symbols.size() - 2]);
                stack.push(item);
            }
        }
    }

    // 传递
    while (!stack.empty())
    {
        auto top = stack.top();
        stack.pop();
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
            auto splitedRight = splitString(right, "|");
            for (auto singleRight : splitedRight)
            {
                auto symbols = splitSymbols(singleRight, _terminal, _nonterminal, "");
                if (symbols[symbols.size() - 1] != top.first)
                    continue;
                if (_lastvt[left].find(top.second) == _lastvt[left].end())
                {
                    _lastvt[left].insert(top.second);
                    PSS item(left, top.second);
                    stack.push(item);
                }
            }
        }
    }
}

void PrecedenceTable::_getData()
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
        auto splitedRight = splitString(right, "|");
        for (auto singleRight : splitedRight)
        {
            auto symbols = splitSymbols(singleRight, _terminal, _nonterminal, "");
            for (int i = 0; i < int(symbols.size()) - 1; i++)
            {
                // ...ab...
                if (_terminal.find(symbols[i]) != _terminal.end() && _terminal.find(symbols[i + 1]) != _terminal.end())
                {
                    PSS key(symbols[i], symbols[i + 1]);
                    _data[key] = PRECEDENCE_EQ;
                }

                // ...aVb...
                if (i < int(symbols.size()) - 2 && _terminal.find(symbols[i]) != _terminal.end() && _terminal.find(symbols[i + 2]) != _terminal.end() && _nonterminal.find(symbols[i + 1]) != _nonterminal.end())
                {
                    PSS key(symbols[i], symbols[i + 2]);
                    _data[key] = PRECEDENCE_EQ;
                }

                // ...aU...
                if (_terminal.find(symbols[i]) != _terminal.end() && _nonterminal.find(symbols[i + 1]) != _nonterminal.end())
                {
                    for (auto b : _firstvt[symbols[i + 1]])
                    {
                        PSS key(symbols[i], b);
                        _data[key] = PRECEDENCE_LT;
                    }
                }

                // ...Ub...
                if (_nonterminal.find(symbols[i]) != _nonterminal.end() && _terminal.find(symbols[i + 1]) != _terminal.end())
                {
                    for (auto a : _lastvt[symbols[i]])
                    {
                        PSS key(a, symbols[i + 1]);
                        _data[key] = PRECEDENCE_GT;
                    }
                }
            }
        }
    }
}

std::set<std::string, SymbolCmp> PrecedenceTable::terminal()
{
    return _terminal;
}

std::set<std::string, SymbolCmp> PrecedenceTable::nonterminal()
{
    return _nonterminal;
}

std::map<std::string, std::set<std::string>> PrecedenceTable::firstvt()
{
    return _firstvt;
}

std::map<std::string, std::set<std::string>> PrecedenceTable::lastvt()
{
    return _lastvt;
}

std::map<PSS, int> PrecedenceTable::data()
{
    return _data;
}
