#include "ItemSets.h"
#include <string>
#include <vector>
#include <deque>
#include <map>

ItemSets::ItemSets(Grammar grammar) : _grammar(grammar)
{
    Rule startRule;
    std::string startSymbol = grammar.start();
    auto startRulesOrinigal = grammar.collectByLeft(startSymbol);
    if (startRulesOrinigal.size() > 1)
    {
        // 拓广文法
        std::vector<std::string> right;
        right.push_back(startSymbol);
        startRule.left = startSymbol + "'";
        startRule.right = right;
        _augmented = true;
    }
    else
    {
        startRule = startRulesOrinigal[0];
    }
    _startItem.setRule(startRule);
    _startItem.setDot(0);

    // std::set<std::set<Item>> _setAdded;
    std::map<std::set<Item>, bool> _setAdded;
    const std::set<std::string, SymbolCmp> &terminal = _grammar.terminal();
    const std::set<std::string, SymbolCmp> &nonterminal = _grammar.nonterminal();
    auto I0 = closure({_startItem}); // 初态项目集
    _data.push_back(I0);
    _setAdded[I0] = true;
    // _setAdded.insert(I0);
    for (int i = 0; i < int(_data.size()); i++)
    {
        // 遍历非终结符
        for (auto n : nonterminal)
        {
            auto newItemSet = go(_data[i], n);
            // if (!newItemSet.empty() && _setAdded.find(newItemSet) == _setAdded.end())
            if (!newItemSet.empty() && !_setAdded[newItemSet])
            {
                _data.push_back(newItemSet);
                // _setAdded.insert(newItemSet);
                _setAdded[newItemSet] = true;
            }
        }

        // 遍历终结符
        for (auto t : terminal)
        {
            auto newItemSet = go(_data[i], t);
            // if (!newItemSet.empty() && _setAdded.find(newItemSet) == _setAdded.end())
            if (!newItemSet.empty() && !_setAdded[newItemSet])
            {
                _data.push_back(newItemSet);
                // _setAdded.insert(newItemSet);
                _setAdded[newItemSet] = true;
            }
        }
    }
}

bool ItemSets::isAugmented()
{
    return _augmented;
}

std::vector<std::set<Item>> ItemSets::data()
{
    return _data;
}

std::set<Item> ItemSets::closure(std::vector<Item> items)
{
    const std::set<std::string, SymbolCmp> &terminal = _grammar.terminal();
    const std::set<std::string, SymbolCmp> &nonterminal = _grammar.nonterminal();

    // 使用BFS搜索所有结果
    std::set<Item> result;
    std::deque<std::vector<Item>> dequeue;
    dequeue.push_back(items);
    while (!dequeue.empty())
    {
        auto head = dequeue.front();
        dequeue.pop_front();
        for (auto item : head)
        {
            result.insert(item);
            if (item.type(nonterminal, terminal) == ITEM_TO_BE_REDUCED)
            {
                auto behindDot = item.behindDot();                 // 圆点后面的非终结符
                auto newRules = _grammar.collectByLeft(behindDot); // 以该非终结符为左部的所有规则
                std::vector<Item> newItems;
                Item newItem;
                for (auto rule : newRules)
                {
                    newItem.setRule(rule);
                    newItem.setDot(0);
                    newItems.push_back(newItem);
                }
                dequeue.push_back(newItems);
            }
        }
    }
    return result;
}

std::set<Item> ItemSets::go(const std::set<Item> &itemSet, const std::string symbol)
{
    std::vector<Item> forward; // 存放圆点向前移动的项目
    for (auto item : itemSet)
    {
        if (item.behindDot() == symbol)
        {
            Item newItem;
            newItem.setRule(item.rule());
            newItem.setDot(item.dot() + 1);
            forward.push_back(newItem);
        }
    }
    return closure(forward);
}
