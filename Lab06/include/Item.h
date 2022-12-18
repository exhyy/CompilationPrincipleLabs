#ifndef ITEM_H_
#define ITEM_H_

#include <set>
#include "Rule.h"
#include "utils.h"

#define ITEM_ERROR_TYPE -1   // 错误
#define ITEM_REDUCED 1       // 归约项目：圆点在最后
#define ITEM_SHIFTED 2       // 移进项目：圆点后面是终结符
#define ITEM_TO_BE_REDUCED 3 // 待约项目：圆点后面是非终结符

class Item
{
private:
    Rule _rule;
    int _dot = 0;

public:
    Item();
    Item(Rule rule, int dot);
    void setRule(Rule rule);
    void setDot(int dot);
    Rule rule();
    int dot();
    int type(const std::set<std::string, SymbolCmp> &nonterminal, const std::set<std::string, SymbolCmp> &terminal);
    std::string behindDot();                // 返回圆点后面的符号
    bool operator<(const Item &that) const; // 使用std::set<Item>要求重载<
};

#endif