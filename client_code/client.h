#ifndef CLIENT_H
#define CLIENT_H
#include "string_handler.h"
#include "global.h"
class Client
{
public:
    void run(std::string port, std::string ip);
    bool initialize();
    bool send_server();
    bool get_work(std::string &num);

private:
    int port;
    int fd;
    std::string ip;
    StringHandler handler;
};

#endif