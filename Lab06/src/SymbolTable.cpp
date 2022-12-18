#include "SymbolTable.h"

std::vector<std::string> SymbolTable::data()
{
    return _data;
}

bool SymbolTable::exist(const std::string &symbol)
{
    return _index.find(symbol) != _index.end();
}

void SymbolTable::add(const std::string &symbol)
{
    if (!exist(symbol))
    {
        _data.push_back(symbol);
        _index[symbol] = _data.size() - 1;
    }
}

int SymbolTable::getIndex(const std::string &symbol)
{
    if (exist(symbol))
        return _index[symbol];
    else
        return -1;
}

std::string &SymbolTable::operator[](int index)
{
    return _data[index];
}
