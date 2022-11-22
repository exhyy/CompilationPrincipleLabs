#include "OPGParser.h"
#include "utils.h"
#include <sstream>

OPGParser::OPGParser(std::string inputFilename, std::string grammarFilename, PrecedenceTable precedenceTable, std::string end) : _precedenceTable(precedenceTable)
{
    _end = end;
    _grammarFilename = grammarFilename;
    _input.open(inputFilename);
    if (!_input.is_open())
    {
        std::string msg = "无法打开" + inputFilename;
        throw msg;
    }
    _idToSymbol.assign({_end, "void", "int", "float", "double", "if", "else", "for", "do", "while", "i", "UINT", "<", ">", "==", "<=", ">=", "<>", "&&", "||", "!", "+", "-", "*", "/", ";", ",", "(", ")", "{", "}", "++", "--", "<<", ">>", "+=", "-=", "*=", "/=", "="});
    _getReduction();
}

int OPGParser::parse()
{
    auto terminal = _precedenceTable.terminal();
    auto nonterminal = _precedenceTable.nonterminal();
    auto precedenceTable = _precedenceTable.data();
    _input.clear();
    _input.seekg(0, std::ios::beg);
    while (!_stack.empty())
        _stack.pop_back();

    int i = 0, j;
    _stack.push_back(_end);
    advance();

    while (true)
    {
        if (_stack[i] == _end || terminal.find(_stack[i]) != terminal.end())
            j = i;
        else
            j = i - 1;

        PSS key(_stack[j], _symbol);
        if (precedenceTable.find(key) != precedenceTable.end())
        {
            if (precedenceTable[key] == PRECEDENCE_GT)
            {
                do
                {
                    // 查找最左素短语
                    std::string Q = _stack[j];
                    j--;
                    if (_stack[j] != _end && terminal.find(_stack[j]) == terminal.end())
                        j--;
                    key = PSS(_stack[j], Q);
                    if (precedenceTable.find(key) == precedenceTable.end())
                        return 1;
                } while (precedenceTable[key] != PRECEDENCE_LT);

                // 归约
                std::string leftmost = "";
                for (int k = j + 1; k <= i; k++)
                    leftmost += _stack[k];
                i = j + 1;
                // TODO：先判断_reduction[leftmost]是否存在，再弹出
                while (_stack.size() > i)
                    _stack.pop_back();
                // _stack[i] = _reduction[leftmost];
                _stack.push_back(_reduction[leftmost]);

                if (i == 1 && _symbol == _end)
                    return 0;
            }
            else
            {
                // 移进
                i++;
                _stack.push_back(_symbol);
                // if (i >= int(_stack.size()))
                //     _stack.push_back(_symbol);
                // else
                //     _stack[i] = _symbol;
                advance();
            }
        }
        else
        {
            return 1;
        }
    }
}

void OPGParser::advance()
{
    std::string line;
    getline(_input, line);
    if (line[0] == '(')
    {
        int index1 = line.find(',');
        std::string wordId = line.substr(1, index1 - 1);
        std::stringstream stream;
        stream << wordId;
        stream >> this->_wordId;

        int index2 = line.find(')');
        this->_wordValue = line.substr(index1 + 2, index2 - index1 - 2);
    }
    else
    {
        this->_wordId = EOI;
    }
    _symbol = _idToSymbol[_wordId];
}

void OPGParser::_getReduction()
{
    auto terminal = _precedenceTable.terminal();
    auto nonterminal = _precedenceTable.nonterminal();
    std::ifstream grammar;
    grammar.open(_grammarFilename);
    if (!grammar.is_open())
    {
        std::string msg = "无法打开" + _grammarFilename;
        throw msg;
    }

    std::string line;
    while (!grammar.eof())
    {
        getline(grammar, line);
        int index = line.find("->");
        if (index == -1)
            continue;
        std::string left = line.substr(0, index);
        std::string right = line.substr(index + 2);
        auto splitedRight = splitString(right, "|");
        for (auto singleRight : splitedRight)
        {
            // OPG里的非终结符无意义，因此全部换成“N”
            auto symbols = splitSymbols(singleRight, terminal, nonterminal, "");
            std::string key = "";
            for (int i = 0; i < symbols.size(); i++)
            {
                if (nonterminal.find(symbols[i]) != nonterminal.end())
                    symbols[i] = "N";
                key += symbols[i];
            }
            _reduction[key] = "N";
        }
    }
}
