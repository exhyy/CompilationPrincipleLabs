#include "PrecedenceTable.h"
#include "OPGParser.h"
#include <iostream>
#include <cstdio>

int main(int argc, char *argv[])
{
    if (argc > 4)
        throw std::string("参数过多");
    if (argc < 3)
        throw std::string("至少需要2个参数：文法文件和输入文件");
    bool debug = false;
    if (argc == 4 && std::string(argv[3]) == "--debug")
        debug = true;

    std::string grammar = argv[1];
    std::string input = argv[2];
    PrecedenceTable precedenceTable(grammar, "#");

    if (debug)
    {
        std::cout << "FIRSTVT集如下：" << std::endl;
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

        std::cout << std::endl
                  << "LASTVT集如下：" << std::endl;
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

        std::cout << std::endl
                  << "算符优先关系矩阵如下： " << std::endl;
        precedenceTable.print();

        std::cout << std::endl
                  << "分析过程如下：" << std::endl;
    }

    OPGParser parser(input, grammar, precedenceTable, "#");
    int pos = parser.parse(debug);
    if (pos == 0)
        std::cout << "语法分析通过！" << std::endl;
    else
        std::cout << "语法分析失败！出错位置为第" << pos << "个单词。" << std::endl;
    return 0;
}
