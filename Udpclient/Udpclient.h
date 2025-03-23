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

class UdpClient {
    WSADATA _wsa_;
    SOCKET _socket_;
    struct sockaddr_in _server_sddr_ {}, _client_addr_{};
    char* _rec_buf_ = new char[32];
    int _port_ = 12345;
public:
    UdpClient() {

        int err = WSAStartup(MAKEWORD(2, 2), &_wsa_);
        _socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        _server_sddr_.sin_family = AF_INET;
        std::string ip = "127.0.0.1";
        _server_sddr_.sin_addr.s_addr = inet_addr(ip.c_str());
        _server_sddr_.sin_port = htons(_port_);
    }
    ~UdpClient() {
        closesocket(_socket_);
        WSACleanup();
    }

    void send() {

        int server_size = sizeof(_server_sddr_);
        std::string message = "Hello from client";
        sendto(_socket_, message.c_str(), message.size(), 0, (const struct sockaddr*)&_server_sddr_, server_size);
        int size = recvfrom(_socket_, _rec_buf_, 32, 0, (struct sockaddr*)&_server_sddr_, &server_size);
        if (size > 0)
        {
            cout << _rec_buf_ << endl;
        }
    }

};
