#ifndef STRING_HANDLER_H
#define STRING_HANDLER_H
#include "../common/global.h"

class StringHandler
{
public:
    // read in line from the terminal
    bool get_input(std::vector<std::string> &parsed);

    // print line to the log
    double printing(int id, std::string type, std::string n, timeval start, std::string qValue = "");

    // parse the input from the terminal
    void parse_input(std::vector<std::string> &parsed_input, std::string input);
};

#endif