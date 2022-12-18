#include "Rule.h"

bool Rule::operator<(const Rule &that) const
{
    std::string a = left, b = that.left;
    for (auto symbol : right)
        a += symbol;
    for (auto symbol : that.right)
        b += symbol;
    return a < b;
}