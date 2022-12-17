#ifndef ITEMSETS_H_
#define ITEMSETS_H_

#include "Grammar.h"
#include "Rule.h"
#include "Item.h"
#include <set>

class ItemSets
{
private:
    Grammar _grammar;
    Item _startItem;
    bool _augmented = false;
    std::vector<std::set<Item>> _data;

public:
    ItemSets(Grammar grammar);
    bool isAugmented();
    std::vector<std::set<Item>> data();
    std::set<Item> closure(std::vector<Item> items);
    std::set<Item> go(const std::set<Item> &itemSet, const std::string symbol);
};

#endif