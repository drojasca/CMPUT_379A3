#include "string_handler.h"

// double StringHandler::printing(int id, std::string type, std::string n, timeval start, std::string qValue)
// {

//     struct timeval end;
//     gettimeofday(&end, NULL);
//     double time = (end.tv_sec + (double)end.tv_usec / 1000000) - ((double)start.tv_usec / 1000000 + start.tv_sec);

//     printf("\t%-7.3f ID= %-4d %-8s%-10s%s\n", time, id, qValue.c_str(), type.c_str(), n.c_str());
//     fflush(stdout);

//     return time;
// }

// int StringHandler::initialise()
// {
//     std::string fileName = "";
//     int fd = open(fileName.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

//     if (fd == -1)
//     {
//         perror("Could not create file");
//         exit(-1);
//     }

//     // close standard out
//     if (dup2(fd, STDOUT_FILENO) < 0)
//     {
//         perror("could not open file");
//         exit(-1);
//     }

//     return fd;
// }

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