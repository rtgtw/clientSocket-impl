//Client side socket that will connect to a server


#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>	

int main() {

	//Client Side
	

	//Specify which port to connect to in an int variable
	int port = 55555;

	//Initialize the DLL

	//Create a wsaData variable to pass into WsaStartup
	WSADATA wsaData;

	int wsaerr;

	//format version
	WORD wVersionRequested = MAKEWORD(2, 2);

	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr) {
		std::cout << "Winsock dll not found" << std::endl;
		return 0;
	}
	else {
		std::cout << "Winsock dll found" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}


	//Create a socket
	//This specifies the protocol to use, either TCP or UDP
	 //TCP socket
	//Create a socket variable an initialize it to invalid socket
	//Right now the socket is unbound
	SOCKET clientSocket;
	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		//if there is an error, clean up the socket
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "Socket() is OK" << std::endl;
	}



	//After creating the socket, now we have to connect to server
	//The binding happens automatically by the OS which assigns a port when we call the connect function

	//Create socket address service variable to store server ip/port details to connect
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, (L"127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		std::cout << "Client: connect() - Failed to connect." << std::endl;
		WSACleanup();
		return 0;
	}else{
		std::cout << "Client: Connected! " << std::endl;
		std::cout << "Client: Can start sending and receiving data..." << std::endl;
	
	}
	system("pause");
	WSACleanup();





	return 0;
}

