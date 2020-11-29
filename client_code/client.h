#ifndef CLIENT_H
#define CLIENT_H
#include "string_handler.h"
#include "../common/global.h"
#include "../common/tands.h"
class Client
{
public:
    // constructor for the client
    Client(std::string ip);

    // function that runs the entire program from initialization to finish
    void run(std::string port);

    // create then address
    void initialize();

    // get input from the terminal
    bool get_work();

    // connect to server when there is a message
    bool connect_server();

    // get response from server
    bool get_response();

    // send message to server
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