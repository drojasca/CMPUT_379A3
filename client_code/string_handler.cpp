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

bool StringHandler::get_input(std::vector<std::string> &parsed)
{
    std::string input;

    // check if line is EOF character
    if (!std::getline(std::cin, input))
    {
        return false;
    }

    // store input
    else
    {
        this->parse_input(parsed, input);
    }

    return true;
}