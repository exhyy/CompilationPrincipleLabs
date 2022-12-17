#include "ItemSets.h"
#include "FirstSet.h"
#include "FollowSet.h"
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <cstdio>
#include <iomanip>
#include <iostream>

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
        startSymbol = startRule.left; // 更新为拓广后的startSymbol
    }
    else
    {
        startRule = startRulesOrinigal[0];
    }
    _startItem.setRule(startRule);
    _startItem.setDot(0);

    std::map<std::set<Item>, bool> setToStatus; // 项目集映射到对应的状态编号
    const std::set<std::string, SymbolCmp> &terminal = _grammar.terminal();
    const std::set<std::string, SymbolCmp> &nonterminal = _grammar.nonterminal();
    FirstSet firstSet(_grammar);
    FollowSet followSet(_grammar, firstSet);
    auto follow = followSet.data(); // FOLLOW集
    std::map<Rule, int> ruleToNo;   // 规则映射到编号
    auto grammarData = _grammar.data();
    for (int i = 0; i < int(grammarData.size()); i++)
        ruleToNo[grammarData[i]] = i;

    auto I0 = closure({_startItem}); // 初态项目集
    _data.push_back(I0);
    setToStatus[I0] = 0;
    for (int i = 0; i < int(_data.size()); i++)
    {
        // 遍历非终结符
        for (auto n : nonterminal)
        {
            auto newItemSet = GO(_data[i], n);
            if (!newItemSet.empty() && setToStatus.find(newItemSet) == setToStatus.end())
            {
                _data.push_back(newItemSet);
                setToStatus[newItemSet] = _data.size() - 1;
            }
            if (!newItemSet.empty())
            {
                PIS key(i, n);
                _GOTO[key] = setToStatus[newItemSet];
            }
        }

        // 遍历终结符
        for (auto t : terminal)
        {
            auto newItemSet = GO(_data[i], t);
            if (!newItemSet.empty() && setToStatus.find(newItemSet) == setToStatus.end())
            {
                _data.push_back(newItemSet);
                setToStatus[newItemSet] = _data.size() - 1;
            }
            if (!newItemSet.empty())
            {
                PIS key(i, t);
                PII value(ACTION_SHIFT, setToStatus[newItemSet]);
                _ACTION[key] = value;
            }
        }

        // 遍历当前所有项目
        for (auto item : _data[i])
        {
            // 归约项目
            if (item.type(nonterminal, terminal) == ITEM_REDUCED)
            {
                auto followLeft = follow[item.rule().left]; // 左部的FOLLOW集
                for (auto a : followLeft)
                {
                    PIS key(i, a);
                    PII value(ACTION_REDUCE, ruleToNo[item.rule()]);
                    _ACTION[key] = value;
                }

                // 接受项目
                if (item.rule().left == startSymbol)
                {
                    PIS key(i, _grammar.end());
                    PII value(ACTION_ACCEPT, 0);
                    _ACTION[key] = value;
                }
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

std::map<PIS, PII> ItemSets::ACTION()
{
    return _ACTION;
}

std::map<PIS, int> ItemSets::GOTO()
{
    return _GOTO;
}

std::set<Item> ItemSets::closure(std::vector<Item> items)
{
    const std::set<std::string, SymbolCmp> &terminal = _grammar.terminal();
    const std::set<std::string, SymbolCmp> &nonterminal = _grammar.nonterminal();

    // 使用BFS搜索所有结果
    std::set<Item> result;
    std::deque<std::vector<Item>> dequeue;
    dequeue.push_back(items);
    std::map<std::vector<Rule>, bool> usedRules;
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
                if (usedRules[newRules])
                    continue; // 不重复搜索已经搜索过的规则，否则发生死循环
                usedRules[newRules] = true;
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

std::set<Item> ItemSets::GO(const std::set<Item> &itemSet, const std::string symbol)
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

void ItemSets::printParsingTable()
{
    const std::set<std::string, SymbolCmp> &terminal = _grammar.terminal();
    const std::set<std::string, SymbolCmp> &nonterminal = _grammar.nonterminal();
    int statusWidth = 6;

    std::cout << std::left << std::setw(statusWidth) << "Status"
              << "|" << std::setw((terminal.size() + 1) * 5) << "ACTION"
              << "|" << std::setw(nonterminal.size() * 5) << "GOTO" << std::endl;
    std::cout << std::left << std::setw(statusWidth) << ""
              << "|";
    for (auto t : terminal)
        std::cout << std::setw(5) << t;
    std::cout << std::setw(5) << _grammar.end() << "|";
    for (auto n : nonterminal)
        std::cout << std::setw(5) << n;
    std::cout << std::endl;
    int maxLength = statusWidth + (terminal.size() + nonterminal.size() + 1) * 5 + 2;
    std::string line(maxLength, '-');
    std::cout << line << std::endl;

    for (int status = 0; status < int(_data.size()); status++)
    {
        std::cout << std::setw(statusWidth) << status << "|";
        PIS key;
        std::string output = "";

        // 处理终结符和结束符
        for (auto t : terminal)
        {
            key = PIS(status, t);
            output = "";
            if (_ACTION.find(key) != _ACTION.end())
            {
                auto value = _ACTION[key];
                if (value.first == ACTION_SHIFT)
                    output = "S_" + std::to_string(value.second);
                else if (value.first == ACTION_REDUCE)
                    output = "r_" + std::to_string(value.second);
                else
                    output = "acc";
            }
            std::cout << std::setw(5) << output;
        }
        key = PIS(status, _grammar.end());
        output = "";
        if (_ACTION.find(key) != _ACTION.end())
        {
            auto value = _ACTION[key];
            if (value.first == ACTION_SHIFT)
                output = "S_" + std::to_string(value.second);
            else if (value.first == ACTION_REDUCE)
                output = "r_" + std::to_string(value.second);
            else
                output = "acc";
        }
        std::cout << std::setw(5) << output << "|";

        // 处理非终结符
        for (auto n : nonterminal)
        {
            key = PIS(status, n);
            output = "";
            if (_GOTO.find(key) != _GOTO.end())
            {
                output = std::to_string(_GOTO[key]);
            }
            std::cout << std::setw(5) << output;
        }
        std::cout << std::endl;
    }
}
