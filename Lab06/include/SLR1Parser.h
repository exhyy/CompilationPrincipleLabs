#ifndef SLR1PARSER_H_
#define SLR1PARSER_H_

#include "Grammar.h"
#include "ItemSets.h"
#include "SymbolTable.h"
#include <string>
#include <stack>
#include <fstream>
#include <utility>
#include <vector>

#define EOI 0
#define VOID 1
#define INT 2
#define FLOAT 3
#define DOUBLE 4
#define IF 5
#define ELSE 6
#define FOR 7
#define DO 8
#define WHILE 9
#define ID 10           // 标识符
#define UINT 11         // 无符号整数
#define LT 12           // <
#define GT 13           // >
#define EQ 14           // ==
#define LE 15           // <=
#define GE 16           // >=
#define NE 17           // <>
#define AND 18          // &&
#define OR 19           // ||
#define NOT 20          // !
#define ADD 21          // +
#define SUB 22          // -
#define MUL 23          // *
#define DIV 24          // /
#define SEMICOLON 25    // ;
#define COMMA 26        // ,
#define LPARENTHESES 27 // (
#define RPARENTHESES 28 // )
#define LBRACE 29       // {
#define RBRACE 30       // }
#define INC 31          // ++
#define DEC 32          // --
#define LSHIFT 33       // <<
#define RSHIFT 34       // >>
#define ADDE 35         // +=
#define SUBE 36         // -=
#define MULE 37         // *=
#define DIVE 38         // /=
#define ASSIGN 39       // =

typedef std::pair<std::string, std::string> PSS;

class SLR1Parser
{
private:
    Grammar _grammar;
    std::ifstream _infile;
    std::ofstream _outfile;
    std::map<PIS, PII> _ACTION;
    std::map<PIS, int> _GOTO;
    std::stack<int> _statusStack;
    std::stack<PSS> _symbolStack;
    std::vector<std::string> _idToSymbol;
    int _wordId;
    std::string _wordValue;
    std::string _inputSymbol; // 当前输入串符号
    std::string _errorMessage = "";
    SymbolTable _symbolTable; // 符号表
    std::string _code = "";        // 生成的四元式
    void _advance();
    int _doSemanticAction(int ruleId);
    void _generateCode(std::string op, int arg1, int arg2, int target); // arg1，arg2，target都是符号表的下标
    int _newTemp();
    std::string _getStatusString(const std::stack<int> &stack);
    std::string _getSymbolString(const std::stack<PSS> &stack);
    void _printLine(int statusStackLength, const std::stack<int> &statusStack, int symbolStackLength, const std::stack<PSS> &symbolStack, int symbolLength, int actionLength, PII actionValue, int gotoLength, int gotoValue, int codeLength);

public:
    SLR1Parser(Grammar grammar, ItemSets itemSets);
    int parse(std::string inputFilename, std::string outputFilename, bool debug = false);
    std::string errorMessage();
};

#endif