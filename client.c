#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char message[1024];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses
    if (inet_pton(AF_INET, "192.168.31.15", &serv_addr.sin_addr) <= 0) {  // Replace with server's IP
        printf("Invalid address/Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    printf("Connected to the server. Type your messages below:\n");

    while (1) {
        // Get message from user input
        printf("You: ");
        fgets(message, 1024, stdin);
        message[strcspn(message, "\n")] = 0; // Remove trailing newline

        // Send message to server
        send(sock, message, strlen(message), 0);

        // Break the loop if the user types "exit"
        if (strcmp(message, "exit") == 0) {
            printf("Connection closed.\n");
            break;
        }

        // Receive response from server
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        int valread = read(sock, buffer, 1024);
        if (valread > 0) {
            printf("Server: %s\n", buffer);
        } else {
            printf("Server disconnected.\n");
            break;
        }
    }

    close(sock);
    return 0;
}

