#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Port number
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding" << std::endl;
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening" << std::endl;
        return -1;
    }

    // Accept a connection from a client
    sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        std::cerr << "Error accepting client connection" << std::endl;
        return -1;
    }

    char message[1024];
    while (true) {
        memset(message, 0, sizeof(message));

        // Receive message from client
        ssize_t bytesRead = recv(clientSocket, message, sizeof(message), 0);
        if (bytesRead <= 0) {
            std::cerr << "Connection closed by client" << std::endl;
            break;
        }

        std::cout << "Client: " << message << std::endl;

        // Send message to client
        std::cout << "Server: ";
        std::cin.getline(message, sizeof(message));
        send(clientSocket, message, strlen(message), 0);
    }

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
