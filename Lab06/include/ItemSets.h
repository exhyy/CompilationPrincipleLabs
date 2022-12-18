#ifndef ITEMSETS_H_
#define ITEMSETS_H_

#include "Grammar.h"
#include "Rule.h"
#include "Item.h"
#include <set>
#include <utility>
#include <map>

typedef std::pair<int, std::string> PIS; // {状态, 符号}
typedef std::pair<int, int> PII;         // ACTION: {移进/归约, 编号}

#define ACTION_SHIFT 1  // 移进
#define ACTION_REDUCE 2 // 归约
#define ACTION_ACCEPT 3 // 接受

class ItemSets
{
private:
    Grammar _grammar;
    Item _startItem;
    bool _augmented = false;
    std::vector<std::set<Item>> _data;
    std::map<PIS, PII> _ACTION;
    std::map<PIS, int> _GOTO;

public:
    ItemSets(Grammar grammar, bool forcedAugmention=false);
    bool isAugmented();
    std::vector<std::set<Item>> data();
    std::map<PIS, PII> ACTION();
    std::map<PIS, int> GOTO();
    std::set<Item> closure(std::vector<Item> items);
    std::set<Item> GO(const std::set<Item> &itemSet, const std::string symbol);
    void printParsingTable();
};

#endif