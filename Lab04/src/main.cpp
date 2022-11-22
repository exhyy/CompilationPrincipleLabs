#include "PrecedenceTable.h"
#include <iostream>

int main()
{
    PrecedenceTable precedenceTable("grammar.txt");
    auto terminal = precedenceTable.terminal();
    auto nonterminal = precedenceTable.nonterminal();

    std::cout << "Nonterminal:" << std::endl;
    for (auto n : nonterminal)
    {
        std::cout << n << std::endl;
    }

    std::cout << "Terminal:" << std::endl;
    for (auto t : terminal)
    {
        std::cout << t << std::endl;
    }

    auto firstvt = precedenceTable.firstvt();
    for (auto d : firstvt)
    {
        std::cout << "FIRSTVT(" << d.first << "): ";
        for (auto s : d.second)
        {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }

    auto lastvt = precedenceTable.lastvt();
    for (auto d : lastvt)
    {
        std::cout << "LASTVT(" << d.first << "): ";
        for (auto s : d.second)
        {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
