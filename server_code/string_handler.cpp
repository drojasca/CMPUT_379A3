#include "string_handler.h"

void StringHandler::parse_input(std::vector<std::string> &parsed_input, std::string input)
{
    std::string temp;
    // parse string on white space
    for (auto c : input)
    {
        if (c == ' ' && temp.size() != 0)
        {
            parsed_input.push_back(temp);
            temp = "";
        }

        if (c != ' ')
        {
            temp += c;
        }
    }

    if (temp.size() != 0)
    {
        parsed_input.push_back(temp);
    }
}

void StringHandler::print(std::string type, std::string name, int count, double time, std::string command)
{
    // print the correct output to the system
    if (type == "work")
    {
        printf("%.2f: # %3d (T%3s) from %s\n", time, count, command.c_str(), name.c_str());
    }
    else if (type == "done")
    {
        printf("%.2f: # %3d (Done) from %s\n", time, count, name.c_str());
    }
}

void StringHandler::print_statistics(std::unordered_map<std::string, int> clients, bool first, int count, double start, double finish)
{
    // print the summary statistics 
    printf("\nSUMMARY\n");
    for (auto &name : clients)
    {
        printf("\t%d transactions from %s\n", name.second, name.first.c_str());
    }

    double duration = first ? finish - start : 0;

    printf("%.1f transactions/sec (%d/%.2f)\n", (double)(count / (duration == 0 ? 1 : duration)), count, duration);
}