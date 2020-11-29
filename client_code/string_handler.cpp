#include "string_handler.h"

StringHandler::StringHandler() {}

StringHandler::StringHandler(std::string name)
{
    this->fd = open(name.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("Could not create file");
        exit(-1);
    }

    // close standard out
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("could not open file");
        exit(-1);
    }
}
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

void StringHandler::print(std::string val, std::string type)
{
    auto current = std::chrono::system_clock::now();
    double current_epoch = std::chrono::duration<double>(current.time_since_epoch()).count();
    if (type == "send")
    {
        printf("T%-7s%.2f: Send (T%3s)\n", val.c_str(), current_epoch, val.c_str());
    }
    else if (type == "recv")
    {
        printf("\t\t%10.2f: Recv (D%3s)\n", current_epoch, val.c_str());
    }
    else if (type == "sleep")
    {
        printf("\t\tSleep %s units\n", val.c_str());
    }
}

void StringHandler::finalize()
{
    close(this->fd);
}