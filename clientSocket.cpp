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
	InetPton(AF_INET, (L"192.168.87.22"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		std::cout << "Client: connect() - Failed to connect." << std::endl;
		WSACleanup();
		std::cin.get();
		system("pause");
		return 0;
	}else{
		std::cout << "Client: Connected! " << std::endl;
		std::cout << "Client: Can start sending and receiving data..." << std::endl;
	
	
	}



	//A new socket is not created for the client, the socket used to connect to the server
	//is the same socket used for the client



	//TCP requires a connection to be established
	//We can use the send() function to send data
	//Send will return an int of the number of bytes sent, or socket error if unsuccesful
	//We need to create a buffer that will be used to send the data 


	for (int i = 0; i < 1000; i++) {
		//SEND

		const int bufferSize = 200;

		//create buffer
		char buffer[bufferSize];



		//Enter message to be sent to server/client
		std::cout << "Enter your message:";

		//Store message in buffer
		std::cin.getline(buffer, bufferSize);

		//use the send function to send out the message, and do error handling
		//send returns an int of the amount of bytes sent
		int byteCount = send(clientSocket, buffer, bufferSize, 0);
		if (byteCount == SOCKET_ERROR) {
			std::cout << "Sever send error: " << WSAGetLastError() << std::endl;
			return -1;
		}
		else {
			/*std::cout << "Success! Server received: " << byteCount << " bytes of data.." << std::endl;*/
		}






		//RECIEVE
		//Recieve data incoming from the client with recv
		//almost the same process
		const int recievebufferSize = 200;
		char recieveBuffer[recievebufferSize];



		int recievebyteCount = recv(clientSocket, recieveBuffer, recievebufferSize, 0);
		if (recievebyteCount < 0) {
			std::cout << "Server: error " << WSAGetLastError;
			return 0;
		}
		else {
			//If its successful the recieve buffer will be the reply
			std::cout << "Server: " << recieveBuffer << std::endl;

		}


	}

	std::cin.get();
	system("pause");
	WSACleanup();





	return 0;
}

