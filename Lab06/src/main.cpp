#include <iostream>
#include "Grammar.h"
#include "Item.h"
#include "ItemSets.h"
#include "FirstSet.h"
#include "FollowSet.h"
#include "SLR1Parser.h"
#include "lexical.h"
#include <cstdio>
#include <cstdlib>
#include <filesystem> // 需要C++17

int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        std::cerr << "错误：参数过多" << std::endl;
        return 1;
    }
    if (argc < 2)
    {
        std::cerr << "错误：至少需要一个参数：输入文件" << std::endl; 
    }
    FILE *lexSrc, *lexDest;
    lexSrc = fopen(argv[1], "r");
    if (lexSrc == NULL)
    {
        fprintf(stderr, "错误：无法读取输入文件：%s\n", argv[1]);
        return 2;
    }

    std::string lexicalOutput = "lexical_output.txt";
    lexDest = fopen(lexicalOutput.c_str(), "w+");
    if (lexDest == NULL)
    {
        fprintf(stderr, "错误：无法读取词法输出文件：%s\n", lexicalOutput.c_str());
        return 2;
    }

    full_scan(lexSrc, lexDest, 0); // 进行词法分析
    fclose(lexSrc);
    fclose(lexDest);
    std::cout << "生成词法分析结果：" << lexicalOutput << std::endl;

    std::string grammarFile = "grammar.txt";
    std::string grammarOutput = "code.txt";
    if (!std::filesystem::exists(grammarFile))
    {
        std::cerr << "错误：文法文件grammmar.txt不存在" << std::endl;
        return 3;
    }
    bool debug = false;
    if (argc == 3 && std::string(argv[2]) == "--debug")
        debug = true;
    Grammar grammar(grammarFile, "\\epsilon", "#");
    ItemSets itemSets(grammar, true);
    FirstSet firstSet(grammar);
    FollowSet followSet(grammar, firstSet);
    SLR1Parser parser(grammar, itemSets);
    if (debug)
    {
        std::cout << "SLR(1)分析表如下：" << std::endl;
        itemSets.printParsingTable();
        std::cout << std::endl
                  << "分析过程如下：" << std::endl;
    }
    int t = parser.parse(lexicalOutput, grammarOutput, debug);
    if (t == 0)
    {
        std::cout << "语法分析成功" << std::endl;
        std::cout << "生成词法分析结果：" << grammarOutput << std::endl;
    }
    else
    {
        std::cout << "语法分析失败" << std::endl
                  << "错误位置：" << t << std::endl
                  << "错误信息：" << parser.errorMessage() << std::endl;
        std::cout << "若确认语法无误，请检查grammar.txt的内容是否正确" << std::endl;
    }
    return 0;
}