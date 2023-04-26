#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <Ws2tcpip.h>

#include "winsock2.h"

#pragma comment(lib, "Ws2_32.lib")

/*
	Server Functions

initialize WSA - WSAStartup()
Create a socket - socket()
Bind the socket - bind()
Listen on the socket - listen()
Accept a connection - accept(), connect()
Send and recive data - recv(), send(), recvfrom(), sendto()
Disconnect - closesocket()

*/

/*
	Client Functions

initialize WSA - WSAStartup()
Create a socket - socket()
Connect to the server - connect()
Send and recive data - recv(), send(), recvfrom(), sendto()
Disconnect - closesocket()

*/

int server() {

	WSADATA	wsaData; // initialize WSA
	int wsaerr;

	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0) {
		std::cout << "Winsock not found!" << std::endl;
	}
	else {
		std::cout << "Found Winsock dll! " << std::endl;
		std::cout << "status: " << wsaData.szSystemStatus << std::endl;
	}

	SOCKET serverSocket = INVALID_SOCKET; // create a socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET) {

		std::cout << "Error at Socket: " << WSAGetLastError() << std::endl;
		WSACleanup();

		return 0;
	}
	else {

		std::cout << "Socket() is OK! " << std::endl;
	}

	sockaddr_in service;// binding info to socket
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(55555);

	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {

		std::cout << "Bind() failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {

		std::cout << "Bind is OK! " << std::endl;
	}

	if (listen(serverSocket, 1) == SOCKET_ERROR) { // listens for a connection

		std::cout << "Error listening on socket... " << WSAGetLastError() << std::endl;
	}
	else {

		std::cout << '\n'; std::cout << "Waiting for connections... " << std::endl;
	}

	SOCKET acceptSocket; // accepts an incoming connection on a socket
	acceptSocket = accept(serverSocket, NULL, NULL);

	if (acceptSocket == INVALID_SOCKET) {

		std::cout << "Accept Failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}

	char server_inital[300];

	int byteCount = recv(acceptSocket, server_inital, 300, 0);

	if (byteCount < 0) {

		printf("Client:error %id.\n", WSAGetLastError());
		return 0;
	}
	else {

		printf("Received data: %s \n", server_inital);

	}

	closesocket(serverSocket); // closes socket
	WSACleanup();

	return 1;
}

int main() {

	server();

	return 1;
}