#include "client.h"

int main(int argc, char **argv)
{
    std::string port, ip;
    if (argc > 2)
    {
        port = argv[1];
        ip = argv[2];
    }
    else
        return 0;
    Client client;
    client.run(port, ip);
    return 0;
}