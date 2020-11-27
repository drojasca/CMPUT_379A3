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
    else
    {
        std::cout << "error" << std::endl;
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

    if (listen(this->fd, 100) < 0) // wait for clients, only 1 is allowed.
        return false;

    this->fds[0].fd = this->fd;
    this->fds[0].events = POLLIN;

    return true;
}

bool Server::listen_client()
{
    int read_size;
    char buffer[1000];

    printf("Listening on port %d for clients.\n", this->port);

    while (1)
    {
        memset(buffer, 0, 1000);
        printf("Waiting on poll()...\n");
        this->rc = poll(fds, this->nfs, this->timeout);

        if (rc < 0)
        {
            perror("  poll() failed");
            return false;
        }

        if (rc == 0)
        {
            printf("  poll() timed out.  End program.\n");
            return false;
        }

        int c = sizeof(struct sockaddr_in);
        int client;

        //accept connection from an incoming client
        int client_fd = accept(this->fd, (struct sockaddr *)&client, (socklen_t *)&c);
        if (client_fd < 0)
        {
            perror("accept failed");
            return false;
        }

        puts("Connection accepted");

        std::string ans = "";

        while ((read_size = recv(client_fd, buffer, 1000, 0)) > 0)
        {
            //Send the message back to client
            ans += buffer;
            std::cout << ans << std::endl;
            Trans(std::stoi(ans));
            write(client_fd, buffer, strlen(buffer));
            std::cout << "HI" << std::endl;
            memset(buffer, 0, 1000);
        }

        // memset(fds, 0, sizeof(fds));
        // this->fds[0].fd = this->fd;
        // this->fds[0].events = POLLIN;
    }

    return true;
}
