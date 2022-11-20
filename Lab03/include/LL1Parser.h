#ifndef LL1PARSER_H_
#define LL1PARSER_H_

#include "ParsingTable.h"
#include <fstream>
#include <stack>
#include <string>
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

class LL1Parser
{
private:
    std::ifstream _infile;
    ParsingTable &_parsingTable;
    std::stack<std::string> _stack;
    std::string _epsilon;
    std::string _start;
    std::string _end;
    int _wordId;
    std::string _wordValue;
    std::string _symbol;
    std::vector<std::string> _idToSymbol;

public:
    LL1Parser(std::string filename, ParsingTable &parsingTable, std::string start);
    int parse();
    void advance();
};

#endif