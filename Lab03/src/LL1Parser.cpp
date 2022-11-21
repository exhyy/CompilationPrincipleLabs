#include "LL1Parser.h"
#include "utils.h"
#include <sstream>
#include <algorithm>
#include <cstdio>

LL1Parser::LL1Parser(std::string filename, ParsingTable &parsingTable, std::string start) : _parsingTable(parsingTable)
{
    _epsilon = parsingTable.epsilon();
    _start = start;
    _end = parsingTable.end();
    _infile.open(filename, std::ios::in);
    _idToSymbol.assign({_end, "void", "int", "float", "double", "if", "else", "for", "do", "while", "i", "UINT", "<", ">", "==", "<=", ">=", "<>", "&&", "||", "!", "+", "-", "*", "/", ";", ",", "(", ")", "{", "}", "++", "--", "<<", ">>", "+=", "-=", "*=", "/=", "="});
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }
}

int LL1Parser::parse(bool debug)
{
    auto parsingTable = _parsingTable.data();
    auto terminal = _parsingTable.terminal();
    auto nonterminal = _parsingTable.nonterminal();
    int cnt = 0;

    // 清空栈
    while (!_stack.empty())
        _stack.pop();

    // 将结束符、开始符以此入栈，读取输入串的第1个终结符
    _stack.push(_end);
    _stack.push(_start);
    advance();
    cnt++;

    if (debug)
    {
        fprintf(stdout, "%-20s%-8s%-12s\n", "Stack", "Symbol", "Formula");
        auto stackString = _getStackString();
        fprintf(stdout, "%-20s%-8s%-12s\n", stackString.c_str(), _symbol.c_str(), "Init");
    }

    while (true)
    {
        std::string X = _stack.top();
        _stack.pop();
        if (terminal.find(X) != terminal.end())
        {
            // X是终结符
            if (X == _symbol)
            {
                if (debug)
                {
                    auto stackString = _getStackString(X);
                    fprintf(stdout, "%-20s%-8s%-12s\n", stackString.c_str(), _symbol.c_str(), "Next Symbol");
                }
                advance();
                cnt++;
            }
            else
            {
                if (debug)
                {
                    auto stackString = _getStackString(X);
                    fprintf(stdout, "%-20s%-8s%-12s\n", stackString.c_str(), _symbol.c_str(), "ERROR");
                }
                return cnt;
            }
        }
        else if (X == _end)
        {
            if (X == _symbol)
            {
                if (debug)
                {
                    auto stackString = _getStackString(X);
                    fprintf(stdout, "%-20s%-8s%-12s\n", stackString.c_str(), _symbol.c_str(), "Finished");
                }
                return 0;
            }
            else
            {
                if (debug)
                {
                    auto stackString = _getStackString(X);
                    fprintf(stdout, "%-20s%-8s%-12s\n", stackString.c_str(), _symbol.c_str(), "ERROR");
                }
                return cnt;
            }
        }
        else
        {
            PSS key(X, _symbol);
            if (parsingTable.find(key) != parsingTable.end())
            {
                auto value = parsingTable[key];
                if (debug)
                {
                    auto stackString = _getStackString(X);
                    fprintf(stdout, "%-20s%-8s%-12s\n", stackString.c_str(), _symbol.c_str(), value.c_str());
                }
                if (value == _epsilon)
                    continue;
                auto symbols = splitSymbols(value, terminal, nonterminal, _epsilon);
                std::reverse(symbols.begin(), symbols.end());
                for (auto x : symbols)
                    _stack.push(x);
            }
            else
            {
                if (debug)
                {
                    auto stackString = _getStackString(X);
                    fprintf(stdout, "%-20s%-8s%-12s\n", stackString.c_str(), _symbol.c_str(), "ERROR");
                }
                return cnt;
            }
        }
    }
}

void LL1Parser::advance()
{
    std::string line;
    getline(_infile, line);
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

std::string LL1Parser::_getStackString(std::string poped)
{
    auto stack = _stack;
    if (poped != "")
        stack.push(poped);
    std::string result = "";
    while (!stack.empty())
    {
        auto symbol = stack.top();
        stack.pop();
        result += symbol;
    }
    std::reverse(result.begin(), result.end());
    return result;
}