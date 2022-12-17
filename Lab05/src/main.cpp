#include <iostream>
#include "Grammar.h"
#include "Item.h"
#include "ItemSets.h"

int main(int argc, char *argv[])
{
    Grammar grammar(argv[1], "", "S'", "#");
    std::cout << "Grammar构建完成" << std::endl;
    ItemSets itemSets(grammar);
    std::cout << "ItemSets构建完成" << std::endl;
    if (itemSets.isAugmented())
        std::cout << "文法已增广" << std::endl;

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
    // auto startRulesOrinigal = grammar.collectByLeft("S'");
    // std::cout << "startRulesOrinigal.size(): " << startRulesOrinigal.size() << std::endl;
    // Item item;
    // item.setRule(startRulesOrinigal[0]);
    // item.setDot(0);
    // auto firstClosure = itemSets.closure({item});
    // for (auto item : firstClosure)
    // {
    //     std::cout << item.rule().left << "->";
    //     for (auto symbol : item.rule().right)
    //     {
    //         std::cout << symbol;
    //     }
    //     std::cout << "   dot: " << item.dot() << std::endl;
    // }
    return 0;
}