#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // 서버의 IP 주소와 포트 설정
    const char* server_ip = "172.17.0.4"; // 서버의 실제 IP 주소로 대체
    const int server_port = 12345;

    // 소켓 생성
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        std::cerr << "소켓을 생성할 수 없습니다." << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    char buffer[1024];

    while (true) {
        // 클라이언트로부터 메시지 입력
        std::cout << "클라이언트: ";
        std::cin.getline(buffer, sizeof(buffer));

        // 서버에게 메시지 보내기
        sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        // 서버로부터 응답 메시지 받기
        sockaddr_in server_response_addr;
        socklen_t server_response_addr_len = sizeof(server_response_addr);
        int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_response_addr, &server_response_addr_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "서버: " << buffer << std::endl;
        }
    }

    close(client_socket);
    return 0;
}
