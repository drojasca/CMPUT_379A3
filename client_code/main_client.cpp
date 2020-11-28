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

    Client client(ip);
    client.run(port);

    return 0;
}