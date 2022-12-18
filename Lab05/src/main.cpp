#include <iostream>
#include <fstream>
#include "Grammar.h"
#include "Item.h"
#include "ItemSets.h"
#include "FirstSet.h"
#include "FollowSet.h"
#include "SLR1Parser.h"

int main(int argc, char *argv[])
{
    Grammar grammar(argv[1], "\\epsilon", "#");
    std::cout << "Grammar构建完成" << std::endl;
    ItemSets itemSets(grammar, true);
    std::cout << "ItemSets构建完成" << std::endl;
    if (itemSets.isAugmented())
        std::cout << "文法已增广" << std::endl;

    FirstSet firstSet(grammar);
    // std::cout << "FirstSet: " << std::endl;
    // firstSet.print();

    FollowSet followSet(grammar, firstSet);
    // std::cout << std::endl
            //   << "FollowSet: " << std::endl;
    // followSet.print();
    itemSets.printParsingTable();

    SLR1Parser parser(grammar, itemSets);
    int t = parser.parse(argv[2], argv[3], true);
    if (t == 0)
        std::cout << "语法分析成功" << std::endl;
    else
        std::cout << "语法分析失败" << std::endl
                  << "错误位置：" << t << std::endl
                  << "错误信息：" << parser.errorMessage() << std::endl;

    // auto itemSetsData = itemSets.data();
    // for (int i = 0; i < int(itemSetsData.size()); i++)
    // {
    //     std::cout << "Item Set " << i << ": " << std::endl;
    //     for (auto item : itemSetsData[i])
    //     {
    //         std::cout << item.rule().left << "->";
    //         auto right = item.rule().right;
    //         for (int j = 0; j < int(right.size()); j++)
    //         {
    //             if (j == item.dot())
    //                 std::cout << "·";
    //             std::cout << right[j];
    //         }
    //         if (item.dot() == int(right.size()))
    //             std::cout << "·";
    //         std::cout << std::endl;
    //     }
    //     std::cout << std::endl
    //               << std::endl;
    // }
    return 0;
}