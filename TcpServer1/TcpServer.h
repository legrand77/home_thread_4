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
#include <thread>

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

class TcpServer
	 {
	     WSADATA _wsa_data_ = {};
	     SOCKET _listen_socket_ = INVALID_SOCKET;
	     sockaddr_in _server_addr_ = {};
	     sockaddr_in _client_addr_ = {};
	     SOCKET _client_socket_;
	    char server_answer[16] = "Server Answer!!";
	
 public:
	     SocketStatus init_socket(int port);
	     void run();
	     void client_handler(SOCKET * client_socket);
	 };

SocketStatus TcpServer::init_socket(int port){
	     int result = WSAStartup(MAKEWORD(2, 2), &_wsa_data_);
	    if (result != 0){
		    return SocketStatus::err_socket_init;
		}
	     _listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	    if (_listen_socket_ == INVALID_SOCKET){
		    WSACleanup();
		    return SocketStatus::err_socket_init;
		}
	         _server_addr_.sin_family = AF_INET;
	         _server_addr_.sin_addr.s_addr = ADDR_ANY;
	         _server_addr_.sin_port = htons(port);
	
		     result = bind(_listen_socket_, (SOCKADDR*)&_server_addr_, sizeof(_server_addr_));
	         if (result == SOCKET_ERROR){
		         result = closesocket(_listen_socket_);
		         WSACleanup();
		         return SocketStatus::err_socket_bind;
		     }
	         if (listen(_listen_socket_, SOMAXCONN) == SOCKET_ERROR){
		         result = closesocket(_listen_socket_);
		         WSACleanup();
		         return SocketStatus::err_socket_listen;
		     }
	 	return SocketStatus::listening;
}

void TcpServer::run(){
     while (true){
	    int addrlen = sizeof(_client_addr_);
		cout << "∆ду сообщени€ от клиента....." << endl;
		if ((_client_socket_ = accept(_listen_socket_, (struct sockaddr*)&_client_addr_, &addrlen)) == INVALID_SOCKET){
		    closesocket(_listen_socket_);
			WSACleanup();
		}
	  std::thread(&TcpServer::client_handler, this, &_client_socket_).detach();
	 }
}

void TcpServer::client_handler(SOCKET* client_socket){
     char recvbuf[1024];
    int result = recv(*client_socket, recvbuf, sizeof(recvbuf), 0);
    if (result > 0){
         std::cout << recvbuf << std::endl;
        send(*client_socket, server_answer, 16, 0);
	 }
 }