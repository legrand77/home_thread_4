#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

class UdpServer {
    WSADATA _wsa_;
    SOCKET _socket_;
    struct sockaddr_in _server_sddr_{}, _client_addr_{};
    char* _rec_buf_ = new char[32];
    int _port_ = 12345;
public:
    UdpServer() {

        int err = WSAStartup(MAKEWORD(2, 2), &_wsa_);
        _socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        _server_sddr_.sin_family = AF_INET;
        _server_sddr_.sin_addr.s_addr = INADDR_ANY;
        _server_sddr_.sin_port = htons(_port_);
        err = bind(_socket_, (const struct sockaddr*)&_server_sddr_, sizeof(_server_sddr_));
        if (err == SOCKET_ERROR) {

            cout << "Сокет не создан" << endl;
            closesocket(_socket_);
            WSACleanup();
        }
    }
    ~UdpServer() {
        closesocket(_socket_);
        WSACleanup();
    }

    void recieve_and_answer() {

        int client_size = sizeof(_client_addr_);
        while (true) {

            cout << "Жду сообщения от клиента....." << endl;
            int size = recvfrom(_socket_, _rec_buf_, 32, 0, (struct sockaddr*)&_client_addr_, &client_size);
            if (size > 0)
            {
                cout << _rec_buf_ << endl;
                std::string answer = "Hello from server";
                sendto(_socket_, answer.c_str(), answer.size(), 0, (const struct sockaddr*)&_client_addr_, client_size);
            }
        }
    }

};