#ifndef SEVER_H
#define SEVER_H
#include <string>
#include <regex>

class Server
{
public:
    void run(std::string port);
    bool initialize();
    bool listen_client();

private:
    int port;
    int fd;
};

#endif