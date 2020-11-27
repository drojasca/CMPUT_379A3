#ifndef SEVER_H
#define SEVER_H
#include <string>
#include <regex>
#include "../global.h"
#include "../tands.h"

class Server
{
public:
    void run(std::string port);
    bool initialize();
    bool listen_client();

private:
    int port;
    int fd;
    int count = 0;
    pollfd fds[200];
    int timeout = 30000;
    int rc;
    int nfs = 1;
};

#endif