//Client side impl


//Includes
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>	
#include <string>
#include <fstream>
#include <thread>
#include <vector>


//Create a vector of threads
std::vector<std::thread> threads;



//Send file to server, lambda function
auto handleFile = [](SOCKET clientSocket) {

	//File transfer client side
	//First we have to create a string for the file path
	std::string filePath;

	std::cout << "Hey , welcome to the Server :)" << std::endl;
	std::cout << "Enter file path: ";
	//get the line for the file
	std::getline(std::cin, filePath);


	//open the file to be read
	std::ifstream inputFile(filePath, std::ios::binary);
	if (!inputFile) {
		std::cerr << "Failed to open file for reading" << std::endl;
		//close socket and cleanup dll
		system("pause");
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	else {
		std::cout << "Success, reading file in input file" << std::endl;
	}

	//seekg iterates to the end of the file and counts its size
	inputFile.seekg(0, std::ios::end);

	//Save the file size by using tellg, after doing this we have to put the
	//pointer back the the beginning with ::beg
	int fileSize = inputFile.tellg();
	std::cout << "Size of file " << fileSize << std::endl;
	inputFile.seekg(0, std::ios::beg);



	//Send data in chunks, create buffer
	const int bufferSize = 1024;
	char buffer[bufferSize];


	//Read the data into the buffer
	//Read in chunks


	//The goal is the send the data in chunks, create a loop
	//First we want a variable that holds the total bytes sent, this will eventually be equal to the file size
	int bytesSentTotal = 0;

	//We want to create a while loop that iterates while bytes sent is less then the file size
	int bytesRead;


	while (bytesSentTotal < fileSize) {

		//Reads the bytes into the buffer, and also gets the count to how many bytes were read
		//Reads a chunk, then sends a chunk
		bytesRead = inputFile.read(buffer, bufferSize).gcount();

		//Send a chunk of data
		//If bytesSentTotal is filesize - buffersize, then we can add a buffersize worth
		if (bytesSentTotal < fileSize - bufferSize) {
			//Send a chunk of data as large as the buffer
			int bytesSent = send(clientSocket, buffer, bufferSize, 0);
			if (bytesSent == SOCKET_ERROR) {
				std::cout << "Send failed! " << WSAGetLastError() << std::endl;
				break;
			}
			//Update the total
			bytesSentTotal += bytesSent;

		}
		//This means the bytessent is smaller than the buffer, so we do not want to send
		//the full buffer, we just want to send the remaining buffer read from the read operation
		else {
			int bytesSent = send(clientSocket, buffer, bytesRead, 0);
			//Update the total
			bytesSentTotal += bytesSent;

		}

		//Show progress
		std::cout << "Bytes sent: " << bytesSentTotal << " / " << fileSize << std::endl;
	}

	std::cout << "Bytes sent: " << bytesSentTotal << std::endl;
	std::cout << "File size : " << fileSize << std::endl;
	//close the file we are reading from
	inputFile.close();
};




int main() {

	//Specify which port to connect to in an int variable
	int port = 55555;



	//----------------------------------------------------------------------//
	// Initialize the DLL
	//----------------------------------------------------------------------//

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


	//----------------------------------------------------------------------//
	// Create a socket
	//----------------------------------------------------------------------//

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

	//----------------------------------------------------------------------//
	// Connect clientSocket to serverSocket
	//----------------------------------------------------------------------//


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
	}
	else {
		std::cout << "Client: Connected! " << std::endl;
		std::cout << "Client: Can start sending and receiving data..." << std::endl;

		//Create a thread
		//detach it 
		//push the thread into the vector
		std::thread t1(handleFile, clientSocket);
		threads.push_back(std::move(t1));
		t1.join();

	}

	//A new socket is now created for the client, the socket used to connect to the server
	//is the same socket used for the client


	//----------------------------------------------------------------------//
	// Once connected, send / receive files
	//----------------------------------------------------------------------//

	
	system("pause");
	//close the file after its done reading it to the buffer
	std::cout << "closed" << std::endl;
	//close the socket
	closesocket(clientSocket);
	//cleanup socket dll
	WSACleanup();



	return 0;
}



