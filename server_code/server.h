#ifndef SEVER_H
#define SEVER_H
#include <string>
#include <regex>
#include "../common/global.h"
#include "../common/tands.h"

class Server
{
public:
    void run(std::string port);
    bool initialize();
    bool listen_client();
    void print(std::string type, std::string name, std::string command = "");
    std::string handleMessage(std::string message);
    void print_statistics();

private:
    bool first = true;
    int port;
    int fd;
    int count = 0;
    pollfd fds[1];
    int timeout = 30000;
    int rc;
    int nfs = 1;
    std::unordered_map<std::string, int> clients;
};

#endif