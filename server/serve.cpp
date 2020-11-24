#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include "server.h"

void Server::run(std::string port)
{
    if (!std::regex_match(port, std::regex("[0-9]+")))
        return;

    this->port = std::stoi(port);

    if (this->port < 5000 || this->port > 64001)
        return;

    if (this->initialize())
    {
        this->listen_client();
    }
}

bool Server::initialize()
{
    this->fd = socket(AF_INET,     // an Internet socket
                      SOCK_STREAM, // reliable stream-like socket
                      0);
    if (fd < 0)
        return false; // something went wrong

    struct sockaddr_in serv_addr;

    // setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->port); // port

    // bind socket to the server address
    if (bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        return false;

    return true;
}

bool Server::listen_client()
{
    int read_size = 0;

    printf("Listening on port %d for clients.\n", this->port);
    if (listen(this->fd, 100) < 0) // wait for clients, only 1 is allowed.
        return false;

    while (1)
    {

        std::string ans = "";
        struct sockaddr_in client_address; // client address
        int len = sizeof(client_address);

        printf("Waiting for clients...\n");
        int client_fd = accept(fd, (struct sockaddr *)&client_address, (socklen_t *)&len); // accept connection

        if (client_fd < 0) // bad connection
            continue;      // discard

        printf("Client connected.\n");
        char buffer[1000];

        //Receive a message from client

        while ((read_size = recv(client_fd, buffer, 1000, 0)) > 0)
        {
            //Send the message back to client
            ans += buffer;
            memset(buffer, 0, 1000);
        }

        write(client_fd, buffer, strlen(buffer));
        if (read_size == -1)
        {
            perror("recv failed");
        }

        printf("Client disconnected.\n");
    }
}