#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>   

int main() {
    int fd = socket(AF_INET, // an Internet socket
                    SOCK_STREAM, // reliable stream-like socket
                    0); // OS determine the protocol (TCP)

    if (fd < 0)
        return 1; // something went wrong

    struct sockaddr_in serv_addr; 

    // setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(6000); // port

    if (bind(fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) // bind socket to the server address
        return 1;

    printf("Listening on port %d for clients.\n", 6000);
    if (listen(fd, 1) < 0) // wait for clients, only 1 is allowed.
        return 1;

    while(1) {
        
        struct sockaddr_in client_address; // client address
        int len = sizeof(client_address); 

        printf("Waiting for clients...\n");
        int client_fd = accept(fd, (struct sockaddr*) &client_address, &len);  // accept connection

        if (client_fd < 0) // bad connection
            continue; // discard

        printf("Client connected.\n");
        char buffer[1024];
        int count = 0;
        do {
            count = read(client_fd, buffer, sizeof(buffer));
            if(count > 0) {
                buffer[count] = '\0';
                printf("%s\n", buffer);
            }
        } while(count > 0);
        
        printf("Client disconnected.\n");
    }
    

    return 0;
} 