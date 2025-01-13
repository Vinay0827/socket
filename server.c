#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char message[1024];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Bind socket to port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a connection...\n");

    // Accept a client connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected. Waiting for messages...\n");

    while (1) {
        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));

        // Read message from client
        int valread = read(new_socket, buffer, 1024);
        if (valread > 0) {
            printf("Client: %s\n", buffer);

            // Break the loop if the client sends "exit"
            if (strcmp(buffer, "exit") == 0) {
                printf("Client disconnected.\n");
                break;
            }

            // Echo the message back to the client
            printf("You: ");
            fgets(message, 1024, stdin);
            message[strcspn(message, "\n")] = 0; // Remove trailing newline
            send(new_socket, message, strlen(message), 0);
        } else {
            printf("Client disconnected.\n");
            break;
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

