#ifndef SEVER_H
#define SEVER_H
#include <string>
#include <regex>
#include "../common/global.h"
#include "../common/tands.h"
#include "string_handler.h"

class Server
{
public:
    void run(std::string port);
    bool initialize();
    bool listen_client();
    std::string handleMessage(std::string message);

private:
    int port;
    int fd;
    int count = 0;
    pollfd fds[1];
    int timeout = 30000;
    int rc;
    int nfs = 1;
    std::unordered_map<std::string, int> clients;
    bool first = false;
    double start;
    StringHandler handler;
};

#endif