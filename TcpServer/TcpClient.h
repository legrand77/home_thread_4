#pragma once
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#endif // _WIN32
#include <cstdint>
#include <string>
#include <iostream>

using std::cout;
using std::endl;

enum class SocketStatus : uint8_t
	{
	    connected = 0,
		listening,
		err_socket_init,
		err_socket_bind,
		err_socket_connect,
		err_socket_listen,
		disconnected
	};

class TcpClent{
	#ifdef _WIN32
	WSADATA _wsa_data_;
	#endif // _WIN32
  	SOCKET _tcp_socket_ = INVALID_SOCKET;
	sockaddr_in _server_addr_ = {};
	char _message_[18] = "Hello from client";
    public:
	SocketStatus init_socket(std::string ip, int port);
	bool handle();
};


SocketStatus TcpClent::init_socket(std::string ip, int port){
      int result = WSAStartup(MAKEWORD(2, 2), &_wsa_data_);
    if (result != 0){
	             return SocketStatus::err_socket_init;
	}
	  _tcp_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	     if (_tcp_socket_ == INVALID_SOCKET){
		        WSACleanup();
		        return SocketStatus::err_socket_init;
		 }
     _server_addr_.sin_family = AF_INET;
     _server_addr_.sin_addr.s_addr = inet_addr(ip.c_str());
     _server_addr_.sin_port = htons(port);
     result = connect(_tcp_socket_, (SOCKADDR*)&_server_addr_, sizeof(_server_addr_));
	     if (result == SOCKET_ERROR){
		        result = closesocket(_tcp_socket_);
		        WSACleanup();
		        return SocketStatus::err_socket_connect;
		 }
	 	        return SocketStatus::connected;
}

bool TcpClent::handle(){
     char recvbuf[32];
     auto res = send(_tcp_socket_, _message_, sizeof(_message_), 0);
     if (res < 0){
	     return false;
	 }
	 res = recv(_tcp_socket_, recvbuf, sizeof(recvbuf), 0);
	 if (res < 0){
	     return false;
	 }
     else{
	        std::cout << recvbuf << std::endl;
	 }
        return true;
}