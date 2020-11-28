#ifndef CLIENT_H
#define CLIENT_H
#include "string_handler.h"
#include "../common/global.h"
#include "../common/tands.h"
class Client
{
public:
    Client(std::string ip);
    void run(std::string port);
    void initialize();
    bool get_work();
    bool connect_server();
    void print(std::string command, std::string type);
    bool get_response();
    bool send_message(std::string val);

private:
    int port;
    int fd;
    std::string ip;
    StringHandler handler;
    struct sockaddr_in serv_addr;
    int count = 0;
    int timeout = 3000;
    std::string name;
};

#endif