#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // Create a UDP socket
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Enable UDP broadcast
    int enableBroadcast = 1;
    if (setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)) == -1) {
        std::cerr << "Error setting broadcast option" << std::endl;
        return -1;
    }

    // Server address information
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Port number
    serverAddress.sin_addr.s_addr = INADDR_BROADCAST;

    char message[1024];

    while (true) {
        // Get user input
        std::cout << "Server: ";
        std::cin.getline(message, sizeof(message));

        // Send a broadcast message
        sendto(udpSocket, message, strlen(message), 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    }

    // Close the socket
    close(udpSocket);

    return 0;
}
