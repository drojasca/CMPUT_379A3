#include <string>
#include <iostream>
#include "server.h"

int main(int argc, char **argv)
{
    std::string port;
    if (argc > 1)
    {
        port = argv[1];
    }

    else
        return 0;

    Server server;
    server.run(port);

    return 0;
}