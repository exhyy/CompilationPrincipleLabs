#include "PrecedenceTable.h"

PrecedenceTable::PrecedenceTable(std::string filename)
{
    _infile.open(filename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }

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

