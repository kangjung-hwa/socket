#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Port number
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to the server" << std::endl;
        return -1;
    }

    char message[1024];
    while (true) {
        // Send message to server
        std::cout << "Client: ";
        std::cin.getline(message, sizeof(message));
        send(clientSocket, message, strlen(message), 0);

        memset(message, 0, sizeof(message));

        // Receive message from server
        ssize_t bytesRead = recv(clientSocket, message, sizeof(message), 0);
        if (bytesRead <= 0) {
            std::cerr << "Connection closed by server" << std::endl;
            break;
        }

        std::cout << "Server: " << message << std::endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
