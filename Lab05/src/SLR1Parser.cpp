#include "SLR1Parser.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

template <class T>
std::stack<T> reverseStack(std::stack<T> stack)
{
    std::stack<T> result;
    while (!stack.empty())
    {
        result.push(stack.top());
        stack.pop();
    }
    return result;
}

SLR1Parser::SLR1Parser(Grammar grammar, ItemSets itemSets) : _grammar(grammar)
{
    _ACTION = itemSets.ACTION();
    _GOTO = itemSets.GOTO();
    _idToSymbol.assign({_grammar.end(), "void", "int", "float", "double", "if", "else", "for", "do", "while", "i", "UINT", "<", ">", "==", "<=", ">=", "<>", "&&", "||", "!", "+", "-", "*", "/", ";", ",", "(", ")", "{", "}", "++", "--", "<<", ">>", "+=", "-=", "*=", "/=", "="});
}

int SLR1Parser::parse(std::string filename, bool debug)
{
    _errorMessage = "";

    // 打开文件
    _infile.open(filename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }

    // 清空栈
    while (!_statusStack.empty())
        _statusStack.pop();
    while (!_symbolStack.empty())
        _symbolStack.pop();

    // 初始：状态0和结束符号进栈
    _statusStack.push(0);
    _symbolStack.push(_grammar.end());
    _advance();
    int cnt = 1;
    int statusLength = 32, symbolLength = 32;
    if (debug)
    {
        std::cout << std::setw(statusLength) << "Status Stack"
                  << "|" << std::setw(symbolLength) << "Symbol Stack"
                  << "|" << std::setw(8) << "Symbol"
                  << "|" << std::setw(8) << "ACTION"
                  << "|" << std::setw(8) << "GOTO" << std::endl;
        std::string line(statusLength + symbolLength + 28, '-');
        std::cout << line << std::endl;
    }

    auto grammar = _grammar.data();
    PIS key;
    PII actionValue;
    int gotoValue;
    while (true)
    {
        key = PIS(_statusStack.top(), _symbol);
        if (_ACTION.find(key) != _ACTION.end())
        {
            actionValue = _ACTION[key];
            if (actionValue.first == ACTION_SHIFT)
            {
                if (debug)
                    _printLine(statusLength, _statusStack, symbolLength, _symbolStack, 8, 8, actionValue, 8, -1);
                _statusStack.push(actionValue.second);
                _symbolStack.push(_symbol);
                _advance();
                cnt++;
            }
            else if (actionValue.first == ACTION_REDUCE)
            {
                // 归约
                auto tempStatusStack = _statusStack;
                auto tempSymbolStack = _symbolStack;
                auto reductionRule = grammar[actionValue.second];
                for (int i = 0; i < int(reductionRule.right.size()); i++)
                {
                    // 对偶栈元素的个数要保持一致
                    _symbolStack.pop();
                    _statusStack.pop();
                }
                _symbolStack.push(reductionRule.left);

                // GOTO
                key = PIS(_statusStack.top(), _symbolStack.top());
                if (_GOTO.find(key) != _GOTO.end())
                {
                    gotoValue = _GOTO[key];
                    if (debug)
                        _printLine(statusLength, tempStatusStack, symbolLength, tempSymbolStack, 8, 8, actionValue, 8, gotoValue);
                    _statusStack.push(gotoValue);
                }
                else
                {
                    if (debug)
                        _printLine(statusLength, tempStatusStack, symbolLength, tempSymbolStack, 8, 8, actionValue, 8, -1);
                    _errorMessage = "找不到GOTO目标";
                    break;
                }
            }
            else if (actionValue.first == ACTION_ACCEPT)
            {
                if (debug)
                    _printLine(statusLength, _statusStack, symbolLength, _symbolStack, 8, 8, actionValue, 8, -1);
                cnt = 0;
                break;
            }
            else
            {
                _errorMessage = "ACTION信息有误";
                break;
            }
        }
        else
        {
            if (debug)
            {
                PII tempActionValue(-1, -1);
                _printLine(statusLength, _statusStack, symbolLength, _symbolStack, 8, 8, tempActionValue, 8, -1);
            }
            _errorMessage = "找不到ACTION目标";
            break;
        }
    }
    _infile.close();
    return cnt;
}

void SLR1Parser::_advance()
{
    std::string line;
    getline(_infile, line);
    if (line[0] == '(')
    {
        int index1 = line.find(',');
        std::string wordId = line.substr(1, index1 - 1);
        _wordId = std::stoi(wordId);

        int index2 = line.find(')');
        _wordValue = line.substr(index1 + 2, index2 - index1 - 2);
    }
    else
    {
        _wordId = EOI;
    }
    _symbol = _idToSymbol[_wordId];
}

std::string SLR1Parser::_getStatusString(const std::stack<int> &stack)
{
    auto reversedStack = reverseStack(stack);
    std::string result = "";
    while (!reversedStack.empty())
    {
        std::string status = std::to_string(reversedStack.top());
        if (status.length() > 1)
            status = "(" + status + ")";
        result += status;
        reversedStack.pop();
    }
    return result;
}

std::string SLR1Parser::_getSymbolString(const std::stack<std::string> &stack)
{
    auto reversedStack = reverseStack(stack);
    std::string result = "";
    while (!reversedStack.empty())
    {
        std::string symbol = reversedStack.top();
        result += symbol;
        reversedStack.pop();
    }
    return result;
}

void SLR1Parser::_printLine(int statusStackLength, const std::stack<int> &statusStack, int symbolStackLength, const std::stack<std::string> &symbolStack, int symbolLength, int actionLength, PII actionValue, int gotoLength, int gotoValue)
{
    auto statusString = _getStatusString(statusStack);
    auto symbolString = _getSymbolString(symbolStack);
    std::string actionString = "";
    std::string gotoString = "";
    if (actionValue.first == ACTION_SHIFT)
        actionString = "S_" + std::to_string(actionValue.second);
    else if (actionValue.first == ACTION_REDUCE)
        actionString = "r_" + std::to_string(actionValue.second);
    else if (actionValue.first == ACTION_ACCEPT)
        actionString = "acc";
    if (gotoValue >= 0)
        gotoString = std::to_string(gotoValue);
    std::cout << std::setw(statusStackLength) << statusString << "|" << std::setw(symbolStackLength) << symbolString << "|" << std::setw(symbolLength) << _symbol << "|" << std::setw(actionLength) << actionString << "|" << std::setw(gotoLength) << gotoString << std::endl;
}

std::string SLR1Parser::errorMessage()
{
    return _errorMessage;
}
