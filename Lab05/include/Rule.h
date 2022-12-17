#ifndef RULE_H_
#define RULE_H_

#include <string>
#include <vector>

class Rule
{
public:
    std::string left = "";
    std::vector<std::string> right;
    bool operator<(const Rule &that) const;
};

#endif