#include <iostream>
#include "Grammar.h"
#include "Item.h"
#include "ItemSets.h"
#include "FirstSet.h"
#include "FollowSet.h"
#include "SLR1Parser.h"

int main(int argc, char *argv[])
{
    if (argc > 5)
    {
        std::cerr << "参数过多" << std::endl;
        return 1;
    }
    if (argc < 4)
    {
        std::cerr << "至少需要3个参数：文法文件、输入文件和输出文件" << std::endl;
        return 1;
    }
    bool debug = false;
    if (argc == 5 && std::string(argv[4]) == "--debug")
        debug = true;
    Grammar grammar(argv[1], "\\epsilon", "#");
    ItemSets itemSets(grammar, true);
    if (itemSets.isAugmented())
        std::cout << "文法已增广" << std::endl;
    FirstSet firstSet(grammar);
    FollowSet followSet(grammar, firstSet);

    SLR1Parser parser(grammar, itemSets);

    if (debug)
    {
        std::cout << "SLR(1)分析表如下：" << std::endl;
        itemSets.printParsingTable();
        std::cout << std::endl
                  << "分析过程如下：" << std::endl;
    }
    int t = parser.parse(argv[2], argv[3], debug);
    if (t == 0)
        std::cout << "语法分析成功" << std::endl;
    else
        std::cout << "语法分析失败" << std::endl
                  << "错误位置：" << t << std::endl
                  << "错误信息：" << parser.errorMessage() << std::endl;
    return 0;
}