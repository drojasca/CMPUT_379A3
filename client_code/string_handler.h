#ifndef STRING_HANDLER_H
#define STRING_HANDLER_H
#include "../common/global.h"

class StringHandler
{
public:
    // main constructor
    StringHandler(std::string name);

    // empty constructor
    StringHandler();

    // read in line from the terminal
    bool get_input(std::vector<std::string> &parsed);

    // print line to the log
    double printing(int id, std::string type, std::string n, timeval start, std::string qValue = "");

    // parse the input from the terminal
    void parse_input(std::vector<std::string> &parsed_input, std::string input);

    // print to the log file
    void print(std::string val, std::string type);

    // close the file descriptor
    void finalize();

private:
    int fd;
};

#endif