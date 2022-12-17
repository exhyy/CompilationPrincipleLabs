#include <iostream>
#include "Grammar.h"
#include "Item.h"
#include "ItemSets.h"
#include "FirstSet.h"
#include "FollowSet.h"

int main(int argc, char *argv[])
{
    Grammar grammar(argv[1], "\\epsilon", "S'", "#");
    std::cout << "Grammar构建完成" << std::endl;
    ItemSets itemSets(grammar);
    std::cout << "ItemSets构建完成" << std::endl;
    if (itemSets.isAugmented())
        std::cout << "文法已增广" << std::endl;

    FirstSet firstSet(grammar);
    std::cout << "FirstSet: " << std::endl;
    firstSet.print();

    FollowSet followSet(grammar, firstSet);
    std::cout << std::endl << "FollowSet: " << std::endl;
    followSet.print();

    auto itemSetsData = itemSets.data();
    for (int i = 0; i < int(itemSetsData.size()); i++)
    {
        std::cout << "Item Set " << i << ": " << std::endl;
        for (auto item : itemSetsData[i])
        {
            std::cout << item.rule().left << "->";
            auto right = item.rule().right;
            for (int j = 0; j < int(right.size()); j++)
            {
                if (j == item.dot())
                    std::cout << "·";
                std::cout << right[j];
            }
            if (item.dot() == int(right.size()))
                std::cout << "·";
            std::cout << std::endl;
        }
        std::cout << std::endl
                  << std::endl;
    }
    itemSets.printParsingTable();
    return 0;
}