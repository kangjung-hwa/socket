#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // 서버의 IP 주소와 포트 설정
    const char* server_ip = "0.0.0.0";  // 모든 네트워크 인터페이스에서 수신
    const int server_port = 12345;

    // 소켓 생성
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        std::cerr << "소켓을 생성할 수 없습니다." << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // 소켓을 주소와 포트에 바인딩
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "바인딩에 실패했습니다." << std::endl;
        close(server_socket);
        return -1;
    }

    std::cout << "서버가 " << server_ip << ":" << server_port << "에서 대기 중입니다." << std::endl;

    char buffer[1024];
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (true) {
        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "클라이언트: " << buffer << std::endl;

            // 서버로부터 메시지 입력
            std::cout << "서버: ";
            std::cin.getline(buffer, sizeof(buffer));

            // 클라이언트에게 메시지 보내기
            sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_len);
        }
    }

    close(server_socket);
    return 0;
}
