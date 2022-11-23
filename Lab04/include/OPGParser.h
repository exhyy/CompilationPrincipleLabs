#ifndef OPGPARSER_H_
#define OPGPARSER_H_

#include "PrecedenceTable.h"
#include <fstream>
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

class OPGParser
{
private:
    std::ifstream _input;
    std::string _grammarFilename;
    PrecedenceTable _precedenceTable;
    std::string _end;
    std::vector<std::string> _stack;
    int _wordId;
    std::string _wordValue;
    std::string _symbol;
    std::vector<std::string> _idToSymbol;
    std::map<std::string, std::string> _reduction;
    void _getReduction();
    std::string _getStackString(std::string poped = "");
    std::string _getPrecedenceString();

public:
    OPGParser(std::string inputFilename, std::string grammarFilename, PrecedenceTable precedenceTable, std::string end);
    int parse(bool debug = false);
    void advance();
};

#endif
