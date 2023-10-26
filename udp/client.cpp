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

    // Client address information
    sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(12345); // Port number
    clientAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to receive broadcasts
    if (bind(udpSocket, (struct sockaddr*)&clientAddress, sizeof(clientAddress)) == -1) {
        std::cerr << "Error binding" << std::endl;
        return -1;
    }

    char message[1024];
    sockaddr_in senderAddress;
    socklen_t senderAddressLength = sizeof(senderAddress);

    while (true) {
        // Receive broadcast messages
        ssize_t bytesRead = recvfrom(udpSocket, message, sizeof(message), 0, (struct sockaddr*)&senderAddress, &senderAddressLength);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving message" << std::endl;
            break;
        }

        // Print the received message
        std::cout << "Received from " << inet_ntoa(senderAddress.sin_addr) << ": " << message << std::endl;
    }

    // Close the socket
    close(udpSocket);

    return 0;
}
