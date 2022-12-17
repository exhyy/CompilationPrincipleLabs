#include <string>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <iostream>

int main()
{
    int terminalLength = 6;
    int nonterminalLength = 3;
    std::cout << std::left << std::setw(10) << "Status" << "|" <<std::setw(terminalLength * 5) << "ACTION" << "|" << std::setw(nonterminalLength * 5) << "GOTO" << std::endl;
    
    // std::string format = "%%-5s|%%-";
    // std::stringstream stream;
    // std::string str = "";
    // stream << terminalLength * 5;
    // stream >> str;
    // format = format + str + "s|%%-";
    // stream << nonterminalLength * 5;
    // stream >> str;
    // format = format + str + "s\n";

    // fprintf(stdout, format.c_str(), "Status", "ACTION", "GOTO");
    return 0;
}