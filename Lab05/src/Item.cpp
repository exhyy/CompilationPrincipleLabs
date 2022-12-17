#include "Item.h"

Item::Item()
{
    // default
}

Item::Item(Rule rule, int dot)
{
    _rule = rule;
    _dot = dot;
}

void Item::setRule(Rule rule)
{
    _rule = rule;
}

void Item::setDot(int dot)
{
    _dot = dot;
}

Rule Item::rule()
{
    return _rule;
}

int Item::dot()
{
    return _dot;
}

int Item::type(const std::set<std::string, SymbolCmp> &nonterminal, const std::set<std::string, SymbolCmp> &terminal)
{
    if (_dot >= int(_rule.right.size()))
        return ITEM_REDUCED;
    if (nonterminal.find(_rule.right[_dot]) != nonterminal.end())
        return ITEM_TO_BE_REDUCED;
    if (terminal.find(_rule.right[_dot]) != terminal.end())
        return ITEM_REDUCED;
    return ITEM_ERROR_TYPE;
}

std::string Item::behindDot()
{
    if (_dot >= int(_rule.right.size()))
        return "";
    else
        return _rule.right[_dot];
}

bool Item::operator<(const Item &that) const
{
    std::string a = _rule.left, b = that._rule.left;
    for (int i = 0; i < int(_rule.right.size()); i++)
    {
        if (i == _dot)
            a += ".";
        a += _rule.right[i];
    }
    if (_dot == int(_rule.right.size()))
        a += ".";

    for (int j = 0; j < int(that._rule.right.size()); j++)
    {
        if (j == that._dot)
            b += ".";
        b += that._rule.right[j];
    }
    if (that._dot == int(that._rule.right.size()))
        b += ".";
    return a < b;
    // if (_rule.left < that._rule.left)
    //     return true;
    // else
    // {
    //     std::string a = "", b = "";
    //     for (int i = 0; i < int(_rule.right.size()); i++)
    //     {
    //         if (i == _dot)
    //             a += ".";
    //         a += _rule.right[i];
    //     }
    //     if (_dot == int(_rule.right.size()))
    //         a += ".";

    //     for (int j = 0; j < int(that._rule.right.size()); j++)
    //     {
    //         if (j == that._dot)
    //             b += ".";
    //         b += that._rule.right[j];
    //     }
    //     if (that._dot == int(that._rule.right.size()))
    //         b += ".";
    //     return a < b;
    //     // for (auto symbol : _rule.right)
    //     //     a += symbol;
    //     // for (auto symbol : that._rule.right)
    //     //     b += symbol;
    //     // if (a < b)
    //     //     return true;
    //     // else
    //     //     return _dot < that._dot;
    // }
}
