#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <vector>
#include <string>
#include <unordered_map>

class SymbolTable
{
private:
    std::vector<std::string> _data;
    std::unordered_map<std::string, int> _index;

public:
    std::vector<std::string> data();
    bool exist(const std::string &symbol);
    void add(const std::string &symbol);
    int getIndex(const std::string &symbol);
    std::string &operator[](int index);
};

#endif
