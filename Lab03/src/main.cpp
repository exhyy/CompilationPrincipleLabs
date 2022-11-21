#include "FirstSet.h"
#include "FollowSet.h"
#include "ParsingTable.h"
#include "LL1Parser.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc > 4)
        throw std::string("参数过多");
    if (argc < 3)
        throw std::string("至少需要2个参数：文法文件和输入文件");
    bool debug = false;
    if (argc == 4 && std::string(argv[3]) == "--debug")
        debug = true;
    std::string grammar = argv[1];
    std::string input = argv[2];
    FirstSet firstSet(grammar, "\\epsilon");
    FollowSet followSet(grammar, firstSet, "#");
    ParsingTable parsingTable(grammar, firstSet, followSet);
    LL1Parser parser(input, parsingTable, "S");
    if (debug)
    {
        std::cout << "FIRST集如下：" << std::endl;
        firstSet.print();
        std::cout << std::endl
                  << "FOLLOW集如下：" << std::endl;
        followSet.print();
        std::cout << std::endl
                  << "LL1分析表如下：" << std::endl;
        parsingTable.print();
        std::cout << std::endl
                  << "LL1分析过程如下：" << std::endl;
    }
    int result = parser.parse(debug);
    if (result == 0)
        std::cout << "语法分析通过！" << std::endl;
    else
        std::cout << "语法分析失败！出错位置为第" << result << "个单词。" << std::endl;
    return 0;
}