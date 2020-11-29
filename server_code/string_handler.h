#ifndef STRINGHANDLER_H
#define STRINGHANDLER_H
#include "../common/global.h"
class StringHandler
{
public:
    void parse_input(std::vector<std::string> &parsed_input, std::string input);
    void print(std::string type, std::string name, int count, double time, std::string command = "");
    void print_statistics(std::unordered_map<std::string, int> clients, bool first, int count, double start, double finish);
};

#endif