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

    // function responsible for running the server
    void run(std::string port);

    // create and bind the socket
    bool initialize();

    // deal with incoming messages from client
    bool listen_client();

    // parse the message from the client
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
    double finish;
    StringHandler handler;
};

#endif