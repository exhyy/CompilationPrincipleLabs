#include "FirstSet.h"
#include "FollowSet.h"
#include <set>
#include <string>
#include <iostream>
#include <vector>

int main()
{
    FirstSet firstSet("/home/yuyao/code/CompilationPrincipleLabs/Lab03/grammar.txt", "\\epsilon");
    std::set<std::string, SymbolCmp> nonterminal = firstSet.nonterminal();
    std::set<std::string, SymbolCmp> terminal = firstSet.terminal();

    std::cout << "Terminal: " << std::endl;
    for (auto t : terminal)
    {
        std::cout << t << std::endl;
    }

    std::cout << "Nonterminal: " << std::endl;
    for (auto n : nonterminal)
    {
        std::cout << n << std::endl;
    }

    auto firstData = firstSet.data();
    for (auto d : firstData)
    {
        std::cout << "FIRST(" << d.first << "): ";
        for (auto f : d.second)
        {
            std::cout << f << " ";
        }
        std::cout << std::endl;
    }

    FollowSet followSet("/home/yuyao/code/CompilationPrincipleLabs/Lab03/grammar.txt", firstSet, "#");
    auto followData = followSet.data();
    for (auto d : followData)
    {
        std::cout << "FOLLOW(" << d.first << "): ";
        for (auto f : d.second)
        {
            std::cout << f << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}