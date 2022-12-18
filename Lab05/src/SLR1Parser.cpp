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

int SLR1Parser::parse(std::string inputFilename, std::string outputFilename, bool debug)
{
    _errorMessage = "";

    // 打开文件
    _infile.open(inputFilename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + inputFilename;
        throw msg;
    }
    _outfile.open(outputFilename, std::ios::out);
    if (!_outfile.is_open())
    {
        std::string msg = "无法打开" + outputFilename;
        throw msg;
    }

    // 清空栈
    while (!_statusStack.empty())
        _statusStack.pop();
    while (!_symbolStack.empty())
        _symbolStack.pop();

    // 初始：状态0和结束符号进栈
    _statusStack.push(0);
    _symbolStack.push({_grammar.end(), ""});
    _advance();
    int cnt = 1;
    int statusLength = 32, symbolLength = 32, codeLength = 16;
    if (debug)
    {
        std::cout << std::setw(statusLength) << "Status Stack"
                  << "|" << std::setw(symbolLength) << "Symbol Stack"
                  << "|" << std::setw(8) << "Input"
                  << "|" << std::setw(8) << "ACTION"
                  << "|" << std::setw(8) << "GOTO"
                  << "|" << std::setw(codeLength) << "Code" << std::endl;
        std::string line(statusLength + symbolLength + codeLength + 29, '-');
        std::cout << line << std::endl;
    }

    auto grammar = _grammar.data();
    PIS key;
    PII actionValue;
    int gotoValue;
    while (true)
    {
        key = PIS(_statusStack.top(), _inputSymbol);
        if (_ACTION.find(key) != _ACTION.end())
        {
            actionValue = _ACTION[key];
            if (actionValue.first == ACTION_SHIFT)
            {
                if (debug)
                    _printLine(statusLength, _statusStack, symbolLength, _symbolStack, 8, 8, actionValue, 8, -1, codeLength);
                _statusStack.push(actionValue.second);
                _symbolStack.push({_inputSymbol, _wordValue});
                _advance();
                cnt++;
            }
            else if (actionValue.first == ACTION_REDUCE)
            {
                // 归约
                auto tempStatusStack = _statusStack;
                auto tempSymbolStack = _symbolStack;
                auto reductionRule = grammar[actionValue.second];
                int reductionIndex = _doSemanticAction(actionValue.second);
                for (int i = 0; i < int(reductionRule.right.size()); i++)
                {
                    // 对偶栈元素的个数要保持一致
                    _symbolStack.pop();
                    _statusStack.pop();
                }
                _symbolStack.push({reductionRule.left, std::to_string(reductionIndex)});

                // GOTO
                key = PIS(_statusStack.top(), _symbolStack.top().first);
                if (_GOTO.find(key) != _GOTO.end())
                {
                    gotoValue = _GOTO[key];
                    if (debug)
                        _printLine(statusLength, tempStatusStack, symbolLength, tempSymbolStack, 8, 8, actionValue, 8, gotoValue, codeLength);
                    _statusStack.push(gotoValue);
                }
                else
                {
                    if (debug)
                        _printLine(statusLength, tempStatusStack, symbolLength, tempSymbolStack, 8, 8, actionValue, 8, -1, codeLength);
                    _errorMessage = "找不到GOTO目标";
                    break;
                }
            }
            else if (actionValue.first == ACTION_ACCEPT)
            {
                if (debug)
                    _printLine(statusLength, _statusStack, symbolLength, _symbolStack, 8, 8, actionValue, 8, -1, codeLength);
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
                _printLine(statusLength, _statusStack, symbolLength, _symbolStack, 8, 8, tempActionValue, 8, -1, codeLength);
            }
            _errorMessage = "找不到ACTION目标";
            break;
        }
    }

    _infile.close();
    _outfile.close();
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
        _wordValue = "";
    }
    _inputSymbol = _idToSymbol[_wordId];
}

int SLR1Parser::_doSemanticAction(int ruleId)
{
    if (ruleId == 0)
    {
        // S->V=E
        std::vector<PSS> poped;
        for (int i = 0; i < 3; i++)
        {
            poped.push_back(_symbolStack.top());
            _symbolStack.pop();
        }
        _generateCode("=", std::stoi(poped[0].second), -1, std::stoi(poped[2].second));
        for (int i = 2; i >= 0; i--)
            _symbolStack.push(poped[i]); // 还原
        return -1;
    }
    else if (ruleId == 1)
    {
        // E->E+T
        int tempNo = _newTemp();
        std::string tempName = "0TEMP_" + std::to_string(tempNo);
        int tempIndex = _symbolTable.getIndex(tempName);
        std::vector<PSS> poped;
        for (int i = 0; i < 3; i++)
        {
            poped.push_back(_symbolStack.top());
            _symbolStack.pop();
        }
        _generateCode("+", std::stoi(poped[2].second), std::stoi(poped[0].second), tempIndex);
        for (int i = 2; i >= 0; i--)
            _symbolStack.push(poped[i]); // 还原
        return tempIndex;
    }
    else if (ruleId == 2)
    {
        // E->E-T
        int tempNo = _newTemp();
        std::string tempName = "0TEMP_" + std::to_string(tempNo);
        int tempIndex = _symbolTable.getIndex(tempName);
        std::vector<PSS> poped;
        for (int i = 0; i < 3; i++)
        {
            poped.push_back(_symbolStack.top());
            _symbolStack.pop();
        }
        _generateCode("-", std::stoi(poped[2].second), std::stoi(poped[0].second), tempIndex);
        for (int i = 2; i >= 0; i--)
            _symbolStack.push(poped[i]); // 还原
        return tempIndex;
    }
    else if (ruleId == 3)
    {
        // E->T
        // 仅转移属性
        return std::stoi(_symbolStack.top().second);
    }
    else if (ruleId == 4)
    {
        // T->T*F
        int tempNo = _newTemp();
        std::string tempName = "0TEMP_" + std::to_string(tempNo);
        int tempIndex = _symbolTable.getIndex(tempName);
        std::vector<PSS> poped;
        for (int i = 0; i < 3; i++)
        {
            poped.push_back(_symbolStack.top());
            _symbolStack.pop();
        }
        _generateCode("*", std::stoi(poped[2].second), std::stoi(poped[0].second), tempIndex);
        for (int i = 2; i >= 0; i--)
            _symbolStack.push(poped[i]); // 还原
        return tempIndex;
    }
    else if (ruleId == 5)
    {
        // T->T/F
        int tempNo = _newTemp();
        std::string tempName = "0TEMP_" + std::to_string(tempNo);
        int tempIndex = _symbolTable.getIndex(tempName);
        std::vector<PSS> poped;
        for (int i = 0; i < 3; i++)
        {
            poped.push_back(_symbolStack.top());
            _symbolStack.pop();
        }
        _generateCode("/", std::stoi(poped[2].second), std::stoi(poped[0].second), tempIndex);
        for (int i = 2; i >= 0; i--)
            _symbolStack.push(poped[i]); // 还原
        return tempIndex;
    }
    else if (ruleId == 6)
    {
        // T->F
        // 仅转移属性
        return std::stoi(_symbolStack.top().second);
    }
    else if (ruleId == 7)
    {
        // F->(E)
        // 仅转移属性
        auto poped = _symbolStack.top();
        _symbolStack.pop();
        int indexE = std::stoi(_symbolStack.top().second);
        _symbolStack.push(poped); // 还原
        return indexE;
    }
    else if (ruleId == 8)
    {
        // F->i
        // 检查符号表中是否有变量i，若没有则加入符号表
        // 然后将变量i在符号表中的下标填入F的属性值
        _symbolTable.add(_symbolStack.top().second);
        return _symbolTable.getIndex(_symbolStack.top().second);
    }
    else if (ruleId == 9)
    {
        // V->i
        // 检查符号表中是否有变量i，若没有则加入符号表
        // 然后将变量i在符号表中的下标填入V的属性值
        _symbolTable.add(_symbolStack.top().second);
        return _symbolTable.getIndex(_symbolStack.top().second);
    }
}

void SLR1Parser::_generateCode(std::string op, int arg1, int arg2, int target)
{
    std::string result;
    std::string name1 = "", name2 = "", nameTarget = "";
    if (arg1 >= 0)
    {
        name1 = _symbolTable[arg1];
        if (name1[0] == '0')
        {
            // 临时变量
            name1 = "T_" + name1.substr(6);
        }
    }
    if (arg2 >= 0)
    {
        name2 = _symbolTable[arg2];
        if (name2[0] == '0')
        {
            // 临时变量
            name2 = "T_" + name2.substr(6);
        }
    }
    if (target >= 0)
    {
        nameTarget = _symbolTable[target];
        if (nameTarget[0] == '0')
        {
            // 临时变量
            nameTarget = "T_" + nameTarget.substr(6);
        }
    }

    result = "(" + op + ", " + name1 + ", " + name2 + ", " + nameTarget + ")";
    _outfile << result << std::endl;
    _code = result;
}

int SLR1Parser::_newTemp()
{
    static int tempCnt = 0;
    tempCnt++;
    std::string name = "0TEMP_" + std::to_string(tempCnt); // 标识符不能以数字开头，所以以0开头可以防止重名
    _symbolTable.add(name);
    return tempCnt;
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

std::string SLR1Parser::_getSymbolString(const std::stack<PSS> &stack)
{
    auto reversedStack = reverseStack(stack);
    std::string result = "";
    while (!reversedStack.empty())
    {
        std::string symbol = reversedStack.top().first;
        result += symbol;
        reversedStack.pop();
    }
    return result;
}

void SLR1Parser::_printLine(int statusStackLength, const std::stack<int> &statusStack, int symbolStackLength, const std::stack<PSS> &symbolStack, int symbolLength, int actionLength, PII actionValue, int gotoLength, int gotoValue, int codeLength)
{
    auto statusString = _getStatusString(statusStack);
    auto symbolString = _getSymbolString(symbolStack);
    std::string actionString = "";
    std::string gotoString = "";
    static std::string lastCode = ""; // 上一次打印的四元式
    std::string codeString = "";
    if (lastCode != _code)
    {
        // 产生了新的四元式
        lastCode = _code;
        codeString = _code;
    }
    if (actionValue.first == ACTION_SHIFT)
        actionString = "S_" + std::to_string(actionValue.second);
    else if (actionValue.first == ACTION_REDUCE)
        actionString = "r_" + std::to_string(actionValue.second);
    else if (actionValue.first == ACTION_ACCEPT)
        actionString = "acc";
    if (gotoValue >= 0)
        gotoString = std::to_string(gotoValue);
    std::cout << std::setw(statusStackLength) << statusString << "|" << std::setw(symbolStackLength) << symbolString << "|" << std::setw(symbolLength) << _inputSymbol << "|" << std::setw(actionLength) << actionString << "|" << std::setw(gotoLength) << gotoString << "|" << std::setw(codeLength) << codeString << std::endl;
}

std::string SLR1Parser::errorMessage()
{
    return _errorMessage;
}
