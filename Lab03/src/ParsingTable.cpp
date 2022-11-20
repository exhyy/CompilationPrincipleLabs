#include "ParsingTable.h"

ParsingTable::ParsingTable(std::string filename, FirstSet &firstSet, FollowSet &followSet) : _firstSet(firstSet), _followSet(followSet)
{
    _epsilon = firstSet.epsilon();
    _infile.open(filename, std::ios::in);
    if (!_infile.is_open())
    {
        std::string msg = "无法打开" + filename;
        throw msg;
    }
    _getData();
}

void ParsingTable::_getData()
{
    _infile.clear();
    _infile.seekg(0, std::ios::beg);
    std::string line;
    while (!_infile.eof())
    {
        getline(_infile, line);
        int index = line.find("->");
        if (index == -1)
            continue;
        std::string left = line.substr(0, index);
        std::string right = line.substr(index + 2);
        std::vector<std::string> splitedRight = splitString(right, "|");
        for (auto alpha : splitedRight)
        {
            auto firstAlpha = _firstSet.string(alpha);
            if (firstAlpha.find(_epsilon) == firstAlpha.end())
            {
                for (auto a : firstAlpha)
                {
                    PSS key(left, a);
                    _data[key] = alpha;
                }
            }
            else
            {
                auto leftFollow = _followSet.data()[left];
                for (auto b : leftFollow)
                {
                    PSS key(left, b);
                    _data[key] = _epsilon;
                }
            }
        }
    }
}

std::map<PSS, std::string> ParsingTable::data()
{
    return _data;
}
