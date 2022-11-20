#include "FirstSet.h"
#include "FollowSet.h"
#include "ParsingTable.h"
#include "LL1Parser.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::string grammar = argv[1];
    std::string input = argv[2];
    FirstSet firstSet(grammar, "\\epsilon");
    FollowSet followSet(grammar, firstSet, "#");
    ParsingTable parsingTable(grammar, firstSet, followSet);
    LL1Parser parser(input, parsingTable, "S");
    int result = parser.parse();
    if (result == 0)
        std::cout << "语法分析通过！" << std::endl;
    else
        std::cout << "语法分析失败！出错位置为第" << result << "个单词。" << std::endl;
    return 0;
}