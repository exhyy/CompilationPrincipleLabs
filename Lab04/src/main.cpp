#include "PrecedenceTable.h"
#include <iostream>
#include <cstdio>

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

    auto tableData = precedenceTable.data();
    fprintf(stdout, "%-5s", "");
    for (auto t : terminal)
        fprintf(stdout, "%-5s", t.c_str());
    fprintf(stdout, "\n");
    for (auto t1 : terminal)
    {
        fprintf(stdout, "%-5s", t1.c_str());
        for (auto t2 : terminal)
        {
            PSS key(t1, t2);
            if (tableData.find(key) != tableData.end())
            {
                int value = tableData[key];
                if (value == PRECEDENCE_EQ)
                    fprintf(stdout, "%-5s", "=");
                else if (value == PRECEDENCE_LT)
                    fprintf(stdout, "%-5s", "<");
                else if (value == PRECEDENCE_GT)
                    fprintf(stdout, "%-5s", ">");
            }
            else
                fprintf(stdout, "%-5s", "NULL");
        }
        fprintf(stdout, "\n");
    }
    return 0;
}
