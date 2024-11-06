#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 62626
#define LISTEN_BACKLOG 5
#define BUFFER_SIZE 1024

void reverse_string(char* str, int length) {
    for (int i = 0; i < length / 2; ++i) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
    //new line as last char
    if(str[length - 1] != '\n') {
        str[length] = '\n';
        str[length + 1] = '\0';
    }
}

void handle_connection(int a_client) {
    char buffer[BUFFER_SIZE];
    int read_bytes;
    while ((read_bytes = read(a_client, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[read_bytes] = '\0'; 
        printf("received: %s", buffer);
        reverse_string(buffer, read_bytes);
        write(a_client, buffer, read_bytes);
    }
    close(a_client);
}


int main(int argc, char* argvp[]) {

    int return_value, socket_fd, client_fd, port;

    //default port 62626
    port = PORT;

    //specify port
    if(argc == 3 && strcmp(argvp[1], "-p") == 0) {
        port = atoi(argvp[2]);
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    return_value = bind(socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address));
    return_value = listen(socket_fd, LISTEN_BACKLOG);

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    while(1) {
        client_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_len);
        handle_connection(client_fd);
    }
    close(socket_fd);
    return 0;
}