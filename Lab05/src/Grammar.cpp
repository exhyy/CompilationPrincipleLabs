#include "Grammar.h"

Grammar::Grammar(std::string filename, std::string epsilon, std::string start, std::string end) : _filename(filename), _epsilon(epsilon), _start(start), _end(end)
{
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

Grammar::Grammar(const Grammar &that) : _filename(that._filename), _epsilon(that._epsilon), _start(that._start), _end(that._end)
{
    _infile.open(_filename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + _filename;
        throw msg;
    }
    _terminal = that._terminal;
    _nonterminal = that._nonterminal;
    _data = that._data;
}

Grammar::~Grammar()
{
    _infile.close();
}

void Grammar::_getNonterminal()
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

void Grammar::_getTerminal()
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
        if (_epsilon != "")
        {
            index = right.find(_epsilon);
            while (index != -1)
            {
                right.erase(index, _epsilon.length());
                index = right.find(_epsilon);
            }
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

void Grammar::_getData()
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
            auto symbols = splitSymbols(singleRight, _terminal, _nonterminal, _epsilon);
            Rule rule;
            rule.left = left;
            rule.right = symbols;
            _data.push_back(rule);
        }
    }
}

const std::set<std::string, SymbolCmp> &Grammar::nonterminal()
{
    return _nonterminal;
}

const std::set<std::string, SymbolCmp> &Grammar::terminal()
{
    return _terminal;
}

const std::vector<Rule> &Grammar::data()
{
    return _data;
}

std::string Grammar::epsilon()
{
    return _epsilon;
}

std::string Grammar::start()
{
    return _start;
}

std::string Grammar::end()
{
    return _end;
}

std::vector<Rule> Grammar::collectByLeft(std::string left)
{
    std::vector<Rule> result;
    for (auto rule : _data)
    {
        if (rule.left == left)
            result.push_back(rule);
    }
    return result;
}
