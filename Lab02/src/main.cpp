#include "Lab02.h"
#include <iostream>

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
            throw std::string("请提供1个参数作为输入文件名");
        
        Lab02 task = Lab02(argv[1]);
        if (task.run())
            std::cout << "语法分析通过" << std::endl;
        else
        {
            task.printError();
            std::cout << "语法分析失败" << std::endl;
        }
    } catch (std::string &msg)
    {
        std::cerr << msg << std::endl;
    }
    return 0;
}