#ifndef STRINGHANDLER_H
#define STRINGHANDLER_H
#include "../common/global.h"
class StringHandler
{
public:

    //parse input on whitespace
    void parse_input(std::vector<std::string> &parsed_input, std::string input);

    // print when a message is recieved and when it has completed the work
    void print(std::string type, std::string name, int count, double time, std::string command = "");

    // print the summary statistics from the client
    void print_statistics(std::unordered_map<std::string, int> clients, bool first, int count, double start, double finish);
};

#endif